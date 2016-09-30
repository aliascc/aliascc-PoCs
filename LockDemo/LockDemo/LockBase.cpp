#include "LockBase.h"

LockBase::LockBase(TestAndSetMethodOpt testAndSetMethodOpt)
	: mTestAndSetMethodOpt(testAndSetMethodOpt)
{
	switch (mTestAndSetMethodOpt)
	{
		case TestAndSetMethodOpt::CMPXCHG:
			mTestAndSetCallback = std::bind(test_and_set_cmpxchg_asm, std::placeholders::_1);
			break;

		case TestAndSetMethodOpt::BTR:
			mTestAndSetCallback = std::bind(test_and_set_btr_asm, std::placeholders::_1);
			break;

		case TestAndSetMethodOpt::SEMAPHORE:
		default:
			mTestAndSetCallback = nullptr;
			break;
	}
}

LockBase::~LockBase()
{
	UnLock();
}

void LockBase::UnLock()
{
	mLockBit = 0;
}
