
#pragma once

#ifndef _LOCK_BASE_H
#define _LOCK_BASE_H

#include <stdint.h>

#include "LockDemoDefs.h"

class LockBase abstract
{
	protected:
		uint32_t mLockBit = 0;
		TestAndSetMethodOpt mTestAndSetMethodOpt = TestAndSetMethodOpt::CMPXCHG;
		TestAndSetCallback mTestAndSetCallback;

	public:
		LockBase(TestAndSetMethodOpt testAndSetMethodOpt);

		virtual ~LockBase();

		virtual void Lock() = 0;

		virtual void UnLock();
};

#endif
