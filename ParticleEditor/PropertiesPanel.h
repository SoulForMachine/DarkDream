#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections;


namespace ParticleEditor {

	/// <summary>
	/// Summary for PropertiesPanel
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PropertiesPanel : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		PropertiesPanel(EditorCommon::FormDirector^ director);

		void SetParticleSystem(Engine::ParticleSystem* part_sys);
		void SetGraphAttribute(Engine::ParticleSystem::Attribute* attrib, String^ name, float time, float min_val, float max_val);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PropertiesPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  _panelGraph;
	private: System::Windows::Forms::CheckBox^  _checkPause;
	protected: 

	protected: 



	private: System::Windows::Forms::Label^  _labelAttribName;
	private: System::Windows::Forms::NumericUpDown^  _numTime;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  _numValue;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::NumericUpDown^  _numBBoxDepth;

	private: System::Windows::Forms::NumericUpDown^  _numBBoxHeight;

	private: System::Windows::Forms::NumericUpDown^  _numBBoxWidth;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  _checkShowBBox;
	private: System::Windows::Forms::Button^  _btnRun;
	private: System::Windows::Forms::CheckBox^  _checkShowEmitter;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		value struct GraphPoint
		{
			int index;
			float x;
			float y;
		};

		int PointIndexFromPos(int x, int y);
		bool IsPointSelected(int index);
		void UpdateValueFields();
		void SelectPoint(int index);
		void AddNewPoint(int x, int y);
		void RemovePoint(int index);
		void Modified();

