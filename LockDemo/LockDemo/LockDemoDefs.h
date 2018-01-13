#pragma once

#ifndef _LOCK_DEMO_DEFS
#define _LOCK_DEMO_DEFS

#include <stdint.h>
#include <functional>

#define NUM_THREAD_LOCK 100000

#define NUM_THREAD_LOOP 10

#define DeleteMem(x) if(x != nullptr) {delete x; x = nullptr;}

extern "C"
{
	extern uint32_t test_and_set_cmpxchg_asm(uint32_t* lockBit);
	extern uint32_t test_and_set_btr_asm(uint32_t* lockBit);

	extern uint32_t semaphore_lock_asm(uint32_t* pri_sembit_lock, uint32_t* sem_count);
	extern void semaphore_unlock_asm(uint32_t* pri_sembit_lock, uint32_t* sem_count, uint32_t max_sem_count);
};

enum class TestAndSetMethodOpt : uint32_t
{
	CMPXCHG = 0,
	BTR,
	SEMAPHORE
};

typedef std::function<uint32_t(uint32_t*)> TestAndSetCallback;

#endif