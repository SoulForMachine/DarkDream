#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ParticleEditor {

	/// <summary>
	/// Summary for EmitterPanel
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class EmitterPanel : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		EmitterPanel(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EmitterPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckedListBox^  _listEmitters;
	protected: 
	private: System::Windows::Forms::Button^  _btnAdd;
	private: System::Windows::Forms::Button^  _btnRemove;
	private: System::Windows::Forms::Button^  _btnEnableAll;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  _cmbShader;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  _cmbEmitterType;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  _checkLoop;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::Label^  label6;


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
			this->_listEmitters = (gcnew System::Windows::Forms::CheckedListBox());
			this->_btnAdd = (gcnew System::Windows::Forms::Button());
			this->_btnRemove = (gcnew System::Windows::Forms::Button());
			this->_btnEnableAll = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->_cmbEmitterType = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_cmbShader = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->_checkLoop = (gcnew System::Windows::Forms::CheckBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			this->SuspendLayout();
			// 
			// _listEmitters
			// 
			this->_listEmitters->FormattingEnabled = true;
			this->_listEmitters->Location = System::Drawing::Point(12, 12);
			this->_listEmitters->Name = L"_listEmitters";
			this->_listEmitters->Size = System::Drawing::Size(375, 214);
			this->_listEmitters->TabIndex = 0;
			// 
			// _btnAdd
			// 
			this->_btnAdd->Location = System::Drawing::Point(12, 232);
			this->_btnAdd->Name = L"_btnAdd";
			this->_btnAdd->Size = System::Drawing::Size(75, 23);
			this->_btnAdd->TabIndex = 1;
			this->_btnAdd->Text = L"Add";
			this->_btnAdd->UseVisualStyleBackColor = true;
			// 
			// _btnRemove
			// 
			this->_btnRemove->Location = System::Drawing::Point(93, 232);
			this->_btnRemove->Name = L"_btnRemove";
			this->_btnRemove->Size = System::Drawing::Size(75, 23);
			this->_btnRemove->TabIndex = 2;
			this->_btnRemove->Text = L"Remove";
			this->_btnRemove->UseVisualStyleBackColor = true;
			// 
			// _btnEnableAll
			// 
			this->_btnEnableAll->Location = System::Drawing::Point(174, 232);
			this->_btnEnableAll->Name = L"_btnEnableAll";
			this->_btnEnableAll->Size = System::Drawing::Size(75, 23);
			this->_btnEnableAll->TabIndex = 3;
			this->_btnEnableAll->Text = L"Enable All";
			this->_btnEnableAll->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->numericUpDown3);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->_checkLoop);
			this->groupBox1->Controls->Add(this->numericUpDown1);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->_cmbShader);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->_cmbEmitterType);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(12, 274);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(375, 257);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Emitter properties";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(27, 80);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Type:";
			// 
			// _cmbEmitterType
			// 
			this->_cmbEmitterType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbEmitterType->FormattingEnabled = true;
			this->_cmbEmitterType->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"Point", L"Line", L"Rectangle", L"Circle", 
				L"Plane", L"Box", L"Sphere"});
			this->_cmbEmitterType->Location = System::Drawing::Point(67, 77);
			this->_cmbEmitterType->Name = L"_cmbEmitterType";
			this->_cmbEmitterType->Size = System::Drawing::Size(92, 21);
			this->_cmbEmitterType->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(201, 80);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(44, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Shader:";
			// 
			// _cmbShader
			// 
			this->_cmbShader->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbShader->FormattingEnabled = true;
			this->_cmbShader->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Blend", L"Additive"});
			this->_cmbShader->Location = System::Drawing::Point(251, 77);
			this->_cmbShader->Name = L"_cmbShader";
			this->_cmbShader->Size = System::Drawing::Size(92, 21);
			this->_cmbShader->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(27, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(38, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Name:";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(71, 40);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(174, 20);
			this->textBox1->TabIndex = 5;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->numericUpDown2);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Location = System::Drawing::Point(12, 537);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(375, 273);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Particle properties";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(262, 43);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Life:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(31, 50);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(27, 13);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Life:";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(295, 41);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(48, 20);
			this->numericUpDown1->TabIndex = 7;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(64, 48);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(45, 20);
			this->numericUpDown2->TabIndex = 1;
			// 
			// _checkLoop
			// 
			this->_checkLoop->AutoSize = true;
			this->_checkLoop->Location = System::Drawing::Point(30, 117);
			this->_checkLoop->Name = L"_checkLoop";
			this->_checkLoop->Size = System::Drawing::Size(50, 17);
			this->_checkLoop->TabIndex = 8;
			this->_checkLoop->Text = L"Loop";
			this->_checkLoop->UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(182, 129);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(50, 13);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Life start:";
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(238, 127);
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(51, 20);
			this->numericUpDown3->TabIndex = 10;
			// 
			// EmitterPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(399, 822);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->_btnEnableAll);
			this->Controls->Add(this->_btnRemove);
			this->Controls->Add(this->_btnAdd);
			this->Controls->Add(this->_listEmitters);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight));
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Name = L"EmitterPanel";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->ShowInTaskbar = false;
			this->TabText = L"Emitters";
			this->Text = L"Emitters";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
