#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

uint8_t full_add1 (uint8_t a, uint8_t b, uint8_t c, uint8_t *co)
{
	uint8_t sum = ((a & 0x01) ^ (b & 0x01));
	if (co != NULL)
	{
		*co = ((a & 0x01) & (b & 0x01)) | ((c & 0x01) & sum);
	}

	return (sum ^ (c & 0x01));
}

uint8_t add8 (uint8_t a, uint8_t b, uint8_t c, uint8_t *co)
{
	uint8_t sum = 0;
	uint8_t c_out = 0;
	int32_t i;
	for (i = 0; i < 8; ++ i)
	{
		sum |= (full_add1 ((a >> i) & 0x01, (b >> i) & 0x01, (i == 0 ? (c >> i) & 0x01 : c_out), &c_out) << i);
	}

	*co = c_out;
	return (sum);
}

uint8_t *bigint_add (uint8_t *a, int64_t a_size, uint8_t *b, int64_t b_size, int64_t *out_size)
{
	*out_size = (a_size > b_size ? a_size + 1 : b_size + 1);
	uint8_t *result = (uint8_t *) malloc (*out_size);
	
	int64_t result_index = (*out_size) - 1;
	uint8_t c = 0;
	int64_t i, j;
	for (i = a_size - 1, j = b_size - 1; i >= 0 || j >= 0; -- i, -- j)
	{
		uint8_t ai;
		uint8_t bj;
		if (i < 0) { ai = 0; }
		else { ai = a [i]; }

		if (j < 0) { bj = 0; }
		else { bj = b [j]; }

		result [result_index --] = add8 (ai, bj, c, &c);
	}

	result [result_index --] = c;
	while (result_index > 0)
	{
		result [result_index --] = 0;
	}

	return (result);
}

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
	uint8_t a[4096] = {0};	
	uint8_t b[4096] = {0};

	gen_random_key (a, 4096);
	gen_random_key (b, 4096);

	int32_t i;
	for (i = 0; i < 4096; ++ i)
	{
		printf ("%s%.2X", (i == 0 ? "0x" : ""), a [i]);
	}

	printf ("\n+\n");

	for (i = 0; i < 4096; ++ i)
	{
		printf ("%s%.2X", (i == 0 ? "0x" : ""), b [i]);
	}

	printf ("\n=\n");

	int64_t key_size = 0;
	uint8_t *key = bigint_add (a, 4096, b, 4096, &key_size);	
	for (i = 0; i < key_size; ++ i)
	{
		printf ("%s%.2X", (i == 0 ? "0x" : ""), key [i]);
	}

	printf ("\n");
	return (0);
}

