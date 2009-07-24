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
		EM_TerrainEdit::Parameters^ _parameters;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  _btnRaiseLower;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TableLayoutPanel^  _tableLayoutBrushProperties;

	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::TrackBar^  trackBar2;
	private: System::Windows::Forms::TrackBar^  trackBar3;
	private: System::Windows::Forms::TrackBar^  trackBar4;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown4;

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
			this->_btnRaiseLower = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_tableLayoutBrushProperties = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->trackBar4 = (gcnew System::Windows::Forms::TrackBar());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->groupBox1->SuspendLayout();
			this->_tableLayoutBrushProperties->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->BeginInit();
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
			this->_btnRaiseLower->Location = System::Drawing::Point(20, 55);
			this->_btnRaiseLower->Name = L"_btnRaiseLower";
			this->_btnRaiseLower->Size = System::Drawing::Size(112, 33);
			this->_btnRaiseLower->TabIndex = 1;
			this->_btnRaiseLower->Text = L"Raise/Lower";
			this->_btnRaiseLower->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(20, 94);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(112, 33);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Plateau";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(138, 55);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(112, 33);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Smooth";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(256, 55);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(112, 33);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Noise";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(138, 94);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(112, 33);
			this->button4->TabIndex = 5;
			this->button4->Text = L"Relative Plateau";
			this->button4->UseVisualStyleBackColor = true;
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(256, 94);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(112, 33);
			this->button5->TabIndex = 6;
			this->button5->Text = L"Ramp";
			this->button5->UseVisualStyleBackColor = true;
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
				25.15924F)));
			this->_tableLayoutBrushProperties->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				74.84076F)));
			this->_tableLayoutBrushProperties->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				45)));
			this->_tableLayoutBrushProperties->Controls->Add(this->numericUpDown1, 2, 0);
			this->_tableLayoutBrushProperties->Controls->Add(this->label2, 0, 0);
			this->_tableLayoutBrushProperties->Controls->Add(this->label3, 0, 1);
			this->_tableLayoutBrushProperties->Controls->Add(this->label4, 0, 2);
			this->_tableLayoutBrushProperties->Controls->Add(this->trackBar1, 1, 0);
			this->_tableLayoutBrushProperties->Controls->Add(this->trackBar2, 1, 1);
			this->_tableLayoutBrushProperties->Controls->Add(this->trackBar3, 1, 2);
			this->_tableLayoutBrushProperties->Controls->Add(this->label5, 0, 3);
			this->_tableLayoutBrushProperties->Controls->Add(this->trackBar4, 1, 3);
			this->_tableLayoutBrushProperties->Controls->Add(this->numericUpDown2, 2, 1);
			this->_tableLayoutBrushProperties->Controls->Add(this->numericUpDown3, 2, 2);
			this->_tableLayoutBrushProperties->Controls->Add(this->numericUpDown4, 2, 3);
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
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(293, 3);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(37, 20);
			this->numericUpDown1->TabIndex = 5;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Dock = System::Windows::Forms::DockStyle::Left;
			this->label2->Location = System::Drawing::Point(3, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 25);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Radius:";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Dock = System::Windows::Forms::DockStyle::Left;
			this->label3->Location = System::Drawing::Point(3, 25);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 25);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Hardness:";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Dock = System::Windows::Forms::DockStyle::Left;
			this->label4->Location = System::Drawing::Point(3, 50);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(50, 25);
			this->label4->TabIndex = 2;
			this->label4->Text = L"Strength:";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(76, 3);
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(211, 19);
			this->trackBar1->TabIndex = 4;
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(76, 28);
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(211, 19);
			this->trackBar2->TabIndex = 5;
			// 
			// trackBar3
			// 
			this->trackBar3->Location = System::Drawing::Point(76, 53);
			this->trackBar3->Name = L"trackBar3";
			this->trackBar3->Size = System::Drawing::Size(211, 19);
			this->trackBar3->TabIndex = 6;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Dock = System::Windows::Forms::DockStyle::Left;
			this->label5->Location = System::Drawing::Point(3, 75);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(41, 25);
			this->label5->TabIndex = 3;
			this->label5->Text = L"Height:";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// trackBar4
			// 
			this->trackBar4->Location = System::Drawing::Point(76, 78);
			this->trackBar4->Name = L"trackBar4";
			this->trackBar4->Size = System::Drawing::Size(211, 19);
			this->trackBar4->TabIndex = 7;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(293, 28);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(37, 20);
			this->numericUpDown2->TabIndex = 8;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(293, 53);
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(37, 20);
			this->numericUpDown3->TabIndex = 9;
			// 
			// numericUpDown4
			// 
			this->numericUpDown4->Location = System::Drawing::Point(293, 78);
			this->numericUpDown4->Name = L"numericUpDown4";
			this->numericUpDown4->Size = System::Drawing::Size(37, 20);
			this->numericUpDown4->TabIndex = 10;
			// 
			// TerrainEditPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->_btnRaiseLower);
			this->Controls->Add(this->label1);
			this->Name = L"TerrainEditPanel";
			this->Size = System::Drawing::Size(393, 522);
			this->groupBox1->ResumeLayout(false);
			this->_tableLayoutBrushProperties->ResumeLayout(false);
			this->_tableLayoutBrushProperties->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
