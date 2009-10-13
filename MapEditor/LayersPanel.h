#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	/// <summary>
	/// Summary for LayersPanel
	/// </summary>
	public ref class LayersPanel : public System::Windows::Forms::UserControl
	{
	public:
		LayersPanel(void)
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
		~LayersPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  _btnRemoveTexture;
	protected: 

	private: System::Windows::Forms::Button^  _btnAddTexture;
	private: System::Windows::Forms::ListBox^  _listTextures;
	protected: 


	private: System::Windows::Forms::Button^  _btnLayer4;
	private: System::Windows::Forms::Button^  _btnLayer3;
	private: System::Windows::Forms::Button^  _btnLayer2;
	private: System::Windows::Forms::Button^  _btnLayer1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  _btnBringToFront;
	private: System::Windows::Forms::Button^  _btnSendToBack;

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
			this->_btnRemoveTexture = (gcnew System::Windows::Forms::Button());
			this->_btnAddTexture = (gcnew System::Windows::Forms::Button());
			this->_listTextures = (gcnew System::Windows::Forms::ListBox());
			this->_btnLayer4 = (gcnew System::Windows::Forms::Button());
			this->_btnLayer3 = (gcnew System::Windows::Forms::Button());
			this->_btnLayer2 = (gcnew System::Windows::Forms::Button());
			this->_btnLayer1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_btnBringToFront = (gcnew System::Windows::Forms::Button());
			this->_btnSendToBack = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// _btnRemoveTexture
			// 
			this->_btnRemoveTexture->Location = System::Drawing::Point(110, 364);
			this->_btnRemoveTexture->Name = L"_btnRemoveTexture";
			this->_btnRemoveTexture->Size = System::Drawing::Size(80, 23);
			this->_btnRemoveTexture->TabIndex = 14;
			this->_btnRemoveTexture->Text = L"Remove";
			this->_btnRemoveTexture->UseVisualStyleBackColor = true;
			// 
			// _btnAddTexture
			// 
			this->_btnAddTexture->Location = System::Drawing::Point(24, 364);
			this->_btnAddTexture->Name = L"_btnAddTexture";
			this->_btnAddTexture->Size = System::Drawing::Size(80, 23);
			this->_btnAddTexture->TabIndex = 13;
			this->_btnAddTexture->Text = L"Add";
			this->_btnAddTexture->UseVisualStyleBackColor = true;
			// 
			// _listTextures
			// 
			this->_listTextures->FormattingEnabled = true;
			this->_listTextures->Location = System::Drawing::Point(24, 133);
			this->_listTextures->Name = L"_listTextures";
			this->_listTextures->Size = System::Drawing::Size(338, 225);
			this->_listTextures->TabIndex = 12;
			// 
			// _btnLayer4
			// 
			this->_btnLayer4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer4->Location = System::Drawing::Point(132, 68);
			this->_btnLayer4->Name = L"_btnLayer4";
			this->_btnLayer4->Size = System::Drawing::Size(30, 30);
			this->_btnLayer4->TabIndex = 11;
			this->_btnLayer4->Text = L"4";
			this->_btnLayer4->UseVisualStyleBackColor = true;
			// 
			// _btnLayer3
			// 
			this->_btnLayer3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer3->Location = System::Drawing::Point(96, 68);
			this->_btnLayer3->Name = L"_btnLayer3";
			this->_btnLayer3->Size = System::Drawing::Size(30, 30);
			this->_btnLayer3->TabIndex = 10;
			this->_btnLayer3->Text = L"3";
			this->_btnLayer3->UseVisualStyleBackColor = true;
			// 
			// _btnLayer2
			// 
			this->_btnLayer2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer2->Location = System::Drawing::Point(60, 68);
			this->_btnLayer2->Name = L"_btnLayer2";
			this->_btnLayer2->Size = System::Drawing::Size(30, 30);
			this->_btnLayer2->TabIndex = 9;
			this->_btnLayer2->Text = L"2";
			this->_btnLayer2->UseVisualStyleBackColor = true;
			// 
			// _btnLayer1
			// 
			this->_btnLayer1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer1->Location = System::Drawing::Point(24, 68);
			this->_btnLayer1->Name = L"_btnLayer1";
			this->_btnLayer1->Size = System::Drawing::Size(30, 30);
			this->_btnLayer1->TabIndex = 8;
			this->_btnLayer1->Text = L"1";
			this->_btnLayer1->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(20, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(62, 20);
			this->label1->TabIndex = 15;
			this->label1->Text = L"Layers";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 117);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(51, 13);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Textures:";
			// 
			// _btnBringToFront
			// 
			this->_btnBringToFront->Location = System::Drawing::Point(196, 364);
			this->_btnBringToFront->Name = L"_btnBringToFront";
			this->_btnBringToFront->Size = System::Drawing::Size(80, 23);
			this->_btnBringToFront->TabIndex = 17;
			this->_btnBringToFront->Text = L"Front";
			this->_btnBringToFront->UseVisualStyleBackColor = true;
			// 
			// _btnSendToBack
			// 
			this->_btnSendToBack->Location = System::Drawing::Point(282, 364);
			this->_btnSendToBack->Name = L"_btnSendToBack";
			this->_btnSendToBack->Size = System::Drawing::Size(80, 23);
			this->_btnSendToBack->TabIndex = 18;
			this->_btnSendToBack->Text = L"Back";
			this->_btnSendToBack->UseVisualStyleBackColor = true;
			// 
			// LayersPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->_btnSendToBack);
			this->Controls->Add(this->_btnBringToFront);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->_btnRemoveTexture);
			this->Controls->Add(this->_btnAddTexture);
			this->Controls->Add(this->_listTextures);
			this->Controls->Add(this->_btnLayer4);
			this->Controls->Add(this->_btnLayer3);
			this->Controls->Add(this->_btnLayer2);
			this->Controls->Add(this->_btnLayer1);
			this->Name = L"LayersPanel";
			this->Size = System::Drawing::Size(389, 544);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
