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

	protected: 
	private: System::Windows::Forms::Button^  _btnAdd;
	private: System::Windows::Forms::Button^  _btnRemove;
	private: System::Windows::Forms::Button^  _btnEnableAll;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  _cmbShader;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  _cmbEmitterType;
	private: System::Windows::Forms::TextBox^  _textName;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::NumericUpDown^  _numEmitterLife;


	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  _numParticleLife;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  _checkLoop;
	private: System::Windows::Forms::NumericUpDown^  _numEmitterLifeStart;


	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::CheckBox^  _checkImplode;


	private: System::Windows::Forms::CheckBox^  _checkEmitFromEdge;
	private: System::Windows::Forms::Button^  _btnBrowseModel;

	private: System::Windows::Forms::Button^  _btnBrowseTexture;

	private: System::Windows::Forms::TextBox^  _textModel;

	private: System::Windows::Forms::TextBox^  _textTexture;

	private: System::Windows::Forms::RadioButton^  _radioModel;

	private: System::Windows::Forms::RadioButton^  _radioTexture;
	private: System::Windows::Forms::ListBox^  _listEmitterProperties;
	private: System::Windows::Forms::ListBox^  _listParticleProperties;



	private: System::Windows::Forms::CheckBox^  _checkRandomOrient;
	private: System::Windows::Forms::ListView^  _listEmitters;
	private: System::Windows::Forms::ColumnHeader^  _columnName;
	private: System::Windows::Forms::Button^  _btnUp;
	private: System::Windows::Forms::Button^  _btnDown;






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
			this->_btnAdd = (gcnew System::Windows::Forms::Button());
			this->_btnRemove = (gcnew System::Windows::Forms::Button());
			this->_btnEnableAll = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_numEmitterLifeStart = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->_checkLoop = (gcnew System::Windows::Forms::CheckBox());
			this->_numEmitterLife = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->_textName = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->_cmbShader = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_cmbEmitterType = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->_numParticleLife = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->_checkImplode = (gcnew System::Windows::Forms::CheckBox());
			this->_checkEmitFromEdge = (gcnew System::Windows::Forms::CheckBox());
			this->_radioTexture = (gcnew System::Windows::Forms::RadioButton());
			this->_radioModel = (gcnew System::Windows::Forms::RadioButton());
			this->_textTexture = (gcnew System::Windows::Forms::TextBox());
			this->_textModel = (gcnew System::Windows::Forms::TextBox());
			this->_btnBrowseTexture = (gcnew System::Windows::Forms::Button());
			this->_btnBrowseModel = (gcnew System::Windows::Forms::Button());
			this->_listEmitterProperties = (gcnew System::Windows::Forms::ListBox());
			this->_checkRandomOrient = (gcnew System::Windows::Forms::CheckBox());
			this->_listParticleProperties = (gcnew System::Windows::Forms::ListBox());
			this->_listEmitters = (gcnew System::Windows::Forms::ListView());
			this->_columnName = (gcnew System::Windows::Forms::ColumnHeader());
			this->_btnUp = (gcnew System::Windows::Forms::Button());
			this->_btnDown = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLifeStart))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLife))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numParticleLife))->BeginInit();
			this->SuspendLayout();
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
			this->groupBox1->Controls->Add(this->_listEmitterProperties);
			this->groupBox1->Controls->Add(this->_btnBrowseModel);
			this->groupBox1->Controls->Add(this->_btnBrowseTexture);
			this->groupBox1->Controls->Add(this->_textModel);
			this->groupBox1->Controls->Add(this->_textTexture);
			this->groupBox1->Controls->Add(this->_radioModel);
			this->groupBox1->Controls->Add(this->_radioTexture);
			this->groupBox1->Controls->Add(this->_checkEmitFromEdge);
			this->groupBox1->Controls->Add(this->_checkImplode);
			this->groupBox1->Controls->Add(this->_numEmitterLifeStart);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->_checkLoop);
			this->groupBox1->Controls->Add(this->_numEmitterLife);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->_textName);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->_cmbShader);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->_cmbEmitterType);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(12, 274);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(370, 363);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Emitter properties";
			// 
			// _numEmitterLifeStart
			// 
			this->_numEmitterLifeStart->Location = System::Drawing::Point(206, 144);
			this->_numEmitterLifeStart->Name = L"_numEmitterLifeStart";
			this->_numEmitterLifeStart->Size = System::Drawing::Size(51, 20);
			this->_numEmitterLifeStart->TabIndex = 10;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(150, 146);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(50, 13);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Life start:";
			// 
			// _checkLoop
			// 
			this->_checkLoop->AutoSize = true;
			this->_checkLoop->Location = System::Drawing::Point(19, 142);
			this->_checkLoop->Name = L"_checkLoop";
			this->_checkLoop->Size = System::Drawing::Size(50, 17);
			this->_checkLoop->TabIndex = 8;
			this->_checkLoop->Text = L"Loop";
			this->_checkLoop->UseVisualStyleBackColor = true;
			// 
			// _numEmitterLife
			// 
			this->_numEmitterLife->Location = System::Drawing::Point(305, 144);
			this->_numEmitterLife->Name = L"_numEmitterLife";
			this->_numEmitterLife->Size = System::Drawing::Size(48, 20);
			this->_numEmitterLife->TabIndex = 7;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(272, 146);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Life:";
			// 
			// _textName
			// 
			this->_textName->Location = System::Drawing::Point(60, 27);
			this->_textName->Name = L"_textName";
			this->_textName->Size = System::Drawing::Size(174, 20);
			this->_textName->TabIndex = 5;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 30);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(38, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Name:";
			// 
			// _cmbShader
			// 
			this->_cmbShader->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbShader->FormattingEnabled = true;
			this->_cmbShader->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Blend", L"Additive"});
			this->_cmbShader->Location = System::Drawing::Point(244, 53);
			this->_cmbShader->Name = L"_cmbShader";
			this->_cmbShader->Size = System::Drawing::Size(92, 21);
			this->_cmbShader->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(194, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(44, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Shader:";
			// 
			// _cmbEmitterType
			// 
			this->_cmbEmitterType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbEmitterType->FormattingEnabled = true;
			this->_cmbEmitterType->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"Sphere", L"Plane", L"Circle", L"Line"});
			this->_cmbEmitterType->Location = System::Drawing::Point(60, 53);
			this->_cmbEmitterType->Name = L"_cmbEmitterType";
			this->_cmbEmitterType->Size = System::Drawing::Size(92, 21);
			this->_cmbEmitterType->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 56);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Type:";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->_listParticleProperties);
			this->groupBox2->Controls->Add(this->_checkRandomOrient);
			this->groupBox2->Controls->Add(this->_numParticleLife);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Location = System::Drawing::Point(12, 643);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(370, 197);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Particle properties";
			// 
			// _numParticleLife
			// 
			this->_numParticleLife->Location = System::Drawing::Point(49, 34);
			this->_numParticleLife->Name = L"_numParticleLife";
			this->_numParticleLife->Size = System::Drawing::Size(45, 20);
			this->_numParticleLife->TabIndex = 1;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(16, 36);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(27, 13);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Life:";
			// 
			// _checkImplode
			// 
			this->_checkImplode->AutoSize = true;
			this->_checkImplode->Location = System::Drawing::Point(19, 165);
			this->_checkImplode->Name = L"_checkImplode";
			this->_checkImplode->Size = System::Drawing::Size(63, 17);
			this->_checkImplode->TabIndex = 11;
			this->_checkImplode->Text = L"Implode";
			this->_checkImplode->UseVisualStyleBackColor = true;
			// 
			// _checkEmitFromEdge
			// 
			this->_checkEmitFromEdge->AutoSize = true;
			this->_checkEmitFromEdge->Location = System::Drawing::Point(19, 188);
			this->_checkEmitFromEdge->Name = L"_checkEmitFromEdge";
			this->_checkEmitFromEdge->Size = System::Drawing::Size(96, 17);
			this->_checkEmitFromEdge->TabIndex = 12;
			this->_checkEmitFromEdge->Text = L"Emit from edge";
			this->_checkEmitFromEdge->UseVisualStyleBackColor = true;
			// 
			// _radioTexture
			// 
			this->_radioTexture->AutoSize = true;
			this->_radioTexture->Location = System::Drawing::Point(19, 81);
			this->_radioTexture->Name = L"_radioTexture";
			this->_radioTexture->Size = System::Drawing::Size(61, 17);
			this->_radioTexture->TabIndex = 13;
			this->_radioTexture->TabStop = true;
			this->_radioTexture->Text = L"Texture";
			this->_radioTexture->UseVisualStyleBackColor = true;
			// 
			// _radioModel
			// 
			this->_radioModel->AutoSize = true;
			this->_radioModel->Location = System::Drawing::Point(19, 110);
			this->_radioModel->Name = L"_radioModel";
			this->_radioModel->Size = System::Drawing::Size(54, 17);
			this->_radioModel->TabIndex = 14;
			this->_radioModel->TabStop = true;
			this->_radioModel->Text = L"Model";
			this->_radioModel->UseVisualStyleBackColor = true;
			// 
			// _textTexture
			// 
			this->_textTexture->Location = System::Drawing::Point(89, 80);
			this->_textTexture->Name = L"_textTexture";
			this->_textTexture->ReadOnly = true;
			this->_textTexture->Size = System::Drawing::Size(196, 20);
			this->_textTexture->TabIndex = 15;
			// 
			// _textModel
			// 
			this->_textModel->Location = System::Drawing::Point(89, 109);
			this->_textModel->Name = L"_textModel";
			this->_textModel->ReadOnly = true;
			this->_textModel->Size = System::Drawing::Size(196, 20);
			this->_textModel->TabIndex = 16;
			// 
			// _btnBrowseTexture
			// 
			this->_btnBrowseTexture->Location = System::Drawing::Point(291, 78);
			this->_btnBrowseTexture->Name = L"_btnBrowseTexture";
			this->_btnBrowseTexture->Size = System::Drawing::Size(62, 23);
			this->_btnBrowseTexture->TabIndex = 17;
			this->_btnBrowseTexture->Text = L"Browse";
			this->_btnBrowseTexture->UseVisualStyleBackColor = true;
			// 
			// _btnBrowseModel
			// 
			this->_btnBrowseModel->Location = System::Drawing::Point(291, 107);
			this->_btnBrowseModel->Name = L"_btnBrowseModel";
			this->_btnBrowseModel->Size = System::Drawing::Size(62, 23);
			this->_btnBrowseModel->TabIndex = 18;
			this->_btnBrowseModel->Text = L"Browse";
			this->_btnBrowseModel->UseVisualStyleBackColor = true;
			// 
			// _listEmitterProperties
			// 
			this->_listEmitterProperties->FormattingEnabled = true;
			this->_listEmitterProperties->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"Velocity", L"Emitter Size", L"Emit Angle", 
				L"Emit Rate", L"X Offset", L"Y Offset", L"Z Offset", L"X Rotation", L"Y Rotation", L"Z Rotation"});
			this->_listEmitterProperties->Location = System::Drawing::Point(19, 211);
			this->_listEmitterProperties->Name = L"_listEmitterProperties";
			this->_listEmitterProperties->Size = System::Drawing::Size(295, 134);
			this->_listEmitterProperties->TabIndex = 19;
			// 
			// _checkRandomOrient
			// 
			this->_checkRandomOrient->AutoSize = true;
			this->_checkRandomOrient->Location = System::Drawing::Point(153, 35);
			this->_checkRandomOrient->Name = L"_checkRandomOrient";
			this->_checkRandomOrient->Size = System::Drawing::Size(120, 17);
			this->_checkRandomOrient->TabIndex = 2;
			this->_checkRandomOrient->Text = L"Random Orientation";
			this->_checkRandomOrient->UseVisualStyleBackColor = true;
			// 
			// _listParticleProperties
			// 
			this->_listParticleProperties->FormattingEnabled = true;
			this->_listParticleProperties->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"Size", L"Rotation Speed", L"Alpha", 
				L"Gravity", L"Friction"});
			this->_listParticleProperties->Location = System::Drawing::Point(19, 60);
			this->_listParticleProperties->Name = L"_listParticleProperties";
			this->_listParticleProperties->Size = System::Drawing::Size(295, 108);
			this->_listParticleProperties->TabIndex = 3;
			// 
			// _listEmitters
			// 
			this->_listEmitters->CheckBoxes = true;
			this->_listEmitters->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->_columnName});
			this->_listEmitters->FullRowSelect = true;
			this->_listEmitters->GridLines = true;
			this->_listEmitters->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::None;
			this->_listEmitters->Location = System::Drawing::Point(12, 12);
			this->_listEmitters->MultiSelect = false;
			this->_listEmitters->Name = L"_listEmitters";
			this->_listEmitters->Size = System::Drawing::Size(370, 214);
			this->_listEmitters->TabIndex = 6;
			this->_listEmitters->UseCompatibleStateImageBehavior = false;
			this->_listEmitters->View = System::Windows::Forms::View::Details;
			// 
			// _columnName
			// 
			this->_columnName->Text = L"Name";
			this->_columnName->Width = 300;
			// 
			// _btnUp
			// 
			this->_btnUp->Location = System::Drawing::Point(286, 232);
			this->_btnUp->Name = L"_btnUp";
			this->_btnUp->Size = System::Drawing::Size(45, 23);
			this->_btnUp->TabIndex = 7;
			this->_btnUp->Text = L"Up";
			this->_btnUp->UseVisualStyleBackColor = true;
			// 
			// _btnDown
			// 
			this->_btnDown->Location = System::Drawing::Point(337, 232);
			this->_btnDown->Name = L"_btnDown";
			this->_btnDown->Size = System::Drawing::Size(45, 23);
			this->_btnDown->TabIndex = 8;
			this->_btnDown->Text = L"Down";
			this->_btnDown->UseVisualStyleBackColor = true;
			// 
			// EmitterPanel
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoScrollMargin = System::Drawing::Size(20, 20);
			this->ClientSize = System::Drawing::Size(402, 875);
			this->Controls->Add(this->_btnDown);
			this->Controls->Add(this->_btnUp);
			this->Controls->Add(this->_listEmitters);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->_btnEnableAll);
			this->Controls->Add(this->_btnRemove);
			this->Controls->Add(this->_btnAdd);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Name = L"EmitterPanel";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->ShowInTaskbar = false;
			this->TabText = L"Emitters";
			this->Text = L"Emitters";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLifeStart))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLife))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numParticleLife))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
