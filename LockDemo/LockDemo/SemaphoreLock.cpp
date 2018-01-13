
#include <thread>

#include "SemaphoreLock.h"

SemaphoreLock::SemaphoreLock(uint32_t maxSemCount)
	: CPULock(TestAndSetMethodOpt::SEMAPHORE)
	, mMaxSemCount(maxSemCount)
	, mSemCount(mMaxSemCount)
{
}

SemaphoreLock::~SemaphoreLock()
{
}

void SemaphoreLock::LockCPU(uint32_t cpuid)
{
	while (semaphore_lock_asm(&mPriSemBitLock, &mSemCount) == 1)
	{
		if (mCPUID == cpuid)
		{
			std::this_thread::yield();
		}
	}
}

void SemaphoreLock::UnLock()
{
	UnLockCPU();

	semaphore_unlock_asm(&mPriSemBitLock, &mSemCount, mMaxSemCount);
}

