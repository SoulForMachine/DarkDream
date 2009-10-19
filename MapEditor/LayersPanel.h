#pragma once

#include "EM_LayerEdit.h"

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
		LayersPanel(EM_LayerEdit^ edit_mode);
		void SelectSprite(Engine::BgLayer::Sprite* sprite);

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


	private: System::Windows::Forms::RadioButton^  _btnLayer4;
	private: System::Windows::Forms::RadioButton^  _btnLayer3;
	private: System::Windows::Forms::RadioButton^  _btnLayer2;
	private: System::Windows::Forms::RadioButton^  _btnLayer1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  _btnBringToFront;
	private: System::Windows::Forms::Button^  _btnSendToBack;
	private: System::Windows::Forms::OpenFileDialog^  _selectTextureDialog;
	private: System::Windows::Forms::CheckBox^  _checkTileTexU;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TrackBar^  _trackScrollFactor;
	private: System::Windows::Forms::NumericUpDown^  _textScrollFactor;
	private: System::Windows::Forms::NumericUpDown^  _textScale;


	private: System::Windows::Forms::TrackBar^  _trackScale;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::CheckBox^  _checkTileTexV;
	private: System::Windows::Forms::NumericUpDown^  _textTile;

	private: System::Windows::Forms::TrackBar^  _trackTile;

	private: System::Windows::Forms::Label^  label5;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		void RefreshTextureList();
		void AddSpriteToList(Engine::BgLayer::Sprite* sprite);
		void UpdateScrollFactorControls();
		void UpdateScaleControls();
		void UpdateControlState();
		void SetInitialSpriteRect(Engine::BgLayer::Sprite* sprite);
		void SetSpriteScale(Engine::BgLayer::Sprite* sprite, float scale);

		EM_LayerEdit^ _editMode;

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
			this->_btnLayer4 = (gcnew System::Windows::Forms::RadioButton());
			this->_btnLayer3 = (gcnew System::Windows::Forms::RadioButton());
			this->_btnLayer2 = (gcnew System::Windows::Forms::RadioButton());
			this->_btnLayer1 = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->_btnBringToFront = (gcnew System::Windows::Forms::Button());
			this->_btnSendToBack = (gcnew System::Windows::Forms::Button());
			this->_selectTextureDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_checkTileTexU = (gcnew System::Windows::Forms::CheckBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->_trackScrollFactor = (gcnew System::Windows::Forms::TrackBar());
			this->_textScrollFactor = (gcnew System::Windows::Forms::NumericUpDown());
			this->_textScale = (gcnew System::Windows::Forms::NumericUpDown());
			this->_trackScale = (gcnew System::Windows::Forms::TrackBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->_checkTileTexV = (gcnew System::Windows::Forms::CheckBox());
			this->_textTile = (gcnew System::Windows::Forms::NumericUpDown());
			this->_trackTile = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackScrollFactor))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textScrollFactor))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textScale))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackScale))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textTile))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackTile))->BeginInit();
			this->SuspendLayout();
			// 
			// _btnRemoveTexture
			// 
			this->_btnRemoveTexture->Location = System::Drawing::Point(110, 406);
			this->_btnRemoveTexture->Name = L"_btnRemoveTexture";
			this->_btnRemoveTexture->Size = System::Drawing::Size(80, 23);
			this->_btnRemoveTexture->TabIndex = 14;
			this->_btnRemoveTexture->Text = L"Remove";
			this->_btnRemoveTexture->UseVisualStyleBackColor = true;
			this->_btnRemoveTexture->Click += gcnew System::EventHandler(this, &LayersPanel::_btnRemoveTexture_Click);
			// 
			// _btnAddTexture
			// 
			this->_btnAddTexture->Location = System::Drawing::Point(24, 406);
			this->_btnAddTexture->Name = L"_btnAddTexture";
			this->_btnAddTexture->Size = System::Drawing::Size(80, 23);
			this->_btnAddTexture->TabIndex = 13;
			this->_btnAddTexture->Text = L"Add";
			this->_btnAddTexture->UseVisualStyleBackColor = true;
			this->_btnAddTexture->Click += gcnew System::EventHandler(this, &LayersPanel::_btnAddTexture_Click);
			// 
			// _listTextures
			// 
			this->_listTextures->FormattingEnabled = true;
			this->_listTextures->Location = System::Drawing::Point(24, 175);
			this->_listTextures->Name = L"_listTextures";
			this->_listTextures->Size = System::Drawing::Size(338, 225);
			this->_listTextures->TabIndex = 12;
			this->_listTextures->SelectedIndexChanged += gcnew System::EventHandler(this, &LayersPanel::_listTextures_SelectedIndexChanged);
			// 
			// _btnLayer4
			// 
			this->_btnLayer4->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnLayer4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer4->Location = System::Drawing::Point(132, 68);
			this->_btnLayer4->Name = L"_btnLayer4";
			this->_btnLayer4->Size = System::Drawing::Size(30, 30);
			this->_btnLayer4->TabIndex = 11;
			this->_btnLayer4->Text = L"4";
			this->_btnLayer4->UseVisualStyleBackColor = true;
			this->_btnLayer4->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LayersPanel::_btnLayer4_MouseClick);
			// 
			// _btnLayer3
			// 
			this->_btnLayer3->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnLayer3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer3->Location = System::Drawing::Point(96, 68);
			this->_btnLayer3->Name = L"_btnLayer3";
			this->_btnLayer3->Size = System::Drawing::Size(30, 30);
			this->_btnLayer3->TabIndex = 10;
			this->_btnLayer3->Text = L"3";
			this->_btnLayer3->UseVisualStyleBackColor = true;
			this->_btnLayer3->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LayersPanel::_btnLayer3_MouseClick);
			// 
			// _btnLayer2
			// 
			this->_btnLayer2->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnLayer2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer2->Location = System::Drawing::Point(60, 68);
			this->_btnLayer2->Name = L"_btnLayer2";
			this->_btnLayer2->Size = System::Drawing::Size(30, 30);
			this->_btnLayer2->TabIndex = 9;
			this->_btnLayer2->Text = L"2";
			this->_btnLayer2->UseVisualStyleBackColor = true;
			this->_btnLayer2->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LayersPanel::_btnLayer2_MouseClick);
			// 
			// _btnLayer1
			// 
			this->_btnLayer1->Appearance = System::Windows::Forms::Appearance::Button;
			this->_btnLayer1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->_btnLayer1->Location = System::Drawing::Point(24, 68);
			this->_btnLayer1->Name = L"_btnLayer1";
			this->_btnLayer1->Size = System::Drawing::Size(30, 30);
			this->_btnLayer1->TabIndex = 8;
			this->_btnLayer1->Text = L"1";
			this->_btnLayer1->UseVisualStyleBackColor = true;
			this->_btnLayer1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LayersPanel::_btnLayer1_MouseClick);
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
			this->label2->Location = System::Drawing::Point(21, 159);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(51, 13);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Textures:";
			// 
			// _btnBringToFront
			// 
			this->_btnBringToFront->Location = System::Drawing::Point(196, 406);
			this->_btnBringToFront->Name = L"_btnBringToFront";
			this->_btnBringToFront->Size = System::Drawing::Size(80, 23);
			this->_btnBringToFront->TabIndex = 17;
			this->_btnBringToFront->Text = L"Front";
			this->_btnBringToFront->UseVisualStyleBackColor = true;
			this->_btnBringToFront->Click += gcnew System::EventHandler(this, &LayersPanel::_btnBringToFront_Click);
			// 
			// _btnSendToBack
			// 
			this->_btnSendToBack->Location = System::Drawing::Point(282, 406);
			this->_btnSendToBack->Name = L"_btnSendToBack";
			this->_btnSendToBack->Size = System::Drawing::Size(80, 23);
			this->_btnSendToBack->TabIndex = 18;
			this->_btnSendToBack->Text = L"Back";
			this->_btnSendToBack->UseVisualStyleBackColor = true;
			this->_btnSendToBack->Click += gcnew System::EventHandler(this, &LayersPanel::_btnSendToBack_Click);
			// 
			// _selectTextureDialog
			// 
			this->_selectTextureDialog->Filter = L"Texture Files (*.dds;*.tga;*.bmp)|*.dds;*.tga;*.bmp|All Files (*.*)|*.*";
			this->_selectTextureDialog->Title = L"Select Texture";
			this->_selectTextureDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &LayersPanel::_selectTextureDialog_FileOk);
			// 
			// _checkTileTexU
			// 
			this->_checkTileTexU->AutoSize = true;
			this->_checkTileTexU->Location = System::Drawing::Point(24, 460);
			this->_checkTileTexU->Name = L"_checkTileTexU";
			this->_checkTileTexU->Size = System::Drawing::Size(54, 17);
			this->_checkTileTexU->TabIndex = 19;
			this->_checkTileTexU->Text = L"Tile U";
			this->_checkTileTexU->UseVisualStyleBackColor = true;
			this->_checkTileTexU->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LayersPanel::_checkTileTexU_MouseClick);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(21, 126);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(69, 13);
			this->label3->TabIndex = 20;
			this->label3->Text = L"Scroll Factor:";
			// 
			// _trackScrollFactor
			// 
			this->_trackScrollFactor->AutoSize = false;
			this->_trackScrollFactor->Location = System::Drawing::Point(96, 124);
			this->_trackScrollFactor->Maximum = 100;
			this->_trackScrollFactor->Name = L"_trackScrollFactor";
			this->_trackScrollFactor->Size = System::Drawing::Size(200, 20);
			this->_trackScrollFactor->TabIndex = 21;
			this->_trackScrollFactor->TickFrequency = 5;
			this->_trackScrollFactor->Scroll += gcnew System::EventHandler(this, &LayersPanel::_trackScrollFactor_Scroll);
			// 
			// _textScrollFactor
			// 
			this->_textScrollFactor->DecimalPlaces = 2;
			this->_textScrollFactor->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->_textScrollFactor->Location = System::Drawing::Point(302, 124);
			this->_textScrollFactor->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_textScrollFactor->Name = L"_textScrollFactor";
			this->_textScrollFactor->Size = System::Drawing::Size(60, 20);
			this->_textScrollFactor->TabIndex = 22;
			this->_textScrollFactor->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textScrollFactor_ValueChanged);
			// 
			// _textScale
			// 
			this->_textScale->DecimalPlaces = 2;
			this->_textScale->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->_textScale->Location = System::Drawing::Point(302, 529);
			this->_textScale->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {4, 0, 0, 0});
			this->_textScale->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_textScale->Name = L"_textScale";
			this->_textScale->Size = System::Drawing::Size(60, 20);
			this->_textScale->TabIndex = 25;
			this->_textScale->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
			this->_textScale->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textScale_ValueChanged);
			// 
			// _trackScale
			// 
			this->_trackScale->AutoSize = false;
			this->_trackScale->LargeChange = 25;
			this->_trackScale->Location = System::Drawing::Point(96, 529);
			this->_trackScale->Maximum = 400;
			this->_trackScale->Minimum = 10;
			this->_trackScale->Name = L"_trackScale";
			this->_trackScale->Size = System::Drawing::Size(200, 20);
			this->_trackScale->TabIndex = 24;
			this->_trackScale->TickFrequency = 25;
			this->_trackScale->Value = 10;
			this->_trackScale->Scroll += gcnew System::EventHandler(this, &LayersPanel::_trackScale_Scroll);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(21, 531);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(37, 13);
			this->label4->TabIndex = 23;
			this->label4->Text = L"Scale:";
			// 
			// _checkTileTexV
			// 
			this->_checkTileTexV->AutoSize = true;
			this->_checkTileTexV->Location = System::Drawing::Point(110, 460);
			this->_checkTileTexV->Name = L"_checkTileTexV";
			this->_checkTileTexV->Size = System::Drawing::Size(53, 17);
			this->_checkTileTexV->TabIndex = 26;
			this->_checkTileTexV->Text = L"Tile V";
			this->_checkTileTexV->UseVisualStyleBackColor = true;
			this->_checkTileTexV->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LayersPanel::_checkTileTexV_MouseClick);
			// 
			// _textTile
			// 
			this->_textTile->DecimalPlaces = 2;
			this->_textTile->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
			this->_textTile->Location = System::Drawing::Point(302, 493);
			this->_textTile->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			this->_textTile->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_textTile->Name = L"_textTile";
			this->_textTile->Size = System::Drawing::Size(60, 20);
			this->_textTile->TabIndex = 29;
			this->_textTile->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->_textTile->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textTile_ValueChanged);
			// 
			// _trackTile
			// 
			this->_trackTile->AutoSize = false;
			this->_trackTile->LargeChange = 100;
			this->_trackTile->Location = System::Drawing::Point(96, 493);
			this->_trackTile->Maximum = 1000;
			this->_trackTile->Minimum = 100;
			this->_trackTile->Name = L"_trackTile";
			this->_trackTile->Size = System::Drawing::Size(200, 20);
			this->_trackTile->TabIndex = 28;
			this->_trackTile->TickFrequency = 50;
			this->_trackTile->Value = 100;
			this->_trackTile->Scroll += gcnew System::EventHandler(this, &LayersPanel::_trackTile_Scroll);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(21, 495);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(27, 13);
			this->label5->TabIndex = 27;
			this->label5->Text = L"Tile:";
			// 
			// LayersPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->_textTile);
			this->Controls->Add(this->_trackTile);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->_checkTileTexV);
			this->Controls->Add(this->_textScale);
			this->Controls->Add(this->_trackScale);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->_textScrollFactor);
			this->Controls->Add(this->_trackScrollFactor);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->_checkTileTexU);
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
			this->Size = System::Drawing::Size(389, 617);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackScrollFactor))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textScrollFactor))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textScale))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackScale))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_textTile))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_trackTile))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void _selectTextureDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
	private: System::Void _btnLayer1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _btnLayer2_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _btnLayer3_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _btnLayer4_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _listTextures_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnAddTexture_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnRemoveTexture_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnBringToFront_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _btnSendToBack_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _checkTileTexU_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _checkTileTexV_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void _trackScrollFactor_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _textScrollFactor_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackScale_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _textScale_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _trackTile_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _textTile_ValueChanged(System::Object^  sender, System::EventArgs^  e);
};
}
