
#include "BaseLib/Common.h"

#define SSE_ALIGN	__declspec(align(16))

BASELIB_API void SSE_MultMatrices4x4(float* result, const float* mat1, const float* mat2);
BASELIB_API void SSE_Intr_MultMatrices4x4(float* result, const float* mat1, const float* mat2);
