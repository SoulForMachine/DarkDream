
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/gl.h>
#include <cstring>

#include "h_glext.h"
#define EXTFUNC_DEFINE
#include "h_glext.h"


#define INIT_FUNC_PTR(func)\
		if(!InitFuncPtr(func, #func)) return false


template <class _FUNCT>
inline
bool InitFuncPtr(_FUNCT& func_ptr, const char* func_name)
{
	func_ptr = (_FUNCT)wglGetProcAddress(func_name);
	return func_ptr != 0;
}

// include extension loading function definitons
//=====================================================================

#include "glext_ptrs.inc"
#include "wglext_ptrs.inc"

//=====================================================================


bool glextIsSupported(const char* extension)
{
	if(!extension || !extension[0])
		return false;

	const char* ext_string = (const char*)glGetString(GL_EXTENSIONS);

	if(ext_string && strstr(ext_string, extension))
		return true;

	glextLoad_WGL_ARB_extensions_string();
	if(wglGetExtensionsStringARB)
	{
		ext_string = (const char*)wglGetExtensionsStringARB(wglGetCurrentDC());
		if(ext_string && strstr(ext_string, extension))
			return true;
	}

	return false;
}

// glextLoadAll() definition
//=====================================================================

#include "glext_load.inc"
#include "wglext_load.inc"

//=====================================================================

