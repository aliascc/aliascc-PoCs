
#include <thread>
#include <iostream>
#include <stdint.h>
#include <chrono>
#include <Windows.h>
#include <functional>

using namespace std;

#define NUM_THREAD_LOCK 100000
#define NUM_THREAD_LOOP 10
#define DeleteMem(x) if(x != nullptr) {delete x; x = nullptr;}

extern "C"
{
	extern uint32_t test_and_set_cmpxchg_asm(uint32_t* lockBit, uint32_t toSet);
	extern uint32_t test_and_set_btr_asm(uint32_t* lockBit, uint32_t toSet);

	extern uint32_t semaphore_lock_asm(uint32_t* pri_sembit_lock, uint32_t* sem_count);
	extern void semaphore_unlock_asm(uint32_t* pri_sembit_lock, uint32_t* sem_count, uint32_t max_sem_count);
};

class MyLock
{
	protected:
		uint32_t mLockBit = 0;

	public:
		MyLock()
		{
		}

		virtual ~MyLock()
		{
			UnLock();
		}

		virtual void Lock() = 0;

		virtual void UnLock()
		{
			mLockBit = 0;
		}
};

class MySpinLock sealed : public MyLock
{
	public:
		MySpinLock()
		{
		}

		~MySpinLock()
		{
		}

		void Lock() override
		{
			while (test_and_set_cmpxchg_asm(&mLockBit, 1) == 1);
		}
};

class MyYieldLock sealed : public MyLock
{
	public:
		MyYieldLock()
		{
		}

		~MyYieldLock()
		{
		}

		void Lock() override
		{
			while (test_and_set_cmpxchg_asm(&mLockBit, 1) == 1)
			{
				std::this_thread::yield();
			}
		}
};

class MyCPULock : public MyLock
{
	protected:
		uint32_t mCPUID = 0;
		DWORD mMaxCPUMask = 0;

		virtual void LockCPU(uint32_t cpuid)
		{
			while (test_and_set_cmpxchg_asm(&mLockBit, 1) == 1)
			{
				if (mCPUID == cpuid)
				{
					std::this_thread::yield();
				}
			}
		}

		void UnLockCPU()
		{
			SetThreadAffinityMask(GetCurrentThread(), mMaxCPUMask);
		}

	public:

		MyCPULock()
		{
			DWORD maxProcessor = GetMaximumProcessorCount(0);

			mMaxCPUMask += 1;
			for (DWORD i = 1; i < maxProcessor; i++)
			{
				mMaxCPUMask <<= 1;

				mMaxCPUMask += 1;
			}
		}

		~MyCPULock()
		{
		}

		void Lock() override
		{
			uint32_t cpuid = GetCurrentProcessorNumber();
			SetThreadAffinityMask(GetCurrentThread(), 1 << cpuid);

			LockCPU(cpuid);

			mCPUID = cpuid;
		}

		void UnLock() override
		{
			UnLockCPU();

			mLockBit = 0;
		}
};

class MySemaphore sealed : public MyCPULock
{
	private:
		uint32_t mPriSemBitLock = 1;
		uint32_t mMaxSemCount = 0;
		uint32_t mSemCount = 0;

		void LockCPU(uint32_t cpuid) override
		{
			while (semaphore_lock_asm(&mPriSemBitLock, &mSemCount) == 1)
			{
				if (mCPUID == cpuid)
				{
					std::this_thread::yield();
				}
			}
		}

	public:
		MySemaphore(uint32_t maxSemCount)
			: mMaxSemCount(maxSemCount)
			, mSemCount(mMaxSemCount)
		{
		}

		void UnLock() override
		{
			UnLockCPU();

			semaphore_unlock_asm(&mPriSemBitLock, &mSemCount, mMaxSemCount);
		}
};

MyLock* myLock = nullptr;
int32_t gTestNum = 0;

void testNumIncNoThreadLock(int32_t& addition)
{
	for (size_t i = 0; i < NUM_THREAD_LOOP; i++)
	{
		int32_t tempNum = gTestNum;

		//"do work..."
		tempNum++;

		gTestNum = tempNum;

		addition += gTestNum;
	}
}

void TestNoLock()
{
	std::thread myThreads[NUM_THREAD_LOCK];
	int32_t addition = 0;

	gTestNum = 0;

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i] = std::thread(testNumIncNoThreadLock, std::ref(addition));
	}

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i].join();
	}
}

void testNumIncThreadLock(int32_t& addition)
{
	for (size_t i = 0; i < NUM_THREAD_LOOP; i++)
	{
		myLock->Lock();

		int32_t tempNum = gTestNum;

		//"do work..."
		tempNum++;
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));

		gTestNum = tempNum;

		addition += gTestNum;

		myLock->UnLock();
	}
}

void TestSpinLock()
{
	std::thread myThreads[NUM_THREAD_LOCK];
	int32_t addition = 0;

	DeleteMem(myLock);
	myLock = new MySpinLock();

	gTestNum = 0;

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i] = std::thread(testNumIncThreadLock, std::ref(addition));
	}

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i].join();
	}
}

void TestYieldLock()
{
	std::thread myThreads[NUM_THREAD_LOCK];
	int32_t addition = 0;

	DeleteMem(myLock);
	myLock = new MyYieldLock();

	gTestNum = 0;

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i] = std::thread(testNumIncThreadLock, std::ref(addition));
	}

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i].join();
	}
}

void TestCPULock()
{
	std::thread myThreads[NUM_THREAD_LOCK];
	int32_t addition = 0;

	DeleteMem(myLock);
	myLock = new MyCPULock();

	gTestNum = 0;

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i] = std::thread(testNumIncThreadLock, std::ref(addition));
	}

	for (size_t i = 0; i < NUM_THREAD_LOCK; i++)
	{
		myThreads[i].join();
	}
}

int main()
{
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point endTime;

	cout << "No Lock Test" << endl;
	startTime = std::chrono::high_resolution_clock::now();
	TestNoLock();
	endTime = std::chrono::high_resolution_clock::now();
	cout << "Exec Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << endl;
	cout << endl;

	cout << "Spin Lock Test" << endl;
	startTime = std::chrono::high_resolution_clock::now();
	TestSpinLock();
	endTime = std::chrono::high_resolution_clock::now();
	cout << "Exec Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << endl;
	cout << endl;

	cout << "Yield Lock Test" << endl;
	startTime = std::chrono::high_resolution_clock::now();
	TestYieldLock();
	endTime = std::chrono::high_resolution_clock::now();
	cout << "Exec Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << endl;
	cout << endl;

	startTime = std::chrono::high_resolution_clock::now();
	cout << "CPU Lock Test" << endl;
	TestCPULock();
	endTime = std::chrono::high_resolution_clock::now();
	cout << "Exec Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << endl;
	cout << endl;

	return EXIT_SUCCESS;
}
