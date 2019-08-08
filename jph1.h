#ifndef JPH1_H
#define JPH1_H

#include <stdint.h>

#ifdef JPH1_STATIC
#define JPH1_API static
#else
#define JPH1_API extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

JPH1_API uint8_t *jph1_add (uint8_t *a, int64_t a_size, uint8_t *b, int64_t b_size, int64_t *size_out);

#ifdef __cplusplus
}
#endif


#ifdef JPH1_IMPLEMENTATION
#include <stdlib.h>

uint8_t jph1__full_add1 (uint8_t a, uint8_t b, uint8_t c, uint8_t *co)
{
	uint8_t sum = ((a & 0x01) ^ (b & 0x01));
	if (co != NULL)
	{
		*co = ((a & 0x01) & (b & 0x01)) | ((c & 0x01) & sum);
	}

	return (sum ^ (c & 0x01));
}

uint8_t jph1__add8 (uint8_t a, uint8_t b, uint8_t c, uint8_t *co)
{
	uint8_t sum = 0;
	uint8_t c_out = 0;
	int32_t i;
	for (i = 0; i < 8; ++ i)
	{
		sum |= (jph1__full_add1 ((a >> i) & 0x01, (b >> i) & 0x01, (i == 0 ? (c >> i) & 0x01 : c_out), &c_out) << i);
	}

	*co = c_out;
	return (sum);
}

JPH1_API uint8_t *jph1_add (uint8_t *a, int64_t a_size, uint8_t *b, int64_t b_size, int64_t *size_out)
{
	*size_out = (a_size > b_size ? a_size + 1 : b_size + 1);
	uint8_t *result = (uint8_t *) malloc (*size_out);
	
	int64_t result_index = (*size_out) - 1;
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

		result [result_index --] = jph1__add8 (ai, bj, c, &c);
	}

	result [result_index --] = c;
	while (result_index > 0)
	{
		result [result_index --] = 0;
	}

	return (result);
}

#endif
#endif // JPH1_H

