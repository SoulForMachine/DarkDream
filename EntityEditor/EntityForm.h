#pragma once


namespace EntityEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;


	/// <summary>
	/// Summary for EntityForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class EntityForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		EntityForm(EditorCommon::FormDirector^ director);

		void SetEntity(Engine::ModelEntity* entity)
		{
			_entity = entity;
			UpdateControls();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EntityForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void InsertJoint(System::Windows::Forms::TreeNodeCollection^ nodes, const Engine::Joint* joint);
		void UpdateControls();

		Engine::ModelEntity* _entity;
		EditorCommon::FormDirector^ _director;
		literal int JOINT_IMAGE_INDEX = 6;
		literal int JOINT_ATT_IMAGE_INDEX = 7;
		literal int JOINT_NONE_IMAGE_INDEX = 1;

	protected: 

	private: System::Windows::Forms::OpenFileDialog^  _openFileDialog;

	private: System::Windows::Forms::MenuStrip^  _menuEntity;

	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewExpandAll;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewCollapseAll;
	private: System::Windows::Forms::Panel^  _panelScroll;

	private: Salamander::Windows::Forms::CollapsiblePanelBar^  _panelEntity;



	private: Salamander::Windows::Forms::CollapsiblePanel^  _panelSounds;
	private: System::Windows::Forms::ListView^  _listSounds;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::Button^  _btnAddSound;
	private: System::Windows::Forms::Button^  _btnRemoveSound;
	private: System::Windows::Forms::Button^  _btnPlaySound;
	private: Salamander::Windows::Forms::CollapsiblePanel^  _panelAnimations;
	private: System::Windows::Forms::ListView^  _listAnimations;
	private: System::Windows::Forms::ColumnHeader^  _colHdrName;
	private: System::Windows::Forms::ColumnHeader^  _colHdrAnimation;
	private: System::Windows::Forms::Button^  _btnAddAnim;
	private: System::Windows::Forms::Button^  _btnRemoveAnim;
	private: System::Windows::Forms::Button^  _btnPlayAnim;
	private: Salamander::Windows::Forms::CollapsiblePanel^  _panelSkeleton;
	private: System::Windows::Forms::Button^  _btnRemoveAttachment;
	private: System::Windows::Forms::Button^  _btnBrowseAttachment;
	private: System::Windows::Forms::TextBox^  _textJointAttachment;
	private: System::Windows::Forms::TreeView^  _treeSkelet;
	private: System::Windows::Forms::Label^  label4;
	private: Salamander::Windows::Forms::CollapsiblePanel^  _panelModel;
	private: System::Windows::Forms::TextBox^  _textModel;
	private: System::Windows::Forms::Button^  _btnBrowseModel;
	private: System::Windows::Forms::ToolStrip^  _mainToolbar;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnExpandAll;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnCollapseAll;
	private: System::Windows::Forms::ImageList^  _imageList;
	private: System::Windows::Forms::Button^  _btnCollapseAllSkelet;
	private: System::Windows::Forms::Button^  _btnExpandAllSkelet;
	private: System::Windows::Forms::Button^  _btnTPose;


	protected: 

	private: System::ComponentModel::IContainer^  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(EntityForm::typeid));
			this->_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_menuEntity = (gcnew System::Windows::Forms::MenuStrip());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewExpandAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewCollapseAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_panelScroll = (gcnew System::Windows::Forms::Panel());
			this->_panelEntity = (gcnew Salamander::Windows::Forms::CollapsiblePanelBar());
			this->_imageList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->_panelSounds = (gcnew Salamander::Windows::Forms::CollapsiblePanel());
			this->_listSounds = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->_btnAddSound = (gcnew System::Windows::Forms::Button());
			this->_btnRemoveSound = (gcnew System::Windows::Forms::Button());
			this->_btnPlaySound = (gcnew System::Windows::Forms::Button());
			this->_panelAnimations = (gcnew Salamander::Windows::Forms::CollapsiblePanel());
			this->_listAnimations = (gcnew System::Windows::Forms::ListView());
			this->_colHdrName = (gcnew System::Windows::Forms::ColumnHeader());
			this->_colHdrAnimation = (gcnew System::Windows::Forms::ColumnHeader());
			this->_btnAddAnim = (gcnew System::Windows::Forms::Button());
			this->_btnRemoveAnim = (gcnew System::Windows::Forms::Button());
			this->_btnPlayAnim = (gcnew System::Windows::Forms::Button());
			this->_btnTPose = (gcnew System::Windows::Forms::Button());
			this->_panelSkeleton = (gcnew Salamander::Windows::Forms::CollapsiblePanel());
			this->_btnCollapseAllSkelet = (gcnew System::Windows::Forms::Button());
			this->_btnExpandAllSkelet = (gcnew System::Windows::Forms::Button());
			this->_treeSkelet = (gcnew System::Windows::Forms::TreeView());
			this->_btnRemoveAttachment = (gcnew System::Windows::Forms::Button());
			this->_btnBrowseAttachment = (gcnew System::Windows::Forms::Button());
			this->_textJointAttachment = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->_panelModel = (gcnew Salamander::Windows::Forms::CollapsiblePanel());
			this->_textModel = (gcnew System::Windows::Forms::TextBox());
			this->_btnBrowseModel = (gcnew System::Windows::Forms::Button());
			this->_mainToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->_toolBtnExpandAll = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnCollapseAll = (gcnew System::Windows::Forms::ToolStripButton());
			this->_menuEntity->SuspendLayout();
			this->_panelScroll->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_panelEntity))->BeginInit();
			this->_panelEntity->SuspendLayout();
			this->_panelSounds->SuspendLayout();
			this->_panelAnimations->SuspendLayout();
			this->_panelSkeleton->SuspendLayout();
			this->_panelModel->SuspendLayout();
			this->_mainToolbar->SuspendLayout();
			this->SuspendLayout();
			// 
			// _openFileDialog
			// 
			this->_openFileDialog->Filter = L"All Files (*.*)|*.*";
			this->_openFileDialog->Title = L"Open Resource";
			this->_openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &EntityForm::_openFileDialog_FileOk);
			// 
			// _menuEntity
			// 
			this->_menuEntity->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->viewToolStripMenuItem});
			this->_menuEntity->Location = System::Drawing::Point(0, 0);
			this->_menuEntity->Name = L"_menuEntity";
			this->_menuEntity->Size = System::Drawing::Size(502, 24);
			this->_menuEntity->TabIndex = 2;
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_mnuViewExpandAll, 
				this->_mnuViewCollapseAll});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->viewToolStripMenuItem->Text = L"&View";
			// 
			// _mnuViewExpandAll
			// 
			this->_mnuViewExpandAll->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuViewExpandAll.Image")));
			this->_mnuViewExpandAll->ImageTransparentColor = System::Drawing::Color::White;
			this->_mnuViewExpandAll->Name = L"_mnuViewExpandAll";
			this->_mnuViewExpandAll->Size = System::Drawing::Size(139, 22);
			this->_mnuViewExpandAll->Text = L"&Expand All";
			this->_mnuViewExpandAll->Click += gcnew System::EventHandler(this, &EntityForm::_mnuViewExpandAll_Click);
			// 
			// _mnuViewCollapseAll
			// 
			this->_mnuViewCollapseAll->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuViewCollapseAll.Image")));
			this->_mnuViewCollapseAll->ImageTransparentColor = System::Drawing::Color::White;
			this->_mnuViewCollapseAll->Name = L"_mnuViewCollapseAll";
			this->_mnuViewCollapseAll->Size = System::Drawing::Size(139, 22);
			this->_mnuViewCollapseAll->Text = L"&Collapse All";
			this->_mnuViewCollapseAll->Click += gcnew System::EventHandler(this, &EntityForm::_mnuViewCollapseAll_Click);
			// 
			// _panelScroll
			// 
			this->_panelScroll->AutoScroll = true;
			this->_panelScroll->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->_panelScroll->Controls->Add(this->_panelEntity);
			this->_panelScroll->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_panelScroll->Location = System::Drawing::Point(0, 49);
			this->_panelScroll->Name = L"_panelScroll";
			this->_panelScroll->Size = System::Drawing::Size(502, 682);
			this->_panelScroll->TabIndex = 3;
			// 
			// _panelEntity
			// 
			this->_panelEntity->AutoSize = true;
			this->_panelEntity->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->_panelEntity->BackColor = System::Drawing::SystemColors::Control;
			this->_panelEntity->Border = 8;
			this->_panelEntity->Controls->Add(this->_panelSounds);
			this->_panelEntity->Controls->Add(this->_panelAnimations);
			this->_panelEntity->Controls->Add(this->_panelSkeleton);
			this->_panelEntity->Controls->Add(this->_panelModel);
			this->_panelEntity->Dock = System::Windows::Forms::DockStyle::Top;
			this->_panelEntity->Location = System::Drawing::Point(0, 0);
			this->_panelEntity->Name = L"_panelEntity";
			this->_panelEntity->Size = System::Drawing::Size(482, 1113);
			this->_panelEntity->Spacing = 8;
			this->_panelEntity->TabIndex = 2;
			// 
			// _imageList
			// 
			this->_imageList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"_imageList.ImageStream")));
			this->_imageList->TransparentColor = System::Drawing::Color::Transparent;
			this->_imageList->Images->SetKeyName(0, L"openHS.png");
			this->_imageList->Images->SetKeyName(1, L"DeleteHS.png");
			this->_imageList->Images->SetKeyName(2, L"PlayHS.png");
			this->_imageList->Images->SetKeyName(3, L"pause.PNG");
			this->_imageList->Images->SetKeyName(4, L"Expanded.PNG");
			this->_imageList->Images->SetKeyName(5, L"collapsed.png");
			this->_imageList->Images->SetKeyName(6, L"joint.PNG");
			this->_imageList->Images->SetKeyName(7, L"joint_r.PNG");
			this->_imageList->Images->SetKeyName(8, L"tpose.PNG");
			// 
			// _panelSounds
			// 
			this->_panelSounds->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_panelSounds->BackColor = System::Drawing::SystemColors::Control;
			this->_panelSounds->Controls->Add(this->_listSounds);
			this->_panelSounds->Controls->Add(this->_btnAddSound);
			this->_panelSounds->Controls->Add(this->_btnRemoveSound);
			this->_panelSounds->Controls->Add(this->_btnPlaySound);
			this->_panelSounds->EndColour = System::Drawing::Color::LightGray;
			this->_panelSounds->Image = nullptr;
			this->_panelSounds->Location = System::Drawing::Point(8, 827);
			this->_panelSounds->Name = L"_panelSounds";
			this->_panelSounds->PanelState = Salamander::Windows::Forms::PanelState::Expanded;
			this->_panelSounds->Size = System::Drawing::Size(466, 283);
			this->_panelSounds->StartColour = System::Drawing::SystemColors::ControlDarkDark;
			this->_panelSounds->TabIndex = 4;
			this->_panelSounds->TitleFont = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_panelSounds->TitleFontColour = System::Drawing::Color::White;
			this->_panelSounds->TitleText = L"Sounds";
			// 
			// _listSounds
			// 
			this->_listSounds->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_listSounds->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, 
				this->columnHeader2});
			this->_listSounds->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_listSounds->FullRowSelect = true;
			this->_listSounds->GridLines = true;
			this->_listSounds->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->_listSounds->HideSelection = false;
			this->_listSounds->Location = System::Drawing::Point(27, 45);
			this->_listSounds->Name = L"_listSounds";
			this->_listSounds->Size = System::Drawing::Size(400, 191);
			this->_listSounds->TabIndex = 19;
			this->_listSounds->UseCompatibleStateImageBehavior = false;
			this->_listSounds->View = System::Windows::Forms::View::Details;
			this->_listSounds->SelectedIndexChanged += gcnew System::EventHandler(this, &EntityForm::_listSounds_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Name";
			this->columnHeader1->Width = 133;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Sound";
			this->columnHeader2->Width = 281;
			// 
			// _btnAddSound
			// 
			this->_btnAddSound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnAddSound->ImageIndex = 0;
			this->_btnAddSound->ImageList = this->_imageList;
			this->_btnAddSound->Location = System::Drawing::Point(371, 242);
			this->_btnAddSound->Name = L"_btnAddSound";
			this->_btnAddSound->Size = System::Drawing::Size(25, 23);
			this->_btnAddSound->TabIndex = 21;
			this->_btnAddSound->UseVisualStyleBackColor = true;
			this->_btnAddSound->Click += gcnew System::EventHandler(this, &EntityForm::_btnAddSound_Click);
			// 
			// _btnRemoveSound
			// 
			this->_btnRemoveSound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveSound->ImageIndex = 1;
			this->_btnRemoveSound->ImageList = this->_imageList;
			this->_btnRemoveSound->Location = System::Drawing::Point(402, 242);
			this->_btnRemoveSound->Name = L"_btnRemoveSound";
			this->_btnRemoveSound->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveSound->TabIndex = 22;
			this->_btnRemoveSound->UseVisualStyleBackColor = true;
			this->_btnRemoveSound->Click += gcnew System::EventHandler(this, &EntityForm::_btnRemoveSound_Click);
			// 
			// _btnPlaySound
			// 
			this->_btnPlaySound->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnPlaySound->ImageIndex = 2;
			this->_btnPlaySound->ImageList = this->_imageList;
			this->_btnPlaySound->Location = System::Drawing::Point(340, 242);
			this->_btnPlaySound->Name = L"_btnPlaySound";
			this->_btnPlaySound->Size = System::Drawing::Size(25, 23);
			this->_btnPlaySound->TabIndex = 20;
			this->_btnPlaySound->UseVisualStyleBackColor = true;
			this->_btnPlaySound->Click += gcnew System::EventHandler(this, &EntityForm::_btnPlaySound_Click);
			// 
			// _panelAnimations
			// 
			this->_panelAnimations->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_panelAnimations->BackColor = System::Drawing::SystemColors::Control;
			this->_panelAnimations->Controls->Add(this->_listAnimations);
			this->_panelAnimations->Controls->Add(this->_btnAddAnim);
			this->_panelAnimations->Controls->Add(this->_btnRemoveAnim);
			this->_panelAnimations->Controls->Add(this->_btnPlayAnim);
			this->_panelAnimations->Controls->Add(this->_btnTPose);
			this->_panelAnimations->EndColour = System::Drawing::Color::LightGray;
			this->_panelAnimations->Image = nullptr;
			this->_panelAnimations->Location = System::Drawing::Point(8, 524);
			this->_panelAnimations->Name = L"_panelAnimations";
			this->_panelAnimations->PanelState = Salamander::Windows::Forms::PanelState::Expanded;
			this->_panelAnimations->Size = System::Drawing::Size(466, 295);
			this->_panelAnimations->StartColour = System::Drawing::SystemColors::ControlDarkDark;
			this->_panelAnimations->TabIndex = 3;
			this->_panelAnimations->TitleFont = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_panelAnimations->TitleFontColour = System::Drawing::Color::White;
			this->_panelAnimations->TitleText = L"Animations";
			// 
			// _listAnimations
			// 
			this->_listAnimations->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_listAnimations->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->_colHdrName, 
				this->_colHdrAnimation});
			this->_listAnimations->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_listAnimations->FullRowSelect = true;
			this->_listAnimations->GridLines = true;
			this->_listAnimations->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->_listAnimations->HideSelection = false;
			this->_listAnimations->Location = System::Drawing::Point(27, 47);
			this->_listAnimations->Name = L"_listAnimations";
			this->_listAnimations->Size = System::Drawing::Size(400, 200);
			this->_listAnimations->TabIndex = 14;
			this->_listAnimations->UseCompatibleStateImageBehavior = false;
			this->_listAnimations->View = System::Windows::Forms::View::Details;
			this->_listAnimations->SelectedIndexChanged += gcnew System::EventHandler(this, &EntityForm::_listAnimations_SelectedIndexChanged);
			// 
			// _colHdrName
			// 
			this->_colHdrName->Text = L"Name";
			this->_colHdrName->Width = 133;
			// 
			// _colHdrAnimation
			// 
			this->_colHdrAnimation->Text = L"Animation";
			this->_colHdrAnimation->Width = 281;
			// 
			// _btnAddAnim
			// 
			this->_btnAddAnim->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnAddAnim->ImageIndex = 0;
			this->_btnAddAnim->ImageList = this->_imageList;
			this->_btnAddAnim->Location = System::Drawing::Point(371, 253);
			this->_btnAddAnim->Name = L"_btnAddAnim";
			this->_btnAddAnim->Size = System::Drawing::Size(25, 23);
			this->_btnAddAnim->TabIndex = 16;
			this->_btnAddAnim->UseVisualStyleBackColor = true;
			this->_btnAddAnim->Click += gcnew System::EventHandler(this, &EntityForm::_btnAddAnim_Click);
			// 
			// _btnRemoveAnim
			// 
			this->_btnRemoveAnim->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveAnim->ImageIndex = 1;
			this->_btnRemoveAnim->ImageList = this->_imageList;
			this->_btnRemoveAnim->Location = System::Drawing::Point(402, 253);
			this->_btnRemoveAnim->Name = L"_btnRemoveAnim";
			this->_btnRemoveAnim->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveAnim->TabIndex = 17;
			this->_btnRemoveAnim->UseVisualStyleBackColor = true;
			this->_btnRemoveAnim->Click += gcnew System::EventHandler(this, &EntityForm::_btnRemoveAnim_Click);
			// 
			// _btnPlayAnim
			// 
			this->_btnPlayAnim->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnPlayAnim->ImageIndex = 2;
			this->_btnPlayAnim->ImageList = this->_imageList;
			this->_btnPlayAnim->Location = System::Drawing::Point(252, 253);
			this->_btnPlayAnim->Name = L"_btnPlayAnim";
			this->_btnPlayAnim->Size = System::Drawing::Size(25, 23);
			this->_btnPlayAnim->TabIndex = 15;
			this->_btnPlayAnim->UseVisualStyleBackColor = true;
			this->_btnPlayAnim->Click += gcnew System::EventHandler(this, &EntityForm::_btnPlayAnim_Click);
			// 
			// _btnTPose
			// 
			this->_btnTPose->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnTPose->ImageIndex = 8;
			this->_btnTPose->ImageList = this->_imageList;
			this->_btnTPose->Location = System::Drawing::Point(283, 253);
			this->_btnTPose->Name = L"_btnTPose";
			this->_btnTPose->Size = System::Drawing::Size(25, 23);
			this->_btnTPose->TabIndex = 18;
			this->_btnTPose->UseVisualStyleBackColor = true;
			this->_btnTPose->Click += gcnew System::EventHandler(this, &EntityForm::_btnTPose_Click);
			// 
			// _panelSkeleton
			// 
			this->_panelSkeleton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_panelSkeleton->BackColor = System::Drawing::SystemColors::Control;
			this->_panelSkeleton->Controls->Add(this->_btnCollapseAllSkelet);
			this->_panelSkeleton->Controls->Add(this->_btnExpandAllSkelet);
			this->_panelSkeleton->Controls->Add(this->_treeSkelet);
			this->_panelSkeleton->Controls->Add(this->_btnRemoveAttachment);
			this->_panelSkeleton->Controls->Add(this->_btnBrowseAttachment);
			this->_panelSkeleton->Controls->Add(this->_textJointAttachment);
			this->_panelSkeleton->Controls->Add(this->label4);
			this->_panelSkeleton->EndColour = System::Drawing::Color::LightGray;
			this->_panelSkeleton->Image = nullptr;
			this->_panelSkeleton->Location = System::Drawing::Point(8, 118);
			this->_panelSkeleton->Name = L"_panelSkeleton";
			this->_panelSkeleton->PanelState = Salamander::Windows::Forms::PanelState::Expanded;
			this->_panelSkeleton->Size = System::Drawing::Size(466, 398);
			this->_panelSkeleton->StartColour = System::Drawing::SystemColors::ControlDarkDark;
			this->_panelSkeleton->TabIndex = 1;
			this->_panelSkeleton->TitleFont = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_panelSkeleton->TitleFontColour = System::Drawing::Color::White;
			this->_panelSkeleton->TitleText = L"Skeleton";
			// 
			// _btnCollapseAllSkelet
			// 
			this->_btnCollapseAllSkelet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnCollapseAllSkelet->ImageIndex = 4;
			this->_btnCollapseAllSkelet->ImageList = this->_imageList;
			this->_btnCollapseAllSkelet->Location = System::Drawing::Point(402, 311);
			this->_btnCollapseAllSkelet->Name = L"_btnCollapseAllSkelet";
			this->_btnCollapseAllSkelet->Size = System::Drawing::Size(25, 23);
			this->_btnCollapseAllSkelet->TabIndex = 30;
			this->_btnCollapseAllSkelet->UseVisualStyleBackColor = true;
			this->_btnCollapseAllSkelet->Click += gcnew System::EventHandler(this, &EntityForm::_btnCollapseAllSkelet_Click);
			// 
			// _btnExpandAllSkelet
			// 
			this->_btnExpandAllSkelet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnExpandAllSkelet->ImageIndex = 5;
			this->_btnExpandAllSkelet->ImageList = this->_imageList;
			this->_btnExpandAllSkelet->Location = System::Drawing::Point(371, 311);
			this->_btnExpandAllSkelet->Name = L"_btnExpandAllSkelet";
			this->_btnExpandAllSkelet->Size = System::Drawing::Size(25, 23);
			this->_btnExpandAllSkelet->TabIndex = 29;
			this->_btnExpandAllSkelet->UseVisualStyleBackColor = true;
			this->_btnExpandAllSkelet->Click += gcnew System::EventHandler(this, &EntityForm::_btnExpandAllSkelet_Click);
			// 
			// _treeSkelet
			// 
			this->_treeSkelet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_treeSkelet->HideSelection = false;
			this->_treeSkelet->ImageIndex = 0;
			this->_treeSkelet->ImageList = this->_imageList;
			this->_treeSkelet->Location = System::Drawing::Point(27, 39);
			this->_treeSkelet->Name = L"_treeSkelet";
			this->_treeSkelet->SelectedImageIndex = 0;
			this->_treeSkelet->Size = System::Drawing::Size(400, 266);
			this->_treeSkelet->TabIndex = 7;
			this->_treeSkelet->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &EntityForm::_treeSkelet_AfterSelect);
			// 
			// _btnRemoveAttachment
			// 
			this->_btnRemoveAttachment->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveAttachment->ImageIndex = 1;
			this->_btnRemoveAttachment->ImageList = this->_imageList;
			this->_btnRemoveAttachment->Location = System::Drawing::Point(402, 357);
			this->_btnRemoveAttachment->Name = L"_btnRemoveAttachment";
			this->_btnRemoveAttachment->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveAttachment->TabIndex = 28;
			this->_btnRemoveAttachment->UseVisualStyleBackColor = true;
			this->_btnRemoveAttachment->Click += gcnew System::EventHandler(this, &EntityForm::_btnRemoveAttachment_Click);
			// 
			// _btnBrowseAttachment
			// 
			this->_btnBrowseAttachment->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseAttachment->ImageIndex = 0;
			this->_btnBrowseAttachment->ImageList = this->_imageList;
			this->_btnBrowseAttachment->Location = System::Drawing::Point(371, 357);
			this->_btnBrowseAttachment->Name = L"_btnBrowseAttachment";
			this->_btnBrowseAttachment->Size = System::Drawing::Size(25, 23);
			this->_btnBrowseAttachment->TabIndex = 27;
			this->_btnBrowseAttachment->UseVisualStyleBackColor = true;
			this->_btnBrowseAttachment->Click += gcnew System::EventHandler(this, &EntityForm::_btnBrowseAttachment_Click);
			// 
			// _textJointAttachment
			// 
			this->_textJointAttachment->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textJointAttachment->Location = System::Drawing::Point(27, 359);
			this->_textJointAttachment->Name = L"_textJointAttachment";
			this->_textJointAttachment->ReadOnly = true;
			this->_textJointAttachment->Size = System::Drawing::Size(338, 20);
			this->_textJointAttachment->TabIndex = 26;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(24, 341);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(89, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Joint Attachment:";
			// 
			// _panelModel
			// 
			this->_panelModel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_panelModel->BackColor = System::Drawing::SystemColors::Control;
			this->_panelModel->Controls->Add(this->_textModel);
			this->_panelModel->Controls->Add(this->_btnBrowseModel);
			this->_panelModel->EndColour = System::Drawing::Color::LightGray;
			this->_panelModel->Image = nullptr;
			this->_panelModel->Location = System::Drawing::Point(8, 8);
			this->_panelModel->Name = L"_panelModel";
			this->_panelModel->PanelState = Salamander::Windows::Forms::PanelState::Expanded;
			this->_panelModel->Size = System::Drawing::Size(466, 102);
			this->_panelModel->StartColour = System::Drawing::SystemColors::ControlDarkDark;
			this->_panelModel->TabIndex = 0;
			this->_panelModel->TitleFont = (gcnew System::Drawing::Font(L"Tahoma", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_panelModel->TitleFontColour = System::Drawing::Color::White;
			this->_panelModel->TitleText = L"Model";
			// 
			// _textModel
			// 
			this->_textModel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textModel->Location = System::Drawing::Point(27, 50);
			this->_textModel->Name = L"_textModel";
			this->_textModel->ReadOnly = true;
			this->_textModel->Size = System::Drawing::Size(369, 20);
			this->_textModel->TabIndex = 1;
			// 
			// _btnBrowseModel
			// 
			this->_btnBrowseModel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseModel->ImageIndex = 0;
			this->_btnBrowseModel->ImageList = this->_imageList;
			this->_btnBrowseModel->Location = System::Drawing::Point(402, 48);
			this->_btnBrowseModel->Name = L"_btnBrowseModel";
			this->_btnBrowseModel->Size = System::Drawing::Size(25, 23);
			this->_btnBrowseModel->TabIndex = 2;
			this->_btnBrowseModel->UseVisualStyleBackColor = true;
			this->_btnBrowseModel->Click += gcnew System::EventHandler(this, &EntityForm::_btnBrowseModel_Click);
			// 
			// _mainToolbar
			// 
			this->_mainToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_toolBtnExpandAll, 
				this->_toolBtnCollapseAll});
			this->_mainToolbar->Location = System::Drawing::Point(0, 24);
			this->_mainToolbar->Name = L"_mainToolbar";
			this->_mainToolbar->Size = System::Drawing::Size(502, 25);
			this->_mainToolbar->TabIndex = 4;
			this->_mainToolbar->Text = L"Entity Tool Bar";
			// 
			// _toolBtnExpandAll
			// 
			this->_toolBtnExpandAll->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnExpandAll->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnExpandAll.Image")));
			this->_toolBtnExpandAll->ImageTransparentColor = System::Drawing::Color::White;
			this->_toolBtnExpandAll->Name = L"_toolBtnExpandAll";
			this->_toolBtnExpandAll->Size = System::Drawing::Size(23, 22);
			this->_toolBtnExpandAll->Text = L"Expand All";
			this->_toolBtnExpandAll->Click += gcnew System::EventHandler(this, &EntityForm::_mnuViewExpandAll_Click);
			// 
			// _toolBtnCollapseAll
			// 
			this->_toolBtnCollapseAll->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnCollapseAll->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnCollapseAll.Image")));
			this->_toolBtnCollapseAll->ImageTransparentColor = System::Drawing::Color::White;
			this->_toolBtnCollapseAll->Name = L"_toolBtnCollapseAll";
			this->_toolBtnCollapseAll->Size = System::Drawing::Size(23, 22);
			this->_toolBtnCollapseAll->Text = L"Collapse All";
			this->_toolBtnCollapseAll->Click += gcnew System::EventHandler(this, &EntityForm::_mnuViewCollapseAll_Click);
			// 
			// EntityForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScrollMargin = System::Drawing::Size(0, 15);
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(502, 731);
			this->Controls->Add(this->_panelScroll);
			this->Controls->Add(this->_mainToolbar);
			this->Controls->Add(this->_menuEntity);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->HideOnClose = true;
			this->MainMenuStrip = this->_menuEntity;
			this->Name = L"EntityForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->TabText = L"Entity";
			this->Text = L"Entity";
			this->_menuEntity->ResumeLayout(false);
			this->_menuEntity->PerformLayout();
			this->_panelScroll->ResumeLayout(false);
			this->_panelScroll->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_panelEntity))->EndInit();
			this->_panelEntity->ResumeLayout(false);
			this->_panelSounds->ResumeLayout(false);
			this->_panelAnimations->ResumeLayout(false);
			this->_panelSkeleton->ResumeLayout(false);
			this->_panelSkeleton->PerformLayout();
			this->_panelModel->ResumeLayout(false);
			this->_panelModel->PerformLayout();
			this->_mainToolbar->ResumeLayout(false);
			this->_mainToolbar->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	System::Void _btnBrowseModel_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnBrowseAttachment_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnPlayAnim_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnAddAnim_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnRemoveAnim_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnPlaySound_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnAddSound_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnRemoveSound_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
	System::Void _treeSkelet_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
	System::Void _btnRemoveAttachment_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _listAnimations_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void _listSounds_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	System::Void _mnuViewExpandAll_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _mnuViewCollapseAll_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnExpandAllSkelet_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnCollapseAllSkelet_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void _btnTPose_Click(System::Object^  sender, System::EventArgs^  e);
};
}
