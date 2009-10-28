#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections;


namespace EditorCommon {


	ref class TextureListViewItem;


	/// <summary>
	/// Summary for TextureListView
	/// </summary>
	public ref class TextureListView : public System::Windows::Forms::UserControl
	{
	public:
		TextureListView(void)
		{
			InitializeComponent();

			_texList = gcnew ArrayList;
			_selectedIndex = -1;
			_texSize = 128;
			CalcTexPerRow();
		}

		int Add(TextureListViewItem^ item);
		void Clear();

		property int SelectedIndex
		{
			int get()
				{ return _selectedIndex; }
		}

		property int TextureSize
		{
			int get()
				{ return _texSize; }
			void set(int val)
			{
				_texSize = val;
				CalcTexPerRow();
			}
		}

		property int TextureCount
		{
			int get()
				{ return _texList->Count; }
		}

		event EventHandler^ TextureIndexChanged;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TextureListView()
		{
			if (components)
			{
				delete components;
			}
		}

		virtual void OnTextureIndexChanged(EventArgs^ e)
		{
			TextureIndexChanged(this, e);
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		int GetTexIndexFromPoint(Point point);
		Point GetTexPosFromIndex(int index);
		void CalcTexPerRow();

		static const int TEX_SPACING = 10;

		ArrayList^ _texList;
		int _selectedIndex;
		int _texPerRow;
		int _texSize;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// TextureListView
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->DoubleBuffered = true;
			this->Name = L"TextureListView";
			this->Size = System::Drawing::Size(339, 478);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &TextureListView::TextureListView_Paint);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &TextureListView::TextureListView_MouseDown);
			this->Resize += gcnew System::EventHandler(this, &TextureListView::TextureListView_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void TextureListView_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		private: System::Void TextureListView_Resize(System::Object^  sender, System::EventArgs^  e);
		private: System::Void TextureListView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	};


	public ref class TextureListViewItem
	{
	public:
		TextureListViewItem(Bitmap^ bitmap)
		{
			_bitmap = bitmap;
		}

		property Bitmap^ Texture
		{
			Bitmap^ get()
				{ return _bitmap; }
			void set(Bitmap^ val)
				{ _bitmap = val; }
		}

	private:
		Bitmap^ _bitmap;
	};

}
