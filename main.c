#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define JPH1_IMPLEMENTATION
#include "jph1.h"



void gen_random_key (uint8_t *key, size_t key_size)
{
	size_t i;
	for (i = 0; i < key_size; ++ i)
	{
		key [i] = (rand () % 256);
	}
}

int main (int argc, const char **argv)
{
	#define A_SIZE 4
	#define B_SIZE 4
	uint8_t a[A_SIZE] = {0};	
	uint8_t b[B_SIZE] = {0};

	gen_random_key (a, A_SIZE);
	gen_random_key (b, B_SIZE);

	int32_t i;
	for (i = 0; i < A_SIZE; ++ i)
	{
		printf ("%s%.2X", (i == 0 ? "0x" : ""), a [i]);
	}

	printf ("\n+\n");

	for (i = 0; i < B_SIZE; ++ i)
	{
		printf ("%s%.2X", (i == 0 ? "0x" : ""), b [i]);
	}

	printf ("\n=\n");

	int64_t key_size = 0;
	uint8_t *key = jph1_sub (a, A_SIZE, b, B_SIZE, &key_size);	
	for (i = 0; i < key_size; ++ i)
	{
		printf ("%s%.2X", (i == 0 ? "0x" : ""), key [i]);
	}

	printf ("\n");
	return (0);
}

