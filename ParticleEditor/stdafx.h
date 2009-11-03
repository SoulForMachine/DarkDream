// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include <msclr\marshal.h>
#include "BaseLib/Math/math3d.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Console.h"
#include "BaseLib/Timer.h"

#undef MessageBox

#include "Engine/Engine.h"

#ifdef _DEBUG
	#pragma comment(lib, "../Debug/Engine.lib")
#else
	#pragma comment(lib, "../Release/Engine.lib")
#endif
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

extern Engine::EngineAPI* engineAPI;

#define APP_NAME	"Dark Dream Particle System Editor"

inline
System::String^ GetAppName()
{
	return APP_NAME;
}
