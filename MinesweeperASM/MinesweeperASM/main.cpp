
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

extern "C"
{
	extern void play_asm();

	uint32_t helper_random(uint32_t max)
	{
		return  (rand() % max);
	}

	void test()
	{
		printf("Test");
		scanf("Test");
	}
}

int main()
{
	srand((unsigned int)time(NULL));

	play_asm();

	return EXIT_SUCCESS;
}
