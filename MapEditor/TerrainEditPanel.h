#pragma once

#include "EM_TerrainEdit.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	/// <summary>
	/// Summary for TerrainEditPanel
	/// </summary>
	public ref class TerrainEditPanel : public System::Windows::Forms::UserControl
	{
	public:
		TerrainEditPanel(EM_TerrainEdit::Parameters^ params);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TerrainEditPanel()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void UpdateControls();

		EM_TerrainEdit::Parameters^ _parameters;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::RadioButton^  _btnRaiseLower;
	private: System::Windows::Forms::RadioButton^  _btnPlateau;


	private: System::Windows::Forms::RadioButton^  _btnSmooth;
	private: System::Windows::Forms::RadioButton^  _btnNoise;
	private: System::Windows::Forms::RadioButton^  _btnRelativePlateau;
	private: System::Windows::Forms::RadioButton^  _btnRamp;




	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TableLayoutPanel^  _tableLayoutBrushProperties;
	private: System::Windows::Forms::TrackBar^  _trackRadius;
	private: System::Windows::Forms::TrackBar^  _trackHardness;
	private: System::Windows::Forms::TrackBar^  _trackStrength;
	private: System::Windows::Forms::TrackBar^  _trackHeight;
	private: System::Windows::Forms::NumericUpDown^  _numRadius;
	private: System::Windows::Forms::NumericUpDown^  _numHardness;
	private: System::Windows::Forms::NumericUpDown^  _numStrength;
	private: System::Windows::Forms::NumericUpDown^  _numHeight;

	protected: 

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->_btnRaiseLower = (gcnew System::Windows::Forms::RadioButton());
			this->_btnPlateau = (gcnew System::Windows::Forms::RadioButton());
			this->_btnSmooth = (gcnew System::Windows::Forms::RadioButton());
			this->_btnNoise = (gcnew System::Windows::Forms::RadioButton());
			this->_btnRelativePlateau = (gcnew System::Windows::Forms::RadioButton());
			this->_btnRamp = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_tableLayoutBrushProperties = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->_numRadius = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->_trackRadius = (gcnew System::Windows::Forms::TrackBar());
			this->_trackHardness = (gcnew System::Windows::Forms::TrackBar());
			this->_trackStrength = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->_trackHeight = (gcnew System::Windows::Forms::TrackBar());
			this->_numHardness = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numStrength = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numHeight = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1->SuspendLayout();
			this->_tableLayoutBrushProperties->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numRadius))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackRadius))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHardness))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackStrength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHeight))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHardness))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numStrength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHeight))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(16, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(102, 20);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Terrain Edit";
			// 
			// _btnRaiseLower
			// 
			this->_btnRaiseLower->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnRaiseLower->Location = System::Drawing::Point(20, 55);
			this->_btnRaiseLower->Name = L"_btnRaiseLower";
			this->_btnRaiseLower->Size = System::Drawing::Size(112, 33);
			this->_btnRaiseLower->TabIndex = 1;
			this->_btnRaiseLower->Text = L"Raise/Lower";
			this->_btnRaiseLower->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_btnRaiseLower->UseVisualStyleBackColor = true;
			this->_btnRaiseLower->Click += gcnew System::EventHandler(this, &TerrainEditPanel::_btnRaiseLower_Click);
			// 
			// _btnPlateau
			// 
			this->_btnPlateau->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnPlateau->Location = System::Drawing::Point(20, 94);
			this->_btnPlateau->Name = L"_btnPlateau";
			this->_btnPlateau->Size = System::Drawing::Size(112, 33);
			this->_btnPlateau->TabIndex = 4;
			this->_btnPlateau->Text = L"Plateau";
			this->_btnPlateau->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_btnPlateau->UseVisualStyleBackColor = true;
			this->_btnPlateau->Click += gcnew System::EventHandler(this, &TerrainEditPanel::_btnPlateau_Click);
			// 
			// _btnSmooth
			// 
			this->_btnSmooth->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnSmooth->Location = System::Drawing::Point(138, 55);
			this->_btnSmooth->Name = L"_btnSmooth";
			this->_btnSmooth->Size = System::Drawing::Size(112, 33);
			this->_btnSmooth->TabIndex = 2;
			this->_btnSmooth->Text = L"Smooth";
			this->_btnSmooth->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_btnSmooth->UseVisualStyleBackColor = true;
			this->_btnSmooth->Click += gcnew System::EventHandler(this, &TerrainEditPanel::_btnSmooth_Click);
			// 
			// _btnNoise
			// 
			this->_btnNoise->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnNoise->Location = System::Drawing::Point(256, 55);
			this->_btnNoise->Name = L"_btnNoise";
			this->_btnNoise->Size = System::Drawing::Size(112, 33);
			this->_btnNoise->TabIndex = 3;
			this->_btnNoise->Text = L"Noise";
			this->_btnNoise->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_btnNoise->UseVisualStyleBackColor = true;
			this->_btnNoise->Click += gcnew System::EventHandler(this, &TerrainEditPanel::_btnNoise_Click);
			// 
			// _btnRelativePlateau
			// 
			this->_btnRelativePlateau->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnRelativePlateau->Location = System::Drawing::Point(138, 94);
			this->_btnRelativePlateau->Name = L"_btnRelativePlateau";
			this->_btnRelativePlateau->Size = System::Drawing::Size(112, 33);
			this->_btnRelativePlateau->TabIndex = 5;
			this->_btnRelativePlateau->Text = L"Relative Plateau";
			this->_btnRelativePlateau->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_btnRelativePlateau->UseVisualStyleBackColor = true;
			this->_btnRelativePlateau->Click += gcnew System::EventHandler(this, &TerrainEditPanel::_btnRelativePlateau_Click);
			// 
			// _btnRamp
			// 
			this->_btnRamp->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnRamp->Location = System::Drawing::Point(256, 94);
			this->_btnRamp->Name = L"_btnRamp";
			this->_btnRamp->Size = System::Drawing::Size(112, 33);
			this->_btnRamp->TabIndex = 6;
			this->_btnRamp->Text = L"Ramp";
			this->_btnRamp->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->_btnRamp->UseVisualStyleBackColor = true;
			this->_btnRamp->Click += gcnew System::EventHandler(this, &TerrainEditPanel::_btnRamp_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->_tableLayoutBrushProperties);
			this->groupBox1->Location = System::Drawing::Point(20, 150);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(348, 207);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Brush properties";
			// 
			// _tableLayoutBrushProperties
			// 
			this->_tableLayoutBrushProperties->ColumnCount = 3;
			this->_tableLayoutBrushProperties->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				21.18056F)));
			this->_tableLayoutBrushProperties->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				78.81944F)));
			this->_tableLayoutBrushProperties->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				66)));
			this->_tableLayoutBrushProperties->Controls->Add(this->_numRadius, 2, 0);
			this->_tableLayoutBrushProperties->Controls->Add(this->label2, 0, 0);
			this->_tableLayoutBrushProperties->Controls->Add(this->label4, 0, 2);
			this->_tableLayoutBrushProperties->Controls->Add(this->_trackRadius, 1, 0);
			this->_tableLayoutBrushProperties->Controls->Add(this->_trackHardness, 1, 1);
			this->_tableLayoutBrushProperties->Controls->Add(this->_trackStrength, 1, 2);
			this->_tableLayoutBrushProperties->Controls->Add(this->label5, 0, 3);
			this->_tableLayoutBrushProperties->Controls->Add(this->_trackHeight, 1, 3);
			this->_tableLayoutBrushProperties->Controls->Add(this->_numHardness, 2, 1);
			this->_tableLayoutBrushProperties->Controls->Add(this->_numStrength, 2, 2);
			this->_tableLayoutBrushProperties->Controls->Add(this->_numHeight, 2, 3);
			this->_tableLayoutBrushProperties->Controls->Add(this->label3, 0, 1);
			this->_tableLayoutBrushProperties->Location = System::Drawing::Point(6, 32);
			this->_tableLayoutBrushProperties->Name = L"_tableLayoutBrushProperties";
			this->_tableLayoutBrushProperties->RowCount = 4;
			this->_tableLayoutBrushProperties->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 
				25)));
			this->_tableLayoutBrushProperties->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 
				25)));
			this->_tableLayoutBrushProperties->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 
				25)));
			this->_tableLayoutBrushProperties->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 
				25)));
			this->_tableLayoutBrushProperties->Size = System::Drawing::Size(336, 100);
			this->_tableLayoutBrushProperties->TabIndex = 4;
			// 
			// _numRadius
			// 
			this->_numRadius->DecimalPlaces = 1;
			this->_numRadius->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
			this->_numRadius->Location = System::Drawing::Point(272, 3);
			this->_numRadius->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {16, 0, 0, 0});
			this->_numRadius->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numRadius->Name = L"_numRadius";
			this->_numRadius->Size = System::Drawing::Size(52, 20);
			this->_numRadius->TabIndex = 2;
			this->_numRadius->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numRadius->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numRadius_ValueChanged);
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(3, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 25);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Radius:";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label3
			// 
			this->label3->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label3->Location = System::Drawing::Point(3, 30);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(50, 15);
			this->label3->TabIndex = 3;
			this->label3->Text = L"Hardness:";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(3, 50);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(50, 20);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Strength:";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// _trackRadius
			// 
			this->_trackRadius->Location = System::Drawing::Point(60, 3);
			this->_trackRadius->Maximum = 32;
			this->_trackRadius->Minimum = 2;
			this->_trackRadius->Name = L"_trackRadius";
			this->_trackRadius->Size = System::Drawing::Size(206, 19);
			this->_trackRadius->TabIndex = 1;
			this->_trackRadius->Value = 2;
			this->_trackRadius->Scroll += gcnew System::EventHandler(this, &TerrainEditPanel::_trackRadius_Scroll);
			// 
			// _trackHardness
			// 
			this->_trackHardness->Location = System::Drawing::Point(60, 28);
			this->_trackHardness->Maximum = 100;
			this->_trackHardness->Name = L"_trackHardness";
			this->_trackHardness->Size = System::Drawing::Size(206, 19);
			this->_trackHardness->TabIndex = 4;
			this->_trackHardness->TickFrequency = 5;
			this->_trackHardness->Scroll += gcnew System::EventHandler(this, &TerrainEditPanel::_trackHardness_Scroll);
			// 
			// _trackStrength
			// 
			this->_trackStrength->Location = System::Drawing::Point(60, 53);
			this->_trackStrength->Minimum = 1;
			this->_trackStrength->Name = L"_trackStrength";
			this->_trackStrength->Size = System::Drawing::Size(206, 19);
			this->_trackStrength->TabIndex = 7;
			this->_trackStrength->Value = 1;
			this->_trackStrength->Scroll += gcnew System::EventHandler(this, &TerrainEditPanel::_trackStrength_Scroll);
			// 
			// label5
			// 
			this->label5->Location = System::Drawing::Point(3, 75);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(41, 25);
			this->label5->TabIndex = 9;
			this->label5->Text = L"Height:";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// _trackHeight
			// 
			this->_trackHeight->Location = System::Drawing::Point(60, 78);
			this->_trackHeight->Maximum = 15;
			this->_trackHeight->Minimum = -15;
			this->_trackHeight->Name = L"_trackHeight";
			this->_trackHeight->Size = System::Drawing::Size(206, 19);
			this->_trackHeight->TabIndex = 10;
			this->_trackHeight->Value = -15;
			this->_trackHeight->Scroll += gcnew System::EventHandler(this, &TerrainEditPanel::_trackHeight_Scroll);
			// 
			// _numHardness
			// 
			this->_numHardness->Location = System::Drawing::Point(272, 28);
			this->_numHardness->Name = L"_numHardness";
			this->_numHardness->Size = System::Drawing::Size(52, 20);
			this->_numHardness->TabIndex = 5;
			this->_numHardness->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numHardness_ValueChanged);
			// 
			// _numStrength
			// 
			this->_numStrength->Location = System::Drawing::Point(272, 53);
			this->_numStrength->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->_numStrength->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numStrength->Name = L"_numStrength";
			this->_numStrength->Size = System::Drawing::Size(52, 20);
			this->_numStrength->TabIndex = 8;
			this->_numStrength->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_numStrength->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numStrength_ValueChanged);
			// 
			// _numHeight
			// 
			this->_numHeight->DecimalPlaces = 1;
			this->_numHeight->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_numHeight->Location = System::Drawing::Point(272, 78);
			this->_numHeight->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, 0});
			this->_numHeight->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {15, 0, 0, System::Int32::MinValue});
			this->_numHeight->Name = L"_numHeight";
			this->_numHeight->Size = System::Drawing::Size(52, 20);
			this->_numHeight->TabIndex = 11;
			this->_numHeight->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
			this->_numHeight->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numHeight_ValueChanged);
			// 
			// TerrainEditPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->_btnRamp);
			this->Controls->Add(this->_btnRelativePlateau);
			this->Controls->Add(this->_btnNoise);
			this->Controls->Add(this->_btnSmooth);
			this->Controls->Add(this->_btnPlateau);
			this->Controls->Add(this->_btnRaiseLower);
			this->Controls->Add(this->label1);
			this->Name = L"TerrainEditPanel";
			this->Size = System::Drawing::Size(392, 522);
			this->groupBox1->ResumeLayout(false);
			this->_tableLayoutBrushProperties->ResumeLayout(false);
			this->_tableLayoutBrushProperties->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numRadius))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackRadius))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHardness))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackStrength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHeight))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHardness))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numStrength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHeight))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void _btnRaiseLower_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackRadius_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numRadius_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackHardness_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numHardness_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackStrength_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numStrength_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackHeight_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numHeight_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnSmooth_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnNoise_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnPlateau_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnRelativePlateau_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnRamp_Click(System::Object^  sender, System::EventArgs^  e);
};
}
