
#include <thread>

#include "YieldLock.h"


YieldLock::YieldLock(TestAndSetMethodOpt testAndSetMethodOpt)
	: LockBase(testAndSetMethodOpt)
{
}

YieldLock::~YieldLock()
{
}

void YieldLock::Lock()
{
	while (mTestAndSetCallback(&mLockBit) == 1)
	{
		std::this_thread::yield();
	}
}
