#include "SpinLock.h"


SpinLock::SpinLock(TestAndSetMethodOpt testAndSetMethodOpt)
	: LockBase(testAndSetMethodOpt)
{
}

SpinLock::~SpinLock()
{
}

void SpinLock::Lock()
{
	while (mTestAndSetCallback(&mLockBit) == 1);
}
