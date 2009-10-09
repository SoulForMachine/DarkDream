#pragma once


namespace EntityEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for MaterialForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MaterialForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		MaterialForm(EditorCommon::FormDirector^ director);

		void SetEntity(Engine::ModelEntity* entity);
		void SaveAllMaterials();
		bool AnyModified();
		void DeleteCopiedMaterial();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MaterialForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void UpdateControls();
		void UpdateMaterialList();
		bool SaveMaterial(ListViewItem^ item);
		bool GetColorFromUser(math3d::vec3f& color);
		void MaterialChangedNotify(bool modified);
		ListViewItem^ FindListItem(ListView^ list_view, System::String^ text);
		bool PromptSave();
		void EnableControls(bool enable);

		Engine::ModelEntity* _entity;
		Engine::Material* _material;
		Engine::Material* _copiedMaterial;
		EditorCommon::FormDirector^ _director;

	private: System::Windows::Forms::MenuStrip^  _menuMaterial;

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuFileNew;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuFileSave;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuFileSaveAs;

	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEditCopy;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEditPaste;


	private: System::Windows::Forms::ToolStripMenuItem^  _mnuFileOpen;


	private: System::Windows::Forms::ColorDialog^  _colorDialog;
	private: System::Windows::Forms::OpenFileDialog^  _openFileDialog;
	private: System::Windows::Forms::SaveFileDialog^  _saveFileDialog;
	private: System::Windows::Forms::OpenFileDialog^  _selectTextureDialog;


	private: System::Windows::Forms::ListView^  _listMaterials;
	private: System::Windows::Forms::ColumnHeader^  _colHdrMaterialName;
	private: System::Windows::Forms::ColumnHeader^  _colHdrMaterial;

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;

	private: System::Windows::Forms::Panel^  _panelScroll;

	private: System::Windows::Forms::ToolStrip^  _mainToolbar;

	private: System::Windows::Forms::ToolStripButton^  _toolBtnNew;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnOpen;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnSave;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnCopy;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnPaste;

	private: System::Windows::Forms::ImageList^  _imageList;
	private: System::Windows::Forms::TextBox^  _textDiffuseTex;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::PictureBox^  _clrDiffuse;
	private: System::Windows::Forms::Button^  _btnRemoveDiffuseTex;
	private: System::Windows::Forms::Button^  _btnBrowseDiffuseTex;
	private: System::Windows::Forms::CheckBox^  _checkTransparency;

	private: System::Windows::Forms::TextBox^  _textEmissionTex;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  _btnRemoveEmissionTex;
	private: System::Windows::Forms::Button^  _btnBrowseEmissionTex;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TrackBar^  _trackOpacity;
	private: System::Windows::Forms::TextBox^  _textNormalMap;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  _btnRemoveNormalMap;

	private: System::Windows::Forms::Button^  _btnBrowseNormalMap;
	private: System::Windows::Forms::TextBox^  _textTransparencyMap;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  _btnRemoveTransparencyMap;
	private: System::Windows::Forms::Button^  _btnBrowseTransparencyMap;
	private: System::Windows::Forms::NumericUpDown^  _textOpacity;

	private: System::ComponentModel::IContainer^  components;

	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MaterialForm::typeid));
			this->_menuMaterial = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuFileNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuFileOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuFileSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuFileSaveAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuEditCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuEditPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_colorDialog = (gcnew System::Windows::Forms::ColorDialog());
			this->_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->_selectTextureDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_listMaterials = (gcnew System::Windows::Forms::ListView());
			this->_colHdrMaterialName = (gcnew System::Windows::Forms::ColumnHeader());
			this->_colHdrMaterial = (gcnew System::Windows::Forms::ColumnHeader());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->_panelScroll = (gcnew System::Windows::Forms::Panel());
			this->_textOpacity = (gcnew System::Windows::Forms::NumericUpDown());
			this->_textTransparencyMap = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->_btnRemoveTransparencyMap = (gcnew System::Windows::Forms::Button());
			this->_imageList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->_btnBrowseTransparencyMap = (gcnew System::Windows::Forms::Button());
			this->_textNormalMap = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->_btnRemoveNormalMap = (gcnew System::Windows::Forms::Button());
			this->_btnBrowseNormalMap = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->_trackOpacity = (gcnew System::Windows::Forms::TrackBar());
			this->_checkTransparency = (gcnew System::Windows::Forms::CheckBox());
			this->_textEmissionTex = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_btnRemoveEmissionTex = (gcnew System::Windows::Forms::Button());
			this->_btnBrowseEmissionTex = (gcnew System::Windows::Forms::Button());
			this->_textDiffuseTex = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->_clrDiffuse = (gcnew System::Windows::Forms::PictureBox());
			this->_btnRemoveDiffuseTex = (gcnew System::Windows::Forms::Button());
			this->_btnBrowseDiffuseTex = (gcnew System::Windows::Forms::Button());
			this->_mainToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->_toolBtnNew = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnOpen = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_toolBtnCopy = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnPaste = (gcnew System::Windows::Forms::ToolStripButton());
			this->_menuMaterial->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->_panelScroll->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textOpacity))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackOpacity))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_clrDiffuse))->BeginInit();
			this->_mainToolbar->SuspendLayout();
			this->SuspendLayout();
			// 
			// _menuMaterial
			// 
			this->_menuMaterial->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem});
			this->_menuMaterial->Location = System::Drawing::Point(0, 0);
			this->_menuMaterial->Name = L"_menuMaterial";
			this->_menuMaterial->Size = System::Drawing::Size(543, 24);
			this->_menuMaterial->TabIndex = 0;
			this->_menuMaterial->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->_mnuFileNew, 
				this->_mnuFileOpen, this->_mnuFileSave, this->_mnuFileSaveAs});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// _mnuFileNew
			// 
			this->_mnuFileNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuFileNew.Image")));
			this->_mnuFileNew->Name = L"_mnuFileNew";
			this->_mnuFileNew->Size = System::Drawing::Size(152, 22);
			this->_mnuFileNew->Text = L"&New";
			this->_mnuFileNew->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileNew_Click);
			// 
			// _mnuFileOpen
			// 
			this->_mnuFileOpen->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuFileOpen.Image")));
			this->_mnuFileOpen->Name = L"_mnuFileOpen";
			this->_mnuFileOpen->Size = System::Drawing::Size(152, 22);
			this->_mnuFileOpen->Text = L"&Open";
			this->_mnuFileOpen->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileOpen_Click);
			// 
			// _mnuFileSave
			// 
			this->_mnuFileSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuFileSave.Image")));
			this->_mnuFileSave->Name = L"_mnuFileSave";
			this->_mnuFileSave->Size = System::Drawing::Size(152, 22);
			this->_mnuFileSave->Text = L"&Save";
			this->_mnuFileSave->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileSave_Click);
			// 
			// _mnuFileSaveAs
			// 
			this->_mnuFileSaveAs->Name = L"_mnuFileSaveAs";
			this->_mnuFileSaveAs->Size = System::Drawing::Size(152, 22);
			this->_mnuFileSaveAs->Text = L"Save &As";
			this->_mnuFileSaveAs->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileSaveAs_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_mnuEditCopy, 
				this->_mnuEditPaste});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->editToolStripMenuItem->Text = L"&Edit";
			// 
			// _mnuEditCopy
			// 
			this->_mnuEditCopy->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuEditCopy.Image")));
			this->_mnuEditCopy->Name = L"_mnuEditCopy";
			this->_mnuEditCopy->Size = System::Drawing::Size(152, 22);
			this->_mnuEditCopy->Text = L"&Copy";
			this->_mnuEditCopy->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuEditCopy_Click);
			// 
			// _mnuEditPaste
			// 
			this->_mnuEditPaste->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuEditPaste.Image")));
			this->_mnuEditPaste->Name = L"_mnuEditPaste";
			this->_mnuEditPaste->Size = System::Drawing::Size(152, 22);
			this->_mnuEditPaste->Text = L"&Paste";
			this->_mnuEditPaste->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuEditPaste_Click);
			// 
			// _colorDialog
			// 
			this->_colorDialog->FullOpen = true;
			// 
			// _openFileDialog
			// 
			this->_openFileDialog->DefaultExt = L"material";
			this->_openFileDialog->Filter = L"Material Files (*.material)|*.material|All Files (*.*)|*.*";
			this->_openFileDialog->Title = L"Load Material";
			this->_openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MaterialForm::fileDialog_FileOk);
			// 
			// _saveFileDialog
			// 
			this->_saveFileDialog->DefaultExt = L"material";
			this->_saveFileDialog->Filter = L"Material Files (*.material)|*.material|All Files (*.*)|*.*";
			this->_saveFileDialog->Title = L"Save Material";
			this->_saveFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MaterialForm::fileDialog_FileOk);
			// 
			// _selectTextureDialog
			// 
			this->_selectTextureDialog->Filter = L"Texture Files (*.dds;*.tga;*.bmp)|*.dds;*.tga;*.bmp|All Files (*.*)|*.*";
			this->_selectTextureDialog->Title = L"Select Texture";
			this->_selectTextureDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MaterialForm::fileDialog_FileOk);
			// 
			// _listMaterials
			// 
			this->_listMaterials->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->_colHdrMaterialName, 
				this->_colHdrMaterial});
			this->_listMaterials->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_listMaterials->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_listMaterials->FullRowSelect = true;
			this->_listMaterials->GridLines = true;
			this->_listMaterials->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->_listMaterials->HideSelection = false;
			this->_listMaterials->Location = System::Drawing::Point(3, 3);
			this->_listMaterials->Name = L"_listMaterials";
			this->_listMaterials->Size = System::Drawing::Size(537, 194);
			this->_listMaterials->TabIndex = 1;
			this->_listMaterials->UseCompatibleStateImageBehavior = false;
			this->_listMaterials->View = System::Windows::Forms::View::Details;
			this->_listMaterials->SelectedIndexChanged += gcnew System::EventHandler(this, &MaterialForm::_listMaterials_SelectedIndexChanged);
			// 
			// _colHdrMaterialName
			// 
			this->_colHdrMaterialName->Text = L"Name";
			this->_colHdrMaterialName->Width = 148;
			// 
			// _colHdrMaterial
			// 
			this->_colHdrMaterial->Text = L"Material";
			this->_colHdrMaterial->Width = 276;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->AutoSize = true;
			this->tableLayoutPanel1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel1->Controls->Add(this->_listMaterials, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->_panelScroll, 0, 1);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 49);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 200)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 300)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(543, 669);
			this->tableLayoutPanel1->TabIndex = 4;
			// 
			// _panelScroll
			// 
			this->_panelScroll->AutoScroll = true;
			this->_panelScroll->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->_panelScroll->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->_panelScroll->Controls->Add(this->_textOpacity);
			this->_panelScroll->Controls->Add(this->_textTransparencyMap);
			this->_panelScroll->Controls->Add(this->label5);
			this->_panelScroll->Controls->Add(this->_btnRemoveTransparencyMap);
			this->_panelScroll->Controls->Add(this->_btnBrowseTransparencyMap);
			this->_panelScroll->Controls->Add(this->_textNormalMap);
			this->_panelScroll->Controls->Add(this->label4);
			this->_panelScroll->Controls->Add(this->_btnRemoveNormalMap);
			this->_panelScroll->Controls->Add(this->_btnBrowseNormalMap);
			this->_panelScroll->Controls->Add(this->label3);
			this->_panelScroll->Controls->Add(this->_trackOpacity);
			this->_panelScroll->Controls->Add(this->_checkTransparency);
			this->_panelScroll->Controls->Add(this->_textEmissionTex);
			this->_panelScroll->Controls->Add(this->label2);
			this->_panelScroll->Controls->Add(this->_btnRemoveEmissionTex);
			this->_panelScroll->Controls->Add(this->_btnBrowseEmissionTex);
			this->_panelScroll->Controls->Add(this->_textDiffuseTex);
			this->_panelScroll->Controls->Add(this->label1);
			this->_panelScroll->Controls->Add(this->label12);
			this->_panelScroll->Controls->Add(this->_clrDiffuse);
			this->_panelScroll->Controls->Add(this->_btnRemoveDiffuseTex);
			this->_panelScroll->Controls->Add(this->_btnBrowseDiffuseTex);
			this->_panelScroll->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_panelScroll->Location = System::Drawing::Point(3, 203);
			this->_panelScroll->Name = L"_panelScroll";
			this->_panelScroll->Size = System::Drawing::Size(537, 463);
			this->_panelScroll->TabIndex = 2;
			// 
			// _textOpacity
			// 
			this->_textOpacity->Location = System::Drawing::Point(305, 360);
			this->_textOpacity->Name = L"_textOpacity";
			this->_textOpacity->Size = System::Drawing::Size(53, 20);
			this->_textOpacity->TabIndex = 31;
			this->_textOpacity->ValueChanged += gcnew System::EventHandler(this, &MaterialForm::_textOpacity_ValueChanged);
			// 
			// _textTransparencyMap
			// 
			this->_textTransparencyMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textTransparencyMap->Location = System::Drawing::Point(35, 323);
			this->_textTransparencyMap->Name = L"_textTransparencyMap";
			this->_textTransparencyMap->ReadOnly = true;
			this->_textTransparencyMap->Size = System::Drawing::Size(374, 20);
			this->_textTransparencyMap->TabIndex = 28;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(32, 307);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(99, 13);
			this->label5->TabIndex = 27;
			this->label5->Text = L"Transparency Map:";
			// 
			// _btnRemoveTransparencyMap
			// 
			this->_btnRemoveTransparencyMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveTransparencyMap->ImageIndex = 1;
			this->_btnRemoveTransparencyMap->ImageList = this->_imageList;
			this->_btnRemoveTransparencyMap->Location = System::Drawing::Point(446, 321);
			this->_btnRemoveTransparencyMap->Name = L"_btnRemoveTransparencyMap";
			this->_btnRemoveTransparencyMap->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveTransparencyMap->TabIndex = 30;
			this->_btnRemoveTransparencyMap->UseVisualStyleBackColor = true;
			this->_btnRemoveTransparencyMap->Click += gcnew System::EventHandler(this, &MaterialForm::_btnRemoveTransparencyMap_Click);
			// 
			// _imageList
			// 
			this->_imageList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"_imageList.ImageStream")));
			this->_imageList->TransparentColor = System::Drawing::Color::Transparent;
			this->_imageList->Images->SetKeyName(0, L"openHS.png");
			this->_imageList->Images->SetKeyName(1, L"DeleteHS.png");
			// 
			// _btnBrowseTransparencyMap
			// 
			this->_btnBrowseTransparencyMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseTransparencyMap->ImageIndex = 0;
			this->_btnBrowseTransparencyMap->ImageList = this->_imageList;
			this->_btnBrowseTransparencyMap->Location = System::Drawing::Point(415, 321);
			this->_btnBrowseTransparencyMap->Name = L"_btnBrowseTransparencyMap";
			this->_btnBrowseTransparencyMap->Size = System::Drawing::Size(25, 23);
			this->_btnBrowseTransparencyMap->TabIndex = 29;
			this->_btnBrowseTransparencyMap->UseVisualStyleBackColor = true;
			this->_btnBrowseTransparencyMap->Click += gcnew System::EventHandler(this, &MaterialForm::_btnBrowseTransparencyMap_Click);
			// 
			// _textNormalMap
			// 
			this->_textNormalMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textNormalMap->Location = System::Drawing::Point(35, 221);
			this->_textNormalMap->Name = L"_textNormalMap";
			this->_textNormalMap->ReadOnly = true;
			this->_textNormalMap->Size = System::Drawing::Size(374, 20);
			this->_textNormalMap->TabIndex = 24;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(32, 205);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(67, 13);
			this->label4->TabIndex = 23;
			this->label4->Text = L"Normal Map:";
			// 
			// _btnRemoveNormalMap
			// 
			this->_btnRemoveNormalMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveNormalMap->ImageIndex = 1;
			this->_btnRemoveNormalMap->ImageList = this->_imageList;
			this->_btnRemoveNormalMap->Location = System::Drawing::Point(446, 219);
			this->_btnRemoveNormalMap->Name = L"_btnRemoveNormalMap";
			this->_btnRemoveNormalMap->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveNormalMap->TabIndex = 26;
			this->_btnRemoveNormalMap->UseVisualStyleBackColor = true;
			this->_btnRemoveNormalMap->Click += gcnew System::EventHandler(this, &MaterialForm::_btnRemoveNormalMap_Click);
			// 
			// _btnBrowseNormalMap
			// 
			this->_btnBrowseNormalMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseNormalMap->ImageIndex = 0;
			this->_btnBrowseNormalMap->ImageList = this->_imageList;
			this->_btnBrowseNormalMap->Location = System::Drawing::Point(415, 219);
			this->_btnBrowseNormalMap->Name = L"_btnBrowseNormalMap";
			this->_btnBrowseNormalMap->Size = System::Drawing::Size(25, 23);
			this->_btnBrowseNormalMap->TabIndex = 25;
			this->_btnBrowseNormalMap->UseVisualStyleBackColor = true;
			this->_btnBrowseNormalMap->Click += gcnew System::EventHandler(this, &MaterialForm::_btnBrowseNormalMap_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(32, 365);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(46, 13);
			this->label3->TabIndex = 22;
			this->label3->Text = L"Opacity:";
			// 
			// _trackOpacity
			// 
			this->_trackOpacity->AutoSize = false;
			this->_trackOpacity->Location = System::Drawing::Point(84, 361);
			this->_trackOpacity->Maximum = 100;
			this->_trackOpacity->Name = L"_trackOpacity";
			this->_trackOpacity->Size = System::Drawing::Size(215, 19);
			this->_trackOpacity->TabIndex = 21;
			this->_trackOpacity->TickFrequency = 5;
			this->_trackOpacity->Scroll += gcnew System::EventHandler(this, &MaterialForm::_trackOpacity_Scroll);
			// 
			// _checkTransparency
			// 
			this->_checkTransparency->AutoSize = true;
			this->_checkTransparency->Location = System::Drawing::Point(35, 270);
			this->_checkTransparency->Name = L"_checkTransparency";
			this->_checkTransparency->Size = System::Drawing::Size(91, 17);
			this->_checkTransparency->TabIndex = 20;
			this->_checkTransparency->Text = L"Transparency";
			this->_checkTransparency->UseVisualStyleBackColor = true;
			this->_checkTransparency->Click += gcnew System::EventHandler(this, &MaterialForm::_checkTransparency_Click);
			// 
			// _textEmissionTex
			// 
			this->_textEmissionTex->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textEmissionTex->Location = System::Drawing::Point(35, 52);
			this->_textEmissionTex->Name = L"_textEmissionTex";
			this->_textEmissionTex->ReadOnly = true;
			this->_textEmissionTex->Size = System::Drawing::Size(374, 20);
			this->_textEmissionTex->TabIndex = 17;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(32, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(90, 13);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Emission Texture:";
			// 
			// _btnRemoveEmissionTex
			// 
			this->_btnRemoveEmissionTex->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveEmissionTex->ImageIndex = 1;
			this->_btnRemoveEmissionTex->ImageList = this->_imageList;
			this->_btnRemoveEmissionTex->Location = System::Drawing::Point(446, 50);
			this->_btnRemoveEmissionTex->Name = L"_btnRemoveEmissionTex";
			this->_btnRemoveEmissionTex->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveEmissionTex->TabIndex = 19;
			this->_btnRemoveEmissionTex->UseVisualStyleBackColor = true;
			this->_btnRemoveEmissionTex->Click += gcnew System::EventHandler(this, &MaterialForm::_btnRemoveEmissionTex_Click);
			// 
			// _btnBrowseEmissionTex
			// 
			this->_btnBrowseEmissionTex->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseEmissionTex->ImageIndex = 0;
			this->_btnBrowseEmissionTex->ImageList = this->_imageList;
			this->_btnBrowseEmissionTex->Location = System::Drawing::Point(415, 50);
			this->_btnBrowseEmissionTex->Name = L"_btnBrowseEmissionTex";
			this->_btnBrowseEmissionTex->Size = System::Drawing::Size(25, 23);
			this->_btnBrowseEmissionTex->TabIndex = 18;
			this->_btnBrowseEmissionTex->UseVisualStyleBackColor = true;
			this->_btnBrowseEmissionTex->Click += gcnew System::EventHandler(this, &MaterialForm::_btnBrowseEmissionTex_Click);
			// 
			// _textDiffuseTex
			// 
			this->_textDiffuseTex->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_textDiffuseTex->Location = System::Drawing::Point(35, 156);
			this->_textDiffuseTex->Name = L"_textDiffuseTex";
			this->_textDiffuseTex->ReadOnly = true;
			this->_textDiffuseTex->Size = System::Drawing::Size(374, 20);
			this->_textDiffuseTex->TabIndex = 13;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(32, 101);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(70, 13);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Diffuse Color:";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(32, 140);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(82, 13);
			this->label12->TabIndex = 12;
			this->label12->Text = L"Diffuse Texture:";
			// 
			// _clrDiffuse
			// 
			this->_clrDiffuse->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->_clrDiffuse->Location = System::Drawing::Point(106, 93);
			this->_clrDiffuse->Name = L"_clrDiffuse";
			this->_clrDiffuse->Size = System::Drawing::Size(67, 25);
			this->_clrDiffuse->TabIndex = 10;
			this->_clrDiffuse->TabStop = false;
			this->_clrDiffuse->Click += gcnew System::EventHandler(this, &MaterialForm::_clrDiffuse_Click);
			// 
			// _btnRemoveDiffuseTex
			// 
			this->_btnRemoveDiffuseTex->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnRemoveDiffuseTex->ImageIndex = 1;
			this->_btnRemoveDiffuseTex->ImageList = this->_imageList;
			this->_btnRemoveDiffuseTex->Location = System::Drawing::Point(446, 154);
			this->_btnRemoveDiffuseTex->Name = L"_btnRemoveDiffuseTex";
			this->_btnRemoveDiffuseTex->Size = System::Drawing::Size(25, 23);
			this->_btnRemoveDiffuseTex->TabIndex = 15;
			this->_btnRemoveDiffuseTex->UseVisualStyleBackColor = true;
			this->_btnRemoveDiffuseTex->Click += gcnew System::EventHandler(this, &MaterialForm::_btnRemoveDiffuseTex_Click);
			// 
			// _btnBrowseDiffuseTex
			// 
			this->_btnBrowseDiffuseTex->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->_btnBrowseDiffuseTex->ImageIndex = 0;
			this->_btnBrowseDiffuseTex->ImageList = this->_imageList;
			this->_btnBrowseDiffuseTex->Location = System::Drawing::Point(415, 154);
			this->_btnBrowseDiffuseTex->Name = L"_btnBrowseDiffuseTex";
			this->_btnBrowseDiffuseTex->Size = System::Drawing::Size(25, 23);
			this->_btnBrowseDiffuseTex->TabIndex = 14;
			this->_btnBrowseDiffuseTex->UseVisualStyleBackColor = true;
			this->_btnBrowseDiffuseTex->Click += gcnew System::EventHandler(this, &MaterialForm::_btnBrowseDiffuseTex_Click);
			// 
			// _mainToolbar
			// 
			this->_mainToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->_toolBtnNew, this->_toolBtnOpen, 
				this->_toolBtnSave, this->toolStripSeparator1, this->_toolBtnCopy, this->_toolBtnPaste});
			this->_mainToolbar->Location = System::Drawing::Point(0, 24);
			this->_mainToolbar->Name = L"_mainToolbar";
			this->_mainToolbar->Size = System::Drawing::Size(543, 25);
			this->_mainToolbar->TabIndex = 5;
			this->_mainToolbar->Text = L"Material Tool Bar";
			// 
			// _toolBtnNew
			// 
			this->_toolBtnNew->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnNew.Image")));
			this->_toolBtnNew->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnNew->Name = L"_toolBtnNew";
			this->_toolBtnNew->Size = System::Drawing::Size(23, 22);
			this->_toolBtnNew->Text = L"New Material";
			this->_toolBtnNew->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileNew_Click);
			// 
			// _toolBtnOpen
			// 
			this->_toolBtnOpen->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnOpen->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnOpen.Image")));
			this->_toolBtnOpen->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnOpen->Name = L"_toolBtnOpen";
			this->_toolBtnOpen->Size = System::Drawing::Size(23, 22);
			this->_toolBtnOpen->Text = L"Open Material";
			this->_toolBtnOpen->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileOpen_Click);
			// 
			// _toolBtnSave
			// 
			this->_toolBtnSave->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnSave.Image")));
			this->_toolBtnSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnSave->Name = L"_toolBtnSave";
			this->_toolBtnSave->Size = System::Drawing::Size(23, 22);
			this->_toolBtnSave->Text = L"Save Material";
			this->_toolBtnSave->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuFileSave_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// _toolBtnCopy
			// 
			this->_toolBtnCopy->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnCopy->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnCopy.Image")));
			this->_toolBtnCopy->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnCopy->Name = L"_toolBtnCopy";
			this->_toolBtnCopy->Size = System::Drawing::Size(23, 22);
			this->_toolBtnCopy->Text = L"Copy Material";
			this->_toolBtnCopy->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuEditCopy_Click);
			// 
			// _toolBtnPaste
			// 
			this->_toolBtnPaste->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnPaste->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnPaste.Image")));
			this->_toolBtnPaste->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnPaste->Name = L"_toolBtnPaste";
			this->_toolBtnPaste->Size = System::Drawing::Size(23, 22);
			this->_toolBtnPaste->Text = L"Paste Material";
			this->_toolBtnPaste->Click += gcnew System::EventHandler(this, &MaterialForm::_mnuEditPaste_Click);
			// 
			// MaterialForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(543, 718);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->_mainToolbar);
			this->Controls->Add(this->_menuMaterial);
			this->HideOnClose = true;
			this->MainMenuStrip = this->_menuMaterial;
			this->Name = L"MaterialForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->TabText = L"Material";
			this->Text = L"Material";
			this->Load += gcnew System::EventHandler(this, &MaterialForm::MaterialForm_Load);
			this->_menuMaterial->ResumeLayout(false);
			this->_menuMaterial->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->_panelScroll->ResumeLayout(false);
			this->_panelScroll->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textOpacity))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackOpacity))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_clrDiffuse))->EndInit();
			this->_mainToolbar->ResumeLayout(false);
			this->_mainToolbar->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		System::Void _listMaterials_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void _mnuFileNew_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _mnuFileOpen_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _mnuFileSave_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _mnuFileSaveAs_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _mnuEditCopy_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _mnuEditPaste_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _clrDiffuse_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnBrowseDiffuseTex_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnBrowseNormalMap_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _trackOpacity_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void _textOpacity_ValueChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void fileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
		System::Void MaterialForm_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnRemoveDiffuseTex_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnRemoveNormalMap_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnBrowseEmissionTex_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnRemoveEmissionTex_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _checkTransparency_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnBrowseTransparencyMap_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void _btnRemoveTransparencyMap_Click(System::Object^  sender, System::EventArgs^  e);
};

}
