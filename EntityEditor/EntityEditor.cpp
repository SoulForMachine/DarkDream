// EntityEditor.cpp : main project file.

#include "stdafx.h"
#include "MainForm.h"

using namespace Engine;
using namespace EntityEditor;

EngineAPI* engineAPI;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	__try
	{
		// initialize memory manager, console and file system
		if(!Memory::Init())
			return 1;

		if(!::Console::Init(128, 4096))
		{
			Memory::Deinit();
			return 1;
		}

		engineAPI = CreateEngineAPI();
		if(!engineAPI->fileSystem->Init())
		{
			::Console::Deinit();
			Memory::Deinit();
			return 1;
		}

		// Enabling Windows XP visual effects before any controls are created
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false); 

		// Create the main window and run it
		Application::Run(gcnew MainForm());

		engineAPI->fileSystem->Deinit();
		DestroyEngineAPI(engineAPI);
		::Console::Deinit();
		Memory::Deinit();
	}
	__except(RECORD_EXCEPTION_INFO(_t("EntityEditor Main")))
	{
	}

	return 0;
}
