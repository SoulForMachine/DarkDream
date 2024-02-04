
#ifdef EXTPTR
	#undef EXTPTR
#endif

#ifdef EXTFUNC_DEFINE
	#define EXTPTR
#else

#ifndef H_GLEXT_H
#define H_GLEXT_H

#include "glext.h"
#include "wglext.h"


#ifdef __cplusplus
	#define EXTPTR extern "C"
#else
	#define EXTPTR extern
#endif


#ifdef __cplusplus
extern "C"{
#endif

bool glextIsSupported(const char* ext_str);
void glextLoadAll();
void wglextLoadAll();

#ifdef __cplusplus
} // extern "C"
#endif


#endif // H_GLEXT_H

#endif // EXTFUNC_DEFINE

// include all headers with declarations of extension func pointers
//=====================================================================

#include "glext_ptrs.h"
#include "wglext_ptrs.h"

//=====================================================================

