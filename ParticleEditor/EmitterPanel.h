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
		EmitterPanel(void);

		void SetParticleSystem(Engine::ParticleSystem* part_sys);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EmitterPanel();

	protected: 
	private: System::Windows::Forms::Button^  _btnAdd;
	private: System::Windows::Forms::Button^  _btnRemove;
	private: System::Windows::Forms::Button^  _btnEnableAll;
	private: System::Windows::Forms::GroupBox^  _groupEmitterProp;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  _cmbShader;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  _cmbEmitterType;
	private: System::Windows::Forms::TextBox^  _textName;

	private: System::Windows::Forms::Label^  label3;


	private: System::Windows::Forms::NumericUpDown^  _numEmitterLife;


	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  _numParticleLife;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  _checkLoop;
	private: System::Windows::Forms::NumericUpDown^  _numEmitterLifeStart;


	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::CheckBox^  _checkImplode;


	private: System::Windows::Forms::CheckBox^  _checkEmitFromEdge;
	private: System::Windows::Forms::Button^  _btnBrowseResource;



	private: System::Windows::Forms::TextBox^  _textResource;





	private: System::Windows::Forms::RadioButton^  _radioModel;

	private: System::Windows::Forms::RadioButton^  _radioTexture;
	private: System::Windows::Forms::ListBox^  _listEmitterProperties;




	private: System::Windows::Forms::CheckBox^  _checkRandomOrient;
	private: System::Windows::Forms::ListView^  _listEmitters;
	private: System::Windows::Forms::ColumnHeader^  _columnName;
	private: System::Windows::Forms::Button^  _btnUp;
	private: System::Windows::Forms::Button^  _btnDown;
	private: System::Windows::Forms::CheckBox^  _checkAnimTex;
	private: System::Windows::Forms::OpenFileDialog^  _openFileDialog;






	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		void UpdateControls();
		void RefreshEmitterList();
		String^ MakeEmitterName();

		Engine::ParticleSystem* _particleSys;
		Engine::ParticleSystem::Emitter* _selectedEmitter;

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
			this->_groupEmitterProp = (gcnew System::Windows::Forms::GroupBox());
			this->_checkRandomOrient = (gcnew System::Windows::Forms::CheckBox());
			this->_checkAnimTex = (gcnew System::Windows::Forms::CheckBox());
			this->_numParticleLife = (gcnew System::Windows::Forms::NumericUpDown());
			this->_listEmitterProperties = (gcnew System::Windows::Forms::ListBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->_btnBrowseResource = (gcnew System::Windows::Forms::Button());
			this->_textResource = (gcnew System::Windows::Forms::TextBox());
			this->_radioModel = (gcnew System::Windows::Forms::RadioButton());
			this->_radioTexture = (gcnew System::Windows::Forms::RadioButton());
			this->_checkEmitFromEdge = (gcnew System::Windows::Forms::CheckBox());
			this->_checkImplode = (gcnew System::Windows::Forms::CheckBox());
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
			this->_listEmitters = (gcnew System::Windows::Forms::ListView());
			this->_columnName = (gcnew System::Windows::Forms::ColumnHeader());
			this->_btnUp = (gcnew System::Windows::Forms::Button());
			this->_btnDown = (gcnew System::Windows::Forms::Button());
			this->_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_groupEmitterProp->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numParticleLife))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLifeStart))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLife))->BeginInit();
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
			this->_btnAdd->Click += gcnew System::EventHandler(this, &EmitterPanel::_btnAdd_Click);
			// 
			// _btnRemove
			// 
			this->_btnRemove->Location = System::Drawing::Point(93, 232);
			this->_btnRemove->Name = L"_btnRemove";
			this->_btnRemove->Size = System::Drawing::Size(75, 23);
			this->_btnRemove->TabIndex = 2;
			this->_btnRemove->Text = L"Remove";
			this->_btnRemove->UseVisualStyleBackColor = true;
			this->_btnRemove->Click += gcnew System::EventHandler(this, &EmitterPanel::_btnRemove_Click);
			// 
			// _btnEnableAll
			// 
			this->_btnEnableAll->Location = System::Drawing::Point(174, 232);
			this->_btnEnableAll->Name = L"_btnEnableAll";
			this->_btnEnableAll->Size = System::Drawing::Size(75, 23);
			this->_btnEnableAll->TabIndex = 3;
			this->_btnEnableAll->Text = L"Enable All";
			this->_btnEnableAll->UseVisualStyleBackColor = true;
			this->_btnEnableAll->Click += gcnew System::EventHandler(this, &EmitterPanel::_btnEnableAll_Click);
			// 
			// _groupEmitterProp
			// 
			this->_groupEmitterProp->Controls->Add(this->_checkRandomOrient);
			this->_groupEmitterProp->Controls->Add(this->_checkAnimTex);
			this->_groupEmitterProp->Controls->Add(this->_numParticleLife);
			this->_groupEmitterProp->Controls->Add(this->_listEmitterProperties);
			this->_groupEmitterProp->Controls->Add(this->label5);
			this->_groupEmitterProp->Controls->Add(this->_btnBrowseResource);
			this->_groupEmitterProp->Controls->Add(this->_textResource);
			this->_groupEmitterProp->Controls->Add(this->_radioModel);
			this->_groupEmitterProp->Controls->Add(this->_radioTexture);
			this->_groupEmitterProp->Controls->Add(this->_checkEmitFromEdge);
			this->_groupEmitterProp->Controls->Add(this->_checkImplode);
			this->_groupEmitterProp->Controls->Add(this->_numEmitterLifeStart);
			this->_groupEmitterProp->Controls->Add(this->label6);
			this->_groupEmitterProp->Controls->Add(this->_checkLoop);
			this->_groupEmitterProp->Controls->Add(this->_numEmitterLife);
			this->_groupEmitterProp->Controls->Add(this->label4);
			this->_groupEmitterProp->Controls->Add(this->_textName);
			this->_groupEmitterProp->Controls->Add(this->label3);
			this->_groupEmitterProp->Controls->Add(this->_cmbShader);
			this->_groupEmitterProp->Controls->Add(this->label2);
			this->_groupEmitterProp->Controls->Add(this->_cmbEmitterType);
			this->_groupEmitterProp->Controls->Add(this->label1);
			this->_groupEmitterProp->Location = System::Drawing::Point(12, 274);
			this->_groupEmitterProp->Name = L"_groupEmitterProp";
			this->_groupEmitterProp->Size = System::Drawing::Size(370, 504);
			this->_groupEmitterProp->TabIndex = 6;
			this->_groupEmitterProp->TabStop = false;
			this->_groupEmitterProp->Text = L"Emitter properties";
			// 
			// _checkRandomOrient
			// 
			this->_checkRandomOrient->AutoSize = true;
			this->_checkRandomOrient->Location = System::Drawing::Point(153, 223);
			this->_checkRandomOrient->Name = L"_checkRandomOrient";
			this->_checkRandomOrient->Size = System::Drawing::Size(151, 17);
			this->_checkRandomOrient->TabIndex = 2;
			this->_checkRandomOrient->Text = L"Particle random orientation";
			this->_checkRandomOrient->UseVisualStyleBackColor = true;
			this->_checkRandomOrient->Click += gcnew System::EventHandler(this, &EmitterPanel::_checkRandomOrient_Click);
			// 
			// _checkAnimTex
			// 
			this->_checkAnimTex->AutoSize = true;
			this->_checkAnimTex->Location = System::Drawing::Point(153, 188);
			this->_checkAnimTex->Name = L"_checkAnimTex";
			this->_checkAnimTex->Size = System::Drawing::Size(105, 17);
			this->_checkAnimTex->TabIndex = 15;
			this->_checkAnimTex->Text = L"Animated texture";
			this->_checkAnimTex->UseVisualStyleBackColor = true;
			this->_checkAnimTex->Click += gcnew System::EventHandler(this, &EmitterPanel::_checkAnimTex_Click);
			// 
			// _numParticleLife
			// 
			this->_numParticleLife->Location = System::Drawing::Point(83, 222);
			this->_numParticleLife->Name = L"_numParticleLife";
			this->_numParticleLife->Size = System::Drawing::Size(45, 20);
			this->_numParticleLife->TabIndex = 1;
			this->_numParticleLife->ValueChanged += gcnew System::EventHandler(this, &EmitterPanel::_numParticleLife_ValueChanged);
			// 
			// _listEmitterProperties
			// 
			this->_listEmitterProperties->FormattingEnabled = true;
			this->_listEmitterProperties->Location = System::Drawing::Point(19, 261);
			this->_listEmitterProperties->Name = L"_listEmitterProperties";
			this->_listEmitterProperties->Size = System::Drawing::Size(295, 225);
			this->_listEmitterProperties->TabIndex = 20;
			this->_listEmitterProperties->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterPanel::_listEmitterProperties_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(16, 224);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(61, 13);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Particle life:";
			// 
			// _btnBrowseResource
			// 
			this->_btnBrowseResource->Location = System::Drawing::Point(274, 102);
			this->_btnBrowseResource->Name = L"_btnBrowseResource";
			this->_btnBrowseResource->Size = System::Drawing::Size(62, 23);
			this->_btnBrowseResource->TabIndex = 8;
			this->_btnBrowseResource->Text = L"Browse";
			this->_btnBrowseResource->UseVisualStyleBackColor = true;
			this->_btnBrowseResource->Click += gcnew System::EventHandler(this, &EmitterPanel::_btnBrowseResource_Click);
			// 
			// _textResource
			// 
			this->_textResource->Location = System::Drawing::Point(19, 104);
			this->_textResource->Name = L"_textResource";
			this->_textResource->ReadOnly = true;
			this->_textResource->Size = System::Drawing::Size(249, 20);
			this->_textResource->TabIndex = 7;
			// 
			// _radioModel
			// 
			this->_radioModel->AutoSize = true;
			this->_radioModel->Location = System::Drawing::Point(98, 81);
			this->_radioModel->Name = L"_radioModel";
			this->_radioModel->Size = System::Drawing::Size(54, 17);
			this->_radioModel->TabIndex = 9;
			this->_radioModel->TabStop = true;
			this->_radioModel->Text = L"Model";
			this->_radioModel->UseVisualStyleBackColor = true;
			this->_radioModel->Click += gcnew System::EventHandler(this, &EmitterPanel::_radioModel_Click);
			// 
			// _radioTexture
			// 
			this->_radioTexture->AutoSize = true;
			this->_radioTexture->Location = System::Drawing::Point(19, 81);
			this->_radioTexture->Name = L"_radioTexture";
			this->_radioTexture->Size = System::Drawing::Size(61, 17);
			this->_radioTexture->TabIndex = 6;
			this->_radioTexture->TabStop = true;
			this->_radioTexture->Text = L"Texture";
			this->_radioTexture->UseVisualStyleBackColor = true;
			this->_radioTexture->Click += gcnew System::EventHandler(this, &EmitterPanel::_radioTexture_Click);
			// 
			// _checkEmitFromEdge
			// 
			this->_checkEmitFromEdge->AutoSize = true;
			this->_checkEmitFromEdge->Location = System::Drawing::Point(19, 188);
			this->_checkEmitFromEdge->Name = L"_checkEmitFromEdge";
			this->_checkEmitFromEdge->Size = System::Drawing::Size(96, 17);
			this->_checkEmitFromEdge->TabIndex = 14;
			this->_checkEmitFromEdge->Text = L"Emit from edge";
			this->_checkEmitFromEdge->UseVisualStyleBackColor = true;
			this->_checkEmitFromEdge->Click += gcnew System::EventHandler(this, &EmitterPanel::_checkEmitFromEdge_Click);
			// 
			// _checkImplode
			// 
			this->_checkImplode->AutoSize = true;
			this->_checkImplode->Location = System::Drawing::Point(19, 165);
			this->_checkImplode->Name = L"_checkImplode";
			this->_checkImplode->Size = System::Drawing::Size(63, 17);
			this->_checkImplode->TabIndex = 13;
			this->_checkImplode->Text = L"Implode";
			this->_checkImplode->UseVisualStyleBackColor = true;
			this->_checkImplode->Click += gcnew System::EventHandler(this, &EmitterPanel::_checkImplode_Click);
			// 
			// _numEmitterLifeStart
			// 
			this->_numEmitterLifeStart->Location = System::Drawing::Point(206, 144);
			this->_numEmitterLifeStart->Name = L"_numEmitterLifeStart";
			this->_numEmitterLifeStart->Size = System::Drawing::Size(51, 20);
			this->_numEmitterLifeStart->TabIndex = 17;
			this->_numEmitterLifeStart->ValueChanged += gcnew System::EventHandler(this, &EmitterPanel::_numEmitterLifeStart_ValueChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(150, 146);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(50, 13);
			this->label6->TabIndex = 16;
			this->label6->Text = L"Life start:";
			// 
			// _checkLoop
			// 
			this->_checkLoop->AutoSize = true;
			this->_checkLoop->Location = System::Drawing::Point(19, 142);
			this->_checkLoop->Name = L"_checkLoop";
			this->_checkLoop->Size = System::Drawing::Size(50, 17);
			this->_checkLoop->TabIndex = 12;
			this->_checkLoop->Text = L"Loop";
			this->_checkLoop->UseVisualStyleBackColor = true;
			this->_checkLoop->Click += gcnew System::EventHandler(this, &EmitterPanel::_checkLoop_Click);
			// 
			// _numEmitterLife
			// 
			this->_numEmitterLife->Location = System::Drawing::Point(305, 144);
			this->_numEmitterLife->Name = L"_numEmitterLife";
			this->_numEmitterLife->Size = System::Drawing::Size(48, 20);
			this->_numEmitterLife->TabIndex = 19;
			this->_numEmitterLife->ValueChanged += gcnew System::EventHandler(this, &EmitterPanel::_numEmitterLife_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(272, 146);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 18;
			this->label4->Text = L"Life:";
			// 
			// _textName
			// 
			this->_textName->Location = System::Drawing::Point(60, 27);
			this->_textName->Name = L"_textName";
			this->_textName->Size = System::Drawing::Size(174, 20);
			this->_textName->TabIndex = 1;
			this->_textName->TextChanged += gcnew System::EventHandler(this, &EmitterPanel::_textName_TextChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 30);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(38, 13);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Name:";
			// 
			// _cmbShader
			// 
			this->_cmbShader->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbShader->FormattingEnabled = true;
			this->_cmbShader->Location = System::Drawing::Point(244, 53);
			this->_cmbShader->Name = L"_cmbShader";
			this->_cmbShader->Size = System::Drawing::Size(92, 21);
			this->_cmbShader->TabIndex = 5;
			this->_cmbShader->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterPanel::_cmbShader_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(194, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(44, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Shader:";
			// 
			// _cmbEmitterType
			// 
			this->_cmbEmitterType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->_cmbEmitterType->FormattingEnabled = true;
			this->_cmbEmitterType->Location = System::Drawing::Point(60, 53);
			this->_cmbEmitterType->Name = L"_cmbEmitterType";
			this->_cmbEmitterType->Size = System::Drawing::Size(92, 21);
			this->_cmbEmitterType->TabIndex = 3;
			this->_cmbEmitterType->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterPanel::_cmbEmitterType_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 56);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Type:";
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
			this->_listEmitters->TabIndex = 0;
			this->_listEmitters->UseCompatibleStateImageBehavior = false;
			this->_listEmitters->View = System::Windows::Forms::View::Details;
			this->_listEmitters->ItemChecked += gcnew System::Windows::Forms::ItemCheckedEventHandler(this, &EmitterPanel::_listEmitters_ItemChecked);
			this->_listEmitters->SelectedIndexChanged += gcnew System::EventHandler(this, &EmitterPanel::_listEmitters_SelectedIndexChanged);
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
			this->_btnUp->TabIndex = 4;
			this->_btnUp->Text = L"Up";
			this->_btnUp->UseVisualStyleBackColor = true;
			this->_btnUp->Click += gcnew System::EventHandler(this, &EmitterPanel::_btnUp_Click);
			// 
			// _btnDown
			// 
			this->_btnDown->Location = System::Drawing::Point(337, 232);
			this->_btnDown->Name = L"_btnDown";
			this->_btnDown->Size = System::Drawing::Size(45, 23);
			this->_btnDown->TabIndex = 5;
			this->_btnDown->Text = L"Down";
			this->_btnDown->UseVisualStyleBackColor = true;
			this->_btnDown->Click += gcnew System::EventHandler(this, &EmitterPanel::_btnDown_Click);
			// 
			// _openFileDialog
			// 
			this->_openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &EmitterPanel::_openFileDialog_FileOk);
			// 
			// EmitterPanel
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoScrollMargin = System::Drawing::Size(20, 20);
			this->ClientSize = System::Drawing::Size(402, 821);
			this->Controls->Add(this->_btnDown);
			this->Controls->Add(this->_btnUp);
			this->Controls->Add(this->_listEmitters);
			this->Controls->Add(this->_groupEmitterProp);
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
			this->_groupEmitterProp->ResumeLayout(false);
			this->_groupEmitterProp->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numParticleLife))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLifeStart))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numEmitterLife))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void _listEmitters_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnAdd_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnRemove_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnEnableAll_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnUp_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnDown_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _textName_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _cmbEmitterType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _cmbShader_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _radioTexture_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _radioModel_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnBrowseResource_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkLoop_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkImplode_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkEmitFromEdge_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkAnimTex_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numEmitterLifeStart_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numEmitterLife_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _listEmitterProperties_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numParticleLife_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkRandomOrient_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _listEmitters_ItemChecked(System::Object^  sender, System::Windows::Forms::ItemCheckedEventArgs^  e);
	private: System::Void _openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
};
}
