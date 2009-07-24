#include "StdAfx.h"
#include "ToolPanel.h"

using namespace EditorCommon;


namespace MapEditor
{

	ToolPanel::ToolPanel(FormDirector^ director)
	{
		InitializeComponent();
		_director = director;
	}

	void ToolPanel::SetPanel(System::Windows::Forms::UserControl^ panel)
	{
		Controls->Clear();
		_panel = panel;
		if(_panel != nullptr)
			Controls->Add(_panel);
	}

}
