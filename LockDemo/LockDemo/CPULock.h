#pragma once

#ifndef _CPU_LOCK_H
#define _CPU_LOCK_H

#include <Windows.h>

#include "LockBase.h"

class CPULock : public LockBase
{
	protected:
		uint32_t mCPUID = 0;
		DWORD mMaxCPUMask = 0;

		virtual void LockCPU(uint32_t cpuid);

		void UnLockCPU();

	public:
		CPULock(TestAndSetMethodOpt testAndSetMethodOpt);
		virtual ~CPULock();

		void Lock() override;

		virtual void UnLock() override;
};

#endif
