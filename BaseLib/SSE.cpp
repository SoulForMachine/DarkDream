
#include <xmmintrin.h>
#include "SSE.h"



/*void SSE_MultMatrices4x4(float* result, const float* mat1, const float* mat2)
{
	__asm
	{
		mov			edx, mat2
		movaps		xmm4, [edx]
		movaps		xmm5, [edx + 0x10]
		movaps		xmm6, [edx + 0x20]
		movaps		xmm7, [edx + 0x30]

		mov			edx, mat1
		mov			eax, result


		movaps		xmm0, [edx]
		movaps		xmm1, xmm0
		shufps		xmm1, xmm1, 0x0
		mulps		xmm1, xmm4

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0x55
		mulps		xmm2, xmm5
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xAA
		mulps		xmm2, xmm6
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xFF
		mulps		xmm2, xmm7
		addps		xmm1, xmm2

		movaps		[eax], xmm1



		movaps		xmm0, [edx + 0x10]
		movaps		xmm1, xmm0
		shufps		xmm1, xmm1, 0x0
		mulps		xmm1, xmm4

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0x55
		mulps		xmm2, xmm5
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xAA
		mulps		xmm2, xmm6
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xFF
		mulps		xmm2, xmm7
		addps		xmm1, xmm2

		movaps		[eax + 0x10], xmm1



		movaps		xmm0, [edx + 0x20]
		movaps		xmm1, xmm0
		shufps		xmm1, xmm1, 0x0
		mulps		xmm1, xmm4

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0x55
		mulps		xmm2, xmm5
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xAA
		mulps		xmm2, xmm6
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xFF
		mulps		xmm2, xmm7
		addps		xmm1, xmm2

		movaps		[eax + 0x20], xmm1



		movaps		xmm0, [edx + 0x30]
		movaps		xmm1, xmm0
		shufps		xmm1, xmm1, 0x0
		mulps		xmm1, xmm4

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0x55
		mulps		xmm2, xmm5
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xAA
		mulps		xmm2, xmm6
		addps		xmm1, xmm2

		movaps		xmm2, xmm0
		shufps		xmm2, xmm2, 0xFF
		mulps		xmm2, xmm7
		addps		xmm1, xmm2

		movaps		[eax + 0x30], xmm1
	}
}*/


void SSE_MultMatrices4x4(float* result, const float* mat1, const float* mat2)
{
	__m128 m2_0 = _mm_load_ps(&mat2[0]);
	__m128 m2_1 = _mm_load_ps(&mat2[4]);
	__m128 m2_2 = _mm_load_ps(&mat2[8]);
	__m128 m2_3 = _mm_load_ps(&mat2[12]);

	__m128 t1, t2;
	__m128 R0;

	__m128 m1_0 = _mm_load_ps(&mat1[0]);
	t1 = _mm_shuffle_ps(m1_0, m1_0, 0x0);
	R0 = _mm_mul_ps(m2_0, t1);
	t1 = _mm_shuffle_ps(m1_0, m1_0, 0x55);
	t2 = _mm_mul_ps(m2_1, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_0, m1_0, 0xAA);
	t2 = _mm_mul_ps(m2_2, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_0, m1_0, 0xFF);
	t2 = _mm_mul_ps(m2_3, t1);
	R0 = _mm_add_ps(t2, R0);
	_mm_store_ps(&result[0], R0);

	__m128 m1_1 = _mm_load_ps(&mat1[4]);
	t1 = _mm_shuffle_ps(m1_1, m1_1, 0x0);
	R0 = _mm_mul_ps(m2_0, t1);
	t1 = _mm_shuffle_ps(m1_1, m1_1, 0x55);
	t2 = _mm_mul_ps(m2_1, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_1, m1_1, 0xAA);
	t2 = _mm_mul_ps(m2_2, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_1, m1_1, 0xFF);
	t2 = _mm_mul_ps(m2_3, t1);
	R0 = _mm_add_ps(t2, R0);
	_mm_store_ps(&result[4], R0);

	__m128 m1_2 = _mm_load_ps(&mat1[8]);
	t1 = _mm_shuffle_ps(m1_2, m1_2, 0x0);
	R0 = _mm_mul_ps(m2_0, t1);
	t1 = _mm_shuffle_ps(m1_2, m1_2, 0x55);
	t2 = _mm_mul_ps(m2_1, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_2, m1_2, 0xAA);
	t2 = _mm_mul_ps(m2_2, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_2, m1_2, 0xFF);
	t2 = _mm_mul_ps(m2_3, t1);
	R0 = _mm_add_ps(t2, R0);
	_mm_store_ps(&result[8], R0);

	__m128 m1_3 = _mm_load_ps(&mat1[12]);
	t1 = _mm_shuffle_ps(m1_3, m1_3, 0x0);
	R0 = _mm_mul_ps(m2_0, t1);
	t1 = _mm_shuffle_ps(m1_3, m1_3, 0x55);
	t2 = _mm_mul_ps(m2_1, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_3, m1_3, 0xAA);
	t2 = _mm_mul_ps(m2_2, t1);
	R0 = _mm_add_ps(t2, R0);
	t1 = _mm_shuffle_ps(m1_3, m1_3, 0xFF);
	t2 = _mm_mul_ps(m2_3, t1);
	R0 = _mm_add_ps(t2, R0);
	_mm_store_ps(&result[12], R0);
}
