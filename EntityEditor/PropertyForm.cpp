#include "StdAfx.h"
#include "PropertyForm.h"

using namespace EditorCommon;


namespace EntityEditor
{

	PropertyForm::PropertyForm(FormDirector^ director)
	{
		InitializeComponent();
		_director = director;
	}

}