		EditorCommon::FormDirector^ _director;
		Engine::ParticleSystem* _particleSystem;
		Engine::ParticleSystem::Attribute* _attribute;
		float _graphTime;
		float _minValue;
		float _maxValue;
		int _graphWidth;
		int _graphHeight;
		Pen^ _penThickBlack;
		Generic::List<GraphPoint>^ _selectedPoints;
		Point _mouseStart;
		Drawing::Rectangle _selectRect;
		bool _selecting;
		bool _moving;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->_panelGraph = (gcnew System::Windows::Forms::Panel());
			this->_checkPause = (gcnew System::Windows::Forms::CheckBox());
			this->_labelAttribName = (gcnew System::Windows::Forms::Label());
			this->_numTime = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->_numValue = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_checkShowBBox = (gcnew System::Windows::Forms::CheckBox());
			this->_numBBoxDepth = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numBBoxHeight = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numBBoxWidth = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->_btnRun = (gcnew System::Windows::Forms::Button());
			this->_checkShowEmitter = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numTime))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numValue))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numBBoxDepth))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numBBoxHeight))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numBBoxWidth))->BeginInit();
			this->SuspendLayout();
			// 
			// _panelGraph
			// 
			this->_panelGraph->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->_panelGraph->BackColor = System::Drawing::Color::White;
			this->_panelGraph->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->_panelGraph->Location = System::Drawing::Point(426, 33);
			this->_panelGraph->Name = L"_panelGraph";
			this->_panelGraph->Size = System::Drawing::Size(515, 299);
			this->_panelGraph->TabIndex = 0;
			this->_panelGraph->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &PropertiesPanel::_panelGraph_Paint);
			this->_panelGraph->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &PropertiesPanel::_panelGraph_MouseMove);
			this->_panelGraph->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &PropertiesPanel::_panelGraph_MouseDown);
			this->_panelGraph->Resize += gcnew System::EventHandler(this, &PropertiesPanel::_panelGraph_Resize);
			this->_panelGraph->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &PropertiesPanel::_panelGraph_MouseUp);
			// 
			// _checkPause
			// 
			this->_checkPause->Appearance = System::Windows::Forms::Appearance::Button;
			this->_checkPause->Location = System::Drawing::Point(36, 70);
			this->_checkPause->Name = L"_checkPause";
			this->_checkPause->Size = System::Drawing::Size(87, 31);
			this->_checkPause->TabIndex = 1;
			this->_checkPause->Text = L"Pause";
			this->_checkPause->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_checkPause->UseVisualStyleBackColor = true;
			this->_checkPause->Click += gcnew System::EventHandler(this, &PropertiesPanel::_checkPause_Click);
			// 
			// _labelAttribName
			// 
			this->_labelAttribName->AutoSize = true;
			this->_labelAttribName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_labelAttribName->Location = System::Drawing::Point(423, 8);
			this->_labelAttribName->Name = L"_labelAttribName";
			this->_labelAttribName->Size = System::Drawing::Size(70, 18);
			this->_labelAttribName->TabIndex = 2;
			this->_labelAttribName->Text = L"Attribute";
			// 
			// _numTime
			// 
			this->_numTime->DecimalPlaces = 1;
			this->_numTime->Location = System::Drawing::Point(688, 7);
			this->_numTime->Name = L"_numTime";
			this->_numTime->Size = System::Drawing::Size(67, 20);
			this->_numTime->TabIndex = 3;
			this->_numTime->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &PropertiesPanel::_numTime_KeyDown);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(649, 12);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Time:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(793, 12);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(37, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Value:";
			// 
			// _numValue
			// 
			this->_numValue->DecimalPlaces = 2;
			this->_numValue->Location = System::Drawing::Point(836, 7);
			this->_numValue->Name = L"_numValue";
			this->_numValue->Size = System::Drawing::Size(67, 20);
			this->_numValue->TabIndex = 6;
			this->_numValue->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &PropertiesPanel::_numValue_KeyDown);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->_checkShowBBox);
			this->groupBox1->Controls->Add(this->_numBBoxDepth);
			this->groupBox1->Controls->Add(this->_numBBoxHeight);
			this->groupBox1->Controls->Add(this->_numBBoxWidth);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(159, 33);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(199, 158);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Bounding Box";
			// 
			// _checkShowBBox
			// 
			this->_checkShowBBox->AutoSize = true;
			this->_checkShowBBox->Location = System::Drawing::Point(34, 118);
			this->_checkShowBBox->Name = L"_checkShowBBox";
			this->_checkShowBBox->Size = System::Drawing::Size(80, 17);
			this->_checkShowBBox->TabIndex = 8;
			this->_checkShowBBox->Text = L"Show Bbox";
			this->_checkShowBBox->UseVisualStyleBackColor = true;
			this->_checkShowBBox->Click += gcnew System::EventHandler(this, &PropertiesPanel::_checkShowBBox_Click);
			// 
			// _numBBoxDepth
			// 
			this->_numBBoxDepth->DecimalPlaces = 2;
			this->_numBBoxDepth->Location = System::Drawing::Point(85, 82);
			this->_numBBoxDepth->Name = L"_numBBoxDepth";
			this->_numBBoxDepth->Size = System::Drawing::Size(65, 20);
			this->_numBBoxDepth->TabIndex = 5;
			this->_numBBoxDepth->ValueChanged += gcnew System::EventHandler(this, &PropertiesPanel::_numBBoxDepth_ValueChanged);
			// 
			// _numBBoxHeight
			// 
			this->_numBBoxHeight->DecimalPlaces = 2;
			this->_numBBoxHeight->Location = System::Drawing::Point(85, 56);
			this->_numBBoxHeight->Name = L"_numBBoxHeight";
			this->_numBBoxHeight->Size = System::Drawing::Size(65, 20);
			this->_numBBoxHeight->TabIndex = 4;
			this->_numBBoxHeight->ValueChanged += gcnew System::EventHandler(this, &PropertiesPanel::_numBBoxHeight_ValueChanged);
			// 
			// _numBBoxWidth
			// 
			this->_numBBoxWidth->DecimalPlaces = 2;
			this->_numBBoxWidth->Location = System::Drawing::Point(85, 30);
			this->_numBBoxWidth->Name = L"_numBBoxWidth";
			this->_numBBoxWidth->Size = System::Drawing::Size(65, 20);
			this->_numBBoxWidth->TabIndex = 3;
			this->_numBBoxWidth->ValueChanged += gcnew System::EventHandler(this, &PropertiesPanel::_numBBoxWidth_ValueChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(31, 84);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(39, 13);
			this->label5->TabIndex = 2;
			this->label5->Text = L"Depth:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(31, 58);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 13);
			this->label4->TabIndex = 1;
			this->label4->Text = L"Height:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(31, 32);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Width:";
			// 
			// _btnRun
			// 
			this->_btnRun->Location = System::Drawing::Point(36, 33);
			this->_btnRun->Name = L"_btnRun";
			this->_btnRun->Size = System::Drawing::Size(87, 31);
			this->_btnRun->TabIndex = 8;
			this->_btnRun->Text = L"Run";
			this->_btnRun->UseVisualStyleBackColor = true;
			this->_btnRun->Click += gcnew System::EventHandler(this, &PropertiesPanel::_btnRun_Click);
			// 
			// _checkShowEmitter
			// 
			this->_checkShowEmitter->AutoSize = true;
			this->_checkShowEmitter->Location = System::Drawing::Point(36, 151);
			this->_checkShowEmitter->Name = L"_checkShowEmitter";
			this->_checkShowEmitter->Size = System::Drawing::Size(88, 17);
			this->_checkShowEmitter->TabIndex = 9;
			this->_checkShowEmitter->Text = L"Show Emitter";
			this->_checkShowEmitter->UseVisualStyleBackColor = true;
			this->_checkShowEmitter->Click += gcnew System::EventHandler(this, &PropertiesPanel::_checkShowEmitter_Click);
			// 
			// PropertiesPanel
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(953, 344);
			this->Controls->Add(this->_checkShowEmitter);
			this->Controls->Add(this->_btnRun);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->_numValue);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->_numTime);
			this->Controls->Add(this->_labelAttribName);
			this->Controls->Add(this->_checkPause);
			this->Controls->Add(this->_panelGraph);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Name = L"PropertiesPanel";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockBottom;
			this->ShowInTaskbar = false;
			this->Text = L"Properties";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numTime))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numValue))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numBBoxDepth))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numBBoxHeight))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numBBoxWidth))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void _panelGraph_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
	private: System::Void _panelGraph_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _panelGraph_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _panelGraph_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _numBBoxWidth_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numBBoxHeight_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numBBoxDepth_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkShowBBox_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _panelGraph_Resize(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnRun_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkPause_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numTime_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void _numValue_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void _checkShowEmitter_Click(System::Object^  sender, System::EventArgs^  e);
};
}
