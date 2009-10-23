#pragma once

#include "EM_PaintGrass.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;


namespace MapEditor {

	/// <summary>
	/// Summary for GrassPanel
	/// </summary>
	public ref class GrassPanel : public System::Windows::Forms::UserControl
	{
	public:
		GrassPanel(EM_PaintGrass::Parameters^ params);

		void UpdateControls();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~GrassPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  _textTexture;
	private: System::Windows::Forms::Button^  _buttonBrowse;




	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::NumericUpDown^  _numRadius;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  _numScale;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  _numStrength;
	private: System::Windows::Forms::NumericUpDown^  _numHardness;
	private: System::Windows::Forms::TrackBar^  _trackRadius;
	private: System::Windows::Forms::TrackBar^  _trackScale;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TrackBar^  _trackHardness;
	private: System::Windows::Forms::TrackBar^  _trackStrength;
	private: System::Windows::Forms::OpenFileDialog^  _selectTextureDialog;





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Panel^  _panelDummy;

		bool FillTextureList(const GL::Texture* texture);

		EditorCommon::TextureListView^ _textureList;
		EM_PaintGrass::Parameters^ _parameters;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_textTexture = (gcnew System::Windows::Forms::TextBox());
			this->_buttonBrowse = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_numRadius = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->_numScale = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->_numStrength = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numHardness = (gcnew System::Windows::Forms::NumericUpDown());
			this->_trackRadius = (gcnew System::Windows::Forms::TrackBar());
			this->_trackScale = (gcnew System::Windows::Forms::TrackBar());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->_trackHardness = (gcnew System::Windows::Forms::TrackBar());
			this->_trackStrength = (gcnew System::Windows::Forms::TrackBar());
			this->_selectTextureDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_panelDummy = (gcnew System::Windows::Forms::Panel());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numRadius))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numScale))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numStrength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHardness))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackRadius))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackScale))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHardness))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackStrength))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(20, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(57, 20);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Grass";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 254);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(46, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Texture:";
			// 
			// _textTexture
			// 
			this->_textTexture->Location = System::Drawing::Point(73, 251);
			this->_textTexture->Name = L"_textTexture";
			this->_textTexture->ReadOnly = true;
			this->_textTexture->Size = System::Drawing::Size(218, 20);
			this->_textTexture->TabIndex = 3;
			// 
			// _buttonBrowse
			// 
			this->_buttonBrowse->Location = System::Drawing::Point(297, 249);
			this->_buttonBrowse->Name = L"_buttonBrowse";
			this->_buttonBrowse->Size = System::Drawing::Size(75, 23);
			this->_buttonBrowse->TabIndex = 4;
			this->_buttonBrowse->Text = L"Browse";
			this->_buttonBrowse->UseVisualStyleBackColor = true;
			this->_buttonBrowse->Click += gcnew System::EventHandler(this, &GrassPanel::_buttonBrowse_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->_numRadius);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->_numScale);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->_numStrength);
			this->groupBox1->Controls->Add(this->_numHardness);
			this->groupBox1->Controls->Add(this->_trackRadius);
			this->groupBox1->Controls->Add(this->_trackScale);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->_trackHardness);
			this->groupBox1->Controls->Add(this->_trackStrength);
			this->groupBox1->Location = System::Drawing::Point(24, 58);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(348, 162);
			this->groupBox1->TabIndex = 9;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Brush properties";
			// 
			// _numRadius
			// 
			this->_numRadius->DecimalPlaces = 1;
			this->_numRadius->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			this->_numRadius->Location = System::Drawing::Point(278, 37);
			this->_numRadius->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {16, 0, 0, 0});
			this->_numRadius->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numRadius->Name = L"_numRadius";
			this->_numRadius->Size = System::Drawing::Size(52, 20);
			this->_numRadius->TabIndex = 2;
			this->_numRadius->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numRadius->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numRadius_ValueChanged);
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(17, 34);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(43, 25);
			this->label4->TabIndex = 0;
			this->label4->Text = L"Radius:";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label5
			// 
			this->label5->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label5->Location = System::Drawing::Point(17, 64);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(56, 15);
			this->label5->TabIndex = 3;
			this->label5->Text = L"Hardness:";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// _numScale
			// 
			this->_numScale->DecimalPlaces = 1;
			this->_numScale->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_numScale->Location = System::Drawing::Point(278, 112);
			this->_numScale->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->_numScale->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_numScale->Name = L"_numScale";
			this->_numScale->Size = System::Drawing::Size(52, 20);
			this->_numScale->TabIndex = 11;
			this->_numScale->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_numScale->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numScale_ValueChanged);
			// 
			// label6
			// 
			this->label6->Location = System::Drawing::Point(17, 85);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(56, 20);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Strength:";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// _numStrength
			// 
			this->_numStrength->Location = System::Drawing::Point(278, 87);
			this->_numStrength->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->_numStrength->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numStrength->Name = L"_numStrength";
			this->_numStrength->Size = System::Drawing::Size(52, 20);
			this->_numStrength->TabIndex = 8;
			this->_numStrength->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numStrength->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numStrength_ValueChanged);
			// 
			// _numHardness
			// 
			this->_numHardness->Location = System::Drawing::Point(278, 62);
			this->_numHardness->Name = L"_numHardness";
			this->_numHardness->Size = System::Drawing::Size(52, 20);
			this->_numHardness->TabIndex = 5;
			this->_numHardness->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numHardness_ValueChanged);
			// 
			// _trackRadius
			// 
			this->_trackRadius->AutoSize = false;
			this->_trackRadius->Location = System::Drawing::Point(72, 37);
			this->_trackRadius->Maximum = 32;
			this->_trackRadius->Minimum = 2;
			this->_trackRadius->Name = L"_trackRadius";
			this->_trackRadius->Size = System::Drawing::Size(200, 19);
			this->_trackRadius->TabIndex = 1;
			this->_trackRadius->Value = 2;
			this->_trackRadius->Scroll += gcnew System::EventHandler(this, &GrassPanel::_trackRadius_Scroll);
			// 
			// _trackScale
			// 
			this->_trackScale->AutoSize = false;
			this->_trackScale->Location = System::Drawing::Point(72, 112);
			this->_trackScale->Maximum = 30;
			this->_trackScale->Minimum = 1;
			this->_trackScale->Name = L"_trackScale";
			this->_trackScale->Size = System::Drawing::Size(200, 19);
			this->_trackScale->TabIndex = 10;
			this->_trackScale->Value = 1;
			this->_trackScale->Scroll += gcnew System::EventHandler(this, &GrassPanel::_trackScale_Scroll);
			// 
			// label7
			// 
			this->label7->Location = System::Drawing::Point(17, 106);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(41, 25);
			this->label7->TabIndex = 9;
			this->label7->Text = L"Scale:";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// _trackHardness
			// 
			this->_trackHardness->AutoSize = false;
			this->_trackHardness->Location = System::Drawing::Point(72, 62);
			this->_trackHardness->Maximum = 100;
			this->_trackHardness->Name = L"_trackHardness";
			this->_trackHardness->Size = System::Drawing::Size(200, 19);
			this->_trackHardness->TabIndex = 4;
			this->_trackHardness->TickFrequency = 5;
			this->_trackHardness->Scroll += gcnew System::EventHandler(this, &GrassPanel::_trackHardness_Scroll);
			// 
			// _trackStrength
			// 
			this->_trackStrength->AutoSize = false;
			this->_trackStrength->LargeChange = 2;
			this->_trackStrength->Location = System::Drawing::Point(72, 87);
			this->_trackStrength->Minimum = 1;
			this->_trackStrength->Name = L"_trackStrength";
			this->_trackStrength->Size = System::Drawing::Size(200, 19);
			this->_trackStrength->TabIndex = 7;
			this->_trackStrength->Value = 1;
			this->_trackStrength->Scroll += gcnew System::EventHandler(this, &GrassPanel::_trackStrength_Scroll);
			// 
			// _selectTextureDialog
			// 
			this->_selectTextureDialog->Filter = L"Texture Files (*.dds;*.tga;*.bmp)|*.dds;*.tga;*.bmp|All Files (*.*)|*.*";
			this->_selectTextureDialog->Title = L"Select Texture";
			this->_selectTextureDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &GrassPanel::_selectTextureDialog_FileOk);
			// 
			// _panelDummy
			// 
			this->_panelDummy->Location = System::Drawing::Point(24, 288);
			this->_panelDummy->Name = L"_panelDummy";
			this->_panelDummy->Size = System::Drawing::Size(348, 428);
			this->_panelDummy->TabIndex = 10;
			// 
			// GrassPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->_panelDummy);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->_buttonBrowse);
			this->Controls->Add(this->_textTexture);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"GrassPanel";
			this->Size = System::Drawing::Size(394, 791);
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numRadius))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numScale))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numStrength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHardness))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackRadius))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackScale))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHardness))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackStrength))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void _trackRadius_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numRadius_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackHardness_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numHardness_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackStrength_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numStrength_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackScale_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numScale_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _buttonBrowse_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _selectTextureDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
};
}
