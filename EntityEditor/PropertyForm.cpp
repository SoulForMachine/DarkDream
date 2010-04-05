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

	System::Void PropertyForm::_propertyGrid_PropertyValueChanged(System::Object^  s, System::Windows::Forms::PropertyValueChangedEventArgs^  e)
	{
		if(!e->ChangedItem->Value->Equals(e->OldValue))
		{
			if(e->ChangedItem->Value->GetType() == EntityProperties::EntityClass::typeid)
			{
				_director->FormNotify(this, NotifyMessage::ClassPropertyChanged, e->ChangedItem->Value);
			}
			else
			{
				_director->FormNotify(this, NotifyMessage::PropertyChanged, e->ChangedItem->Value);
			}
		}
	}

}
