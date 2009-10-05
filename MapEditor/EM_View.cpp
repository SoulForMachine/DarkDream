
#include "stdafx.h"
#include "ViewPanel.h"
#include "EM_View.h"


namespace MapEditor
{

	EM_View::EM_View(EditModeEventListener^ listener, bool persistent)
		: EditMode(listener, persistent)
	{
		_parameters = gcnew Parameters;
		_parameters->camHeight = 5.0f;
		_parameters->camDistance = 5.0f;
		_parameters->camFOV = 60.0f;

		_panel = gcnew ViewPanel(_parameters);
	}

	System::Windows::Forms::UserControl^ EM_View::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_View::GetModeEnum()
	{
		return EditModeEnum::VIEW;
	}

}
