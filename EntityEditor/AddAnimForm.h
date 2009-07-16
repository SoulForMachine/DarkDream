#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Windows;
using namespace System::Data;
using namespace System::Drawing;


namespace EntityEditor {

	/// <summary>
	/// Summary for AddAnimForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AddAnimForm : public System::Windows::Forms::Form
	{
	public:
		AddAnimForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		property String^ FileName
		{
			String^ get() { return _fileName; }
		}

		property String^ AnimName
		{
			String^ get() { return _animName; }
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AddAnimForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		String^ _fileName;
		String^ _animName;

	private: System::Windows::Forms::Button^  _btnOK;
	private: System::Windows::Forms::Button^  _btnCancel;
	protected: 

	protected: 

	private: System::Windows::Forms::ComboBox^  _cmbStandardAnim;
	private: System::Windows::Forms::TextBox^  _textAnimationName;




	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  _textAnimationFile;
	private: System::Windows::Forms::Button^  _btnBrowseFile;



	private: System::Windows::Forms::RadioButton^  _radioCustomAnim;

	private: System::Windows::Forms::RadioButton^  _radioStandardAnim;

	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::OpenFileDialog^  _openFileDialog;

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
			this->_btnOK = (gcnew System::Windows::Forms::Button());
			this->_btnCancel = (gcnew System::Windows::Forms::Button());
			this->_cmbStandardAnim = (gcnew System::Windows::Forms::ComboBox());
			this->_textAnimationName = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_radioCustomAnim = (gcnew System::Windows::Forms::RadioButton());
			this->_radioStandardAnim = (gcnew System::Windows::Forms::RadioButton());
			this->_textAnimationFile = (gcnew System::Windows::Forms::TextBox());
			this->_btnBrowseFile = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// _btnOK
			// 
			this->_btnOK->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->_btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->_btnOK->Location = System::Drawing::Point(270, 276);
			this->_btnOK->Name = L"_btnOK";
			this->_btnOK->Size = System::Drawing::Size(75, 23);
			this->_btnOK->TabIndex = 2;
			this->_btnOK->Text = L"OK";
			this->_btnOK->UseVisualStyleBackColor = true;
			this->_btnOK->Click += gcnew System::EventHandler(this, &AddAnimForm::_btnOK_Click);
			// 
			// _btnCancel
			// 
			this->_btnCancel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->_btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->_btnCancel->Location = System::Drawing::Point(351, 276);
			this->_btnCancel->Name = L"_btnCancel";
			this->_btnCancel->Size = System::Drawing::Size(75, 23);
			this->_btnCancel->TabIndex = 3;
			this->_btnCancel->Text = L"Cancel";
			this->_btnCancel->UseVisualStyleBackColor = true;
			// 
			// _cmbStandardAnim
			// 
			this->_cmbStandardAnim->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_cmbStandardAnim->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbStandardAnim->FormattingEnabled = true;
			this->_cmbStandardAnim->Items->AddRange(gcnew cli::array< System::Object^  >(9) {L"Idle", L"Walk", L"Run", L"Pain", L"Death1", 
				L"Death2", L"Death3", L"Attack", L"Melee attack"});
			this->_cmbStandardAnim->Location = System::Drawing::Point(132, 35);
			this->_cmbStandardAnim->Name = L"_cmbStandardAnim";
			this->_cmbStandardAnim->Size = System::Drawing::Size(249, 21);
			this->_cmbStandardAnim->TabIndex = 1;
			// 
			// _textAnimationName
			// 
			this->_textAnimationName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textAnimationName->Location = System::Drawing::Point(132, 87);
			this->_textAnimationName->Name = L"_textAnimationName";
			this->_textAnimationName->Size = System::Drawing::Size(249, 20);
			this->_textAnimationName->TabIndex = 3;
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox1->Controls->Add(this->_radioCustomAnim);
			this->groupBox1->Controls->Add(this->_radioStandardAnim);
			this->groupBox1->Controls->Add(this->_textAnimationName);
			this->groupBox1->Controls->Add(this->_cmbStandardAnim);
			this->groupBox1->Location = System::Drawing::Point(12, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(414, 142);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Animation name";
			// 
			// _radioCustomAnim
			// 
			this->_radioCustomAnim->AutoSize = true;
			this->_radioCustomAnim->Location = System::Drawing::Point(31, 88);
			this->_radioCustomAnim->Name = L"_radioCustomAnim";
			this->_radioCustomAnim->Size = System::Drawing::Size(60, 17);
			this->_radioCustomAnim->TabIndex = 2;
			this->_radioCustomAnim->TabStop = true;
			this->_radioCustomAnim->Text = L"Custom";
			this->_radioCustomAnim->UseVisualStyleBackColor = true;
			this->_radioCustomAnim->Click += gcnew System::EventHandler(this, &AddAnimForm::_radioCustomAnim_Click);
			// 
			// _radioStandardAnim
			// 
			this->_radioStandardAnim->AutoSize = true;
			this->_radioStandardAnim->Location = System::Drawing::Point(31, 36);
			this->_radioStandardAnim->Name = L"_radioStandardAnim";
			this->_radioStandardAnim->Size = System::Drawing::Size(68, 17);
			this->_radioStandardAnim->TabIndex = 0;
			this->_radioStandardAnim->TabStop = true;
			this->_radioStandardAnim->Text = L"Standard";
			this->_radioStandardAnim->UseVisualStyleBackColor = true;
			this->_radioStandardAnim->CheckedChanged += gcnew System::EventHandler(this, &AddAnimForm::_radioStandardAnim_CheckedChanged);
			// 
			// _textAnimationFile
			// 
			this->_textAnimationFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textAnimationFile->Location = System::Drawing::Point(18, 35);
			this->_textAnimationFile->Name = L"_textAnimationFile";
			this->_textAnimationFile->ReadOnly = true;
			this->_textAnimationFile->Size = System::Drawing::Size(300, 20);
			this->_textAnimationFile->TabIndex = 0;
			// 
			// _btnBrowseFile
			// 
			this->_btnBrowseFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseFile->Location = System::Drawing::Point(324, 33);
			this->_btnBrowseFile->Name = L"_btnBrowseFile";
			this->_btnBrowseFile->Size = System::Drawing::Size(75, 23);
			this->_btnBrowseFile->TabIndex = 1;
			this->_btnBrowseFile->Text = L"Browse";
			this->_btnBrowseFile->UseVisualStyleBackColor = true;
			this->_btnBrowseFile->Click += gcnew System::EventHandler(this, &AddAnimForm::_btnBrowseFile_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox2->Controls->Add(this->_textAnimationFile);
			this->groupBox2->Controls->Add(this->_btnBrowseFile);
			this->groupBox2->Location = System::Drawing::Point(12, 169);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(414, 83);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Animation file";
			// 
			// _openFileDialog
			// 
			this->_openFileDialog->DefaultExt = L"anima";
			this->_openFileDialog->Filter = L"Animation Files (*.anima)|*.anima|All Files (*.*)|*.*";
			this->_openFileDialog->Title = L"Select Animation";
			this->_openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &AddAnimForm::_openFileDialog_FileOk);
			// 
			// AddAnimForm
			// 
			this->AcceptButton = this->_btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->_btnCancel;
			this->ClientSize = System::Drawing::Size(438, 311);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->_btnCancel);
			this->Controls->Add(this->_btnOK);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"AddAnimForm";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Add Animation";
			this->Load += gcnew System::EventHandler(this, &AddAnimForm::AddAnimForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	System::Void AddAnimForm_Load(System::Object^  sender, System::EventArgs^  e);
	System::Void _radioStandardAnim_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnBrowseFile_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnOK_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _radioCustomAnim_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
};
}
