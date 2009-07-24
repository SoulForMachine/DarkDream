#include "StdAfx.h"
#include "TerrainEditPanel.h"


namespace MapEditor
{

	TerrainEditPanel::TerrainEditPanel(EM_TerrainEdit::Parameters^ params)
	{
		InitializeComponent();
		_parameters = params;
	}

}
