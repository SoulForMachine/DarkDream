#pragma once

#include "Entity.h"


namespace EntityEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for PropertyForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PropertyForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		PropertyForm(EditorCommon::FormDirector^ director);

		void SetProperties(EntityProperties^ props)
			{ _propertyGrid->SelectedObject = props; }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PropertyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PropertyGrid^  _propertyGrid;
	protected: 

	private:
		EditorCommon::FormDirector^ _director;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->_propertyGrid = (gcnew System::Windows::Forms::PropertyGrid());
			this->SuspendLayout();
			// 
			// _propertyGrid
			// 
			this->_propertyGrid->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_propertyGrid->Location = System::Drawing::Point(0, 0);
			this->_propertyGrid->Name = L"_propertyGrid";
			this->_propertyGrid->Size = System::Drawing::Size(292, 572);
			this->_propertyGrid->TabIndex = 0;
			// 
			// PropertyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 572);
			this->Controls->Add(this->_propertyGrid);
			this->HideOnClose = true;
			this->Name = L"PropertyForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->ShowInTaskbar = false;
			this->TabText = L"Properties";
			this->Text = L"Properties";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
