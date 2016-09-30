#pragma once

#ifndef _YIELD_LOCK_H
#define _YIELD_LOCK_H

#include "LockBase.h"

class YieldLock sealed : public LockBase
{
	public:
		YieldLock(TestAndSetMethodOpt testAndSetMethodOpt);
		~YieldLock();

		void Lock() override;
};

#endif
