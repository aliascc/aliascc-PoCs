
#include <thread>

#include "CPULock.h"

CPULock::CPULock(TestAndSetMethodOpt testAndSetMethodOpt)
	: LockBase(testAndSetMethodOpt)
{
	DWORD maxProcessor = GetMaximumProcessorCount(0);

	mMaxCPUMask += 1;
	for (DWORD i = 1; i < maxProcessor; i++)
	{
		mMaxCPUMask <<= 1;

		mMaxCPUMask += 1;
	}
}

CPULock::~CPULock()
{
}

void CPULock::LockCPU(uint32_t cpuid)
{
	while (test_and_set_cmpxchg_asm(&mLockBit, 1) == 1)
	{
		if (mCPUID == cpuid)
		{
			std::this_thread::yield();
		}
	}
}

void CPULock::UnLockCPU()
{
	SetThreadAffinityMask(GetCurrentThread(), mMaxCPUMask);
}

void CPULock::Lock()
{
	uint32_t cpuid = GetCurrentProcessorNumber();
	SetThreadAffinityMask(GetCurrentThread(), 1 << cpuid);

	LockCPU(cpuid);

	mCPUID = cpuid;
}

void CPULock::UnLock()
{
	UnLockCPU();

	mLockBit = 0;
}
