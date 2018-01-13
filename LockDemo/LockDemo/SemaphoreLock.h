#pragma once

#ifndef _SEMAPHORE_LOCK_H
#define _SEMAPHORE_LOCK_H

#include "CPULock.h"

class SemaphoreLock sealed : public CPULock
{
	private:
		uint32_t mPriSemBitLock = 1;
		uint32_t mMaxSemCount = 0;
		uint32_t mSemCount = 0;

		void LockCPU(uint32_t cpuid) override;

	public:
		SemaphoreLock(uint32_t maxSemCount);
		~SemaphoreLock();

		void UnLock() override;
};

#endif
