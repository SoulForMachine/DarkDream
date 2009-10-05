#pragma once

#include "EM_View.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	/// <summary>
	/// Summary for ViewPanel
	/// </summary>
	public ref class ViewPanel : public System::Windows::Forms::UserControl
	{
	public:
		ViewPanel(EM_View::Parameters^ params);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ViewPanel()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		EM_View::Parameters^ _parameters;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  _numFOV;


	private: System::Windows::Forms::NumericUpDown^  _numDistance;

	private: System::Windows::Forms::NumericUpDown^  _numHeight;
	private: System::Windows::Forms::TrackBar^  _trackFOV;




	private: System::Windows::Forms::TrackBar^  _trackDistance;

	private: System::Windows::Forms::TrackBar^  _trackHeight;



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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->_numFOV = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numDistance = (gcnew System::Windows::Forms::NumericUpDown());
			this->_numHeight = (gcnew System::Windows::Forms::NumericUpDown());
			this->_trackFOV = (gcnew System::Windows::Forms::TrackBar());
			this->_trackDistance = (gcnew System::Windows::Forms::TrackBar());
			this->_trackHeight = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numFOV))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numDistance))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHeight))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackFOV))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackDistance))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHeight))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->_numFOV);
			this->groupBox1->Controls->Add(this->_numDistance);
			this->groupBox1->Controls->Add(this->_numHeight);
			this->groupBox1->Controls->Add(this->_trackFOV);
			this->groupBox1->Controls->Add(this->_trackDistance);
			this->groupBox1->Controls->Add(this->_trackHeight);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Location = System::Drawing::Point(25, 61);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(336, 186);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Game view camera properties";
			// 
			// _numFOV
			// 
			this->_numFOV->Location = System::Drawing::Point(266, 120);
			this->_numFOV->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {90, 0, 0, 0});
			this->_numFOV->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {45, 0, 0, 0});
			this->_numFOV->Name = L"_numFOV";
			this->_numFOV->Size = System::Drawing::Size(49, 20);
			this->_numFOV->TabIndex = 11;
			this->_numFOV->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {45, 0, 0, 0});
			this->_numFOV->ValueChanged += gcnew System::EventHandler(this, &ViewPanel::_numFOV_ValueChanged);
			// 
			// _numDistance
			// 
			this->_numDistance->DecimalPlaces = 1;
			this->_numDistance->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_numDistance->Location = System::Drawing::Point(266, 76);
			this->_numDistance->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->_numDistance->Name = L"_numDistance";
			this->_numDistance->Size = System::Drawing::Size(49, 20);
			this->_numDistance->TabIndex = 10;
			this->_numDistance->ValueChanged += gcnew System::EventHandler(this, &ViewPanel::_numDistance_ValueChanged);
			// 
			// _numHeight
			// 
			this->_numHeight->DecimalPlaces = 1;
			this->_numHeight->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_numHeight->Location = System::Drawing::Point(266, 37);
			this->_numHeight->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->_numHeight->Name = L"_numHeight";
			this->_numHeight->Size = System::Drawing::Size(49, 20);
			this->_numHeight->TabIndex = 9;
			this->_numHeight->ValueChanged += gcnew System::EventHandler(this, &ViewPanel::_numHeight_ValueChanged);
			// 
			// _trackFOV
			// 
			this->_trackFOV->AutoSize = false;
			this->_trackFOV->Location = System::Drawing::Point(71, 120);
			this->_trackFOV->Maximum = 90;
			this->_trackFOV->Minimum = 45;
			this->_trackFOV->Name = L"_trackFOV";
			this->_trackFOV->Size = System::Drawing::Size(189, 20);
			this->_trackFOV->TabIndex = 7;
			this->_trackFOV->TickFrequency = 5;
			this->_trackFOV->Value = 45;
			this->_trackFOV->Scroll += gcnew System::EventHandler(this, &ViewPanel::_trackFOV_Scroll);
			// 
			// _trackDistance
			// 
			this->_trackDistance->AutoSize = false;
			this->_trackDistance->Location = System::Drawing::Point(71, 76);
			this->_trackDistance->Maximum = 100;
			this->_trackDistance->Name = L"_trackDistance";
			this->_trackDistance->Size = System::Drawing::Size(189, 20);
			this->_trackDistance->TabIndex = 6;
			this->_trackDistance->TickFrequency = 5;
			this->_trackDistance->Scroll += gcnew System::EventHandler(this, &ViewPanel::_trackDistance_Scroll);
			// 
			// _trackHeight
			// 
			this->_trackHeight->AutoSize = false;
			this->_trackHeight->Location = System::Drawing::Point(71, 37);
			this->_trackHeight->Maximum = 100;
			this->_trackHeight->Name = L"_trackHeight";
			this->_trackHeight->Size = System::Drawing::Size(189, 20);
			this->_trackHeight->TabIndex = 5;
			this->_trackHeight->TickFrequency = 5;
			this->_trackHeight->Scroll += gcnew System::EventHandler(this, &ViewPanel::_trackHeight_Scroll);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(15, 124);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(31, 13);
			this->label5->TabIndex = 3;
			this->label5->Text = L"FOV:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(15, 80);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(52, 13);
			this->label4->TabIndex = 2;
			this->label4->Text = L"Distance:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(15, 41);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Height:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(21, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 20);
			this->label1->TabIndex = 1;
			this->label1->Text = L"View";
			// 
			// ViewPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->label1);
			this->Controls->Add(this->groupBox1);
			this->Name = L"ViewPanel";
			this->Size = System::Drawing::Size(391, 484);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numFOV))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numDistance))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_numHeight))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackFOV))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackDistance))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackHeight))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void _trackHeight_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numHeight_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackDistance_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numDistance_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackFOV_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _numFOV_ValueChanged(System::Object^  sender, System::EventArgs^  e);
};
}
