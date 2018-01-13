#pragma once

#ifndef _SPIN_LOCK_H
#define _SPIN_LOCK_H

#include "LockBase.h"

class SpinLock sealed : public LockBase 
{
	public:
		SpinLock(TestAndSetMethodOpt testAndSetMethodOpt);
		~SpinLock();

		void Lock() override;
};

#endif
