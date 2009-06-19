
#ifndef _BASELIB_H_
#define _BASELIB_H_


#ifdef BASELIB_EXPORTS

#define BASELIB_API __declspec(dllexport)

#else

#define BASELIB_API __declspec(dllimport)

#include "GL/GLRenderer.h"
#include "Math/Math3d.h"
#include "Console.h"
#include "FileSys.h"
#include "FreeList.h"
#include "FreePool.h"
#include "FreeStackPool.h"
#include "Image.h"
#include "Input.h"
#include "List.h"
#include "Memory.h"
#include "PerfTimer.h"
#include "Rect.h"
#include "SortedList.h"
#include "Stack.h"
#include "StaticArray.h"
#include "SstemInfo.h"
#include "Timer.h"
#include "Types.h"
#include "Video.h"

#endif


#endif // _BASELIB_H_
