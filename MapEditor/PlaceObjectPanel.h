#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	ref class EM_PlaceObject;


	/// <summary>
	/// Summary for PlaceObjectPanel
	/// </summary>
	public ref class PlaceObjectPanel : public System::Windows::Forms::UserControl
	{
	public:
		enum class Mode
		{
			ADD_OBJECT,
			PLACE_OBJECT,
		};

		PlaceObjectPanel(EM_PlaceObject^ edit_mode);
		~PlaceObjectPanel();

		void RefreshObjectTree();
		String^ GetSelObjectPath();
		void SetMode(Mode mode);
		Mode GetMode()
			{ return _mode; }
		void Redraw()
			{ _panelObjectView->Invalidate(); }

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TreeView^  _treeObjects;
	private: System::Windows::Forms::Panel^  _panelObjectView;
	private: System::Windows::Forms::Button^  _buttonDrop;

	private: System::Windows::Forms::ImageList^  _imageListObjTree;
	private: System::Windows::Forms::Button^  _buttonRefresh;

	private: System::Windows::Forms::TextBox^  _textFilter;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::RadioButton^  _buttonAdd;
	private: System::Windows::Forms::RadioButton^  _buttonPlace;


	private: System::ComponentModel::IContainer^  components;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		ref class ObjectTreeNode: public TreeNode
		{
		public:
			enum class NodeType
			{
				MODEL,
				PARTICLE_SYSTEM,
				FOLDER
			};

			ObjectTreeNode(String^ text, String^ rel_path, NodeType type)
				: TreeNode(text)
			{
				ImageIndex = (int)type;
				SelectedImageIndex = (int)type;
				_nodeType = type;
				_relPath = rel_path;
			}

			bool IsDirectory()
				{ return _nodeType == NodeType::FOLDER; }
			NodeType GetNodeType()
				{ return _nodeType; }
			String^ GetRelativePath()
				{ return _relPath; }

		private:
			NodeType _nodeType;
			String^ _relPath;
		};

		void AddDir(String^ path, TreeNodeCollection^ nodes, ObjectTreeNode::NodeType type);
		bool MatchFilter(String^ str);
		void DeleteEntity();

		EM_PlaceObject^ _editMode;
		String^ _filterText;
		Engine::RenderableEntity* _entity;
		bool _entityLoaded;
		GL::Renderer* _renderer;
		GL::Renderbuffer* _objViewColorBuf;
		GL::Renderbuffer* _objViewDepthBuf;
		GL::Framebuffer* _objViewFrameBuf;
		bool _fbufOk;
		Engine::World* _world;
		Bitmap^ _objViewBmp;
		float _objRotX;
		float _objRotY;
		int _prevX;
		int _prevY;
		Mode _mode;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(PlaceObjectPanel::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->_treeObjects = (gcnew System::Windows::Forms::TreeView());
			this->_imageListObjTree = (gcnew System::Windows::Forms::ImageList(this->components));
			this->_panelObjectView = (gcnew System::Windows::Forms::Panel());
			this->_buttonDrop = (gcnew System::Windows::Forms::Button());
			this->_buttonRefresh = (gcnew System::Windows::Forms::Button());
			this->_textFilter = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_buttonAdd = (gcnew System::Windows::Forms::RadioButton());
			this->_buttonPlace = (gcnew System::Windows::Forms::RadioButton());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(20, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(119, 20);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Place Objects";
			// 
			// _treeObjects
			// 
			this->_treeObjects->HideSelection = false;
			this->_treeObjects->ImageIndex = 0;
			this->_treeObjects->ImageList = this->_imageListObjTree;
			this->_treeObjects->Location = System::Drawing::Point(15, 126);
			this->_treeObjects->Name = L"_treeObjects";
			this->_treeObjects->SelectedImageIndex = 0;
			this->_treeObjects->Size = System::Drawing::Size(331, 361);
			this->_treeObjects->TabIndex = 3;
			this->_treeObjects->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &PlaceObjectPanel::_treeObjects_AfterSelect);
			// 
			// _imageListObjTree
			// 
			this->_imageListObjTree->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"_imageListObjTree.ImageStream")));
			this->_imageListObjTree->TransparentColor = System::Drawing::Color::Transparent;
			this->_imageListObjTree->Images->SetKeyName(0, L"obj.PNG");
			this->_imageListObjTree->Images->SetKeyName(1, L"ps.png");
			this->_imageListObjTree->Images->SetKeyName(2, L"f2.ico");
			// 
			// _panelObjectView
			// 
			this->_panelObjectView->BackColor = System::Drawing::Color::DarkGray;
			this->_panelObjectView->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->_panelObjectView->Location = System::Drawing::Point(15, 503);
			this->_panelObjectView->Name = L"_panelObjectView";
			this->_panelObjectView->Size = System::Drawing::Size(331, 238);
			this->_panelObjectView->TabIndex = 4;
			this->_panelObjectView->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PlaceObjectPanel::_panelObjectView_Paint);
			this->_panelObjectView->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &PlaceObjectPanel::_panelObjectView_MouseMove);
			this->_panelObjectView->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PlaceObjectPanel::_panelObjectView_MouseDown);
			this->_panelObjectView->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &PlaceObjectPanel::_panelObjectView_MouseUp);
			// 
			// _buttonDrop
			// 
			this->_buttonDrop->Location = System::Drawing::Point(183, 50);
			this->_buttonDrop->Name = L"_buttonDrop";
			this->_buttonDrop->Size = System::Drawing::Size(75, 30);
			this->_buttonDrop->TabIndex = 5;
			this->_buttonDrop->Text = L"Drop";
			this->_buttonDrop->UseVisualStyleBackColor = true;
			this->_buttonDrop->Click += gcnew System::EventHandler(this, &PlaceObjectPanel::_buttonDrop_Click);
			// 
			// _buttonRefresh
			// 
			this->_buttonRefresh->Location = System::Drawing::Point(264, 50);
			this->_buttonRefresh->Name = L"_buttonRefresh";
			this->_buttonRefresh->Size = System::Drawing::Size(75, 30);
			this->_buttonRefresh->TabIndex = 6;
			this->_buttonRefresh->Text = L"Refresh";
			this->_buttonRefresh->UseVisualStyleBackColor = true;
			this->_buttonRefresh->Click += gcnew System::EventHandler(this, &PlaceObjectPanel::_buttonRefresh_Click);
			// 
			// _textFilter
			// 
			this->_textFilter->Location = System::Drawing::Point(50, 100);
			this->_textFilter->Name = L"_textFilter";
			this->_textFilter->Size = System::Drawing::Size(296, 20);
			this->_textFilter->TabIndex = 2;
			this->_textFilter->TextChanged += gcnew System::EventHandler(this, &PlaceObjectPanel::_textFilter_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 103);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(32, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Filter:";
			// 
			// _buttonAdd
			// 
			this->_buttonAdd->Appearance = System::Windows::Forms::Appearance::Button;
			this->_buttonAdd->AutoCheck = false;
			this->_buttonAdd->Location = System::Drawing::Point(15, 50);
			this->_buttonAdd->Name = L"_buttonAdd";
			this->_buttonAdd->Size = System::Drawing::Size(78, 30);
			this->_buttonAdd->TabIndex = 7;
			this->_buttonAdd->Text = L"Add";
			this->_buttonAdd->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_buttonAdd->UseVisualStyleBackColor = true;
			this->_buttonAdd->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &PlaceObjectPanel::_buttonAdd_MouseClick);
			// 
			// _buttonPlace
			// 
			this->_buttonPlace->Appearance = System::Windows::Forms::Appearance::Button;
			this->_buttonPlace->AutoCheck = false;
			this->_buttonPlace->Location = System::Drawing::Point(99, 50);
			this->_buttonPlace->Name = L"_buttonPlace";
			this->_buttonPlace->Size = System::Drawing::Size(78, 30);
			this->_buttonPlace->TabIndex = 8;
			this->_buttonPlace->Text = L"Place";
			this->_buttonPlace->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_buttonPlace->UseVisualStyleBackColor = true;
			this->_buttonPlace->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &PlaceObjectPanel::_buttonPlace_MouseClick);
			// 
			// PlaceObjectPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->_buttonPlace);
			this->Controls->Add(this->_buttonAdd);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->_textFilter);
			this->Controls->Add(this->_buttonRefresh);
			this->Controls->Add(this->_buttonDrop);
			this->Controls->Add(this->_panelObjectView);
			this->Controls->Add(this->_treeObjects);
			this->Controls->Add(this->label1);
			this->Name = L"PlaceObjectPanel";
			this->Size = System::Drawing::Size(366, 869);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void _buttonAdd_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _buttonPlace_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _buttonRefresh_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _buttonDrop_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _textFilter_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _treeObjects_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
	private: System::Void _panelObjectView_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
	private: System::Void _panelObjectView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _panelObjectView_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _panelObjectView_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
};
}
