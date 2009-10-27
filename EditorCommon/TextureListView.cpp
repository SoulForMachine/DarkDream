#include "StdAfx.h"
#include "TextureListView.h"


namespace EditorCommon
{

	System::Void TextureListView::TextureListView_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		e->Graphics->Clear(SystemColors::Window);
		e->Graphics->TranslateTransform((float)AutoScrollPosition.X, (float)AutoScrollPosition.Y);

		int i = 0;
		for each(TextureListViewItem^ item in _texList)
		{
			Point point = GetTexPosFromIndex(i);
			if(i == _selectedIndex)
			{
				Drawing::Rectangle rect(
					point - Drawing::Size(TEX_SPACING / 2, TEX_SPACING / 2),
					Drawing::Size(TEX_SIZE + TEX_SPACING, TEX_SIZE + TEX_SPACING));
				e->Graphics->FillRectangle(Brushes::DarkBlue, rect);
			}

			e->Graphics->DrawImageUnscaled(item->Texture, point);
			++i;
		}
	}

	int TextureListView::Add(TextureListViewItem^ item)
	{
		int index = _texList->Add(item);
		Point point = GetTexPosFromIndex(index);
		AutoScrollMinSize = Drawing::Size(0, point.Y + TEX_SIZE + TEX_SPACING);
		AdjustFormScrollbars(true);
		Invalidate();
		return index;
	}

	void TextureListView::Clear()
	{
		_texList->Clear();
		AutoScrollMinSize = Drawing::Size(0, 0);
		AdjustFormScrollbars(false);
		Invalidate();
	}

	System::Void TextureListView::TextureListView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		int index = GetTexIndexFromPoint(e->Location);
		if(index != _selectedIndex)
		{
			_selectedIndex = index;
			OnTextureIndexChanged(gcnew EventArgs());
			Invalidate();
		}
	}

	int TextureListView::GetTexIndexFromPoint(Point point)
	{
		int x = (point.X - TEX_SPACING) / (TEX_SIZE + TEX_SPACING);
		if(x >= _texPerRow)
			return -1;
		int y = (point.Y - TEX_SPACING) / (TEX_SIZE + TEX_SPACING);
		int index = y * _texPerRow + x;
		if(index >= 0 && index < _texList->Count)
			return index;
		else
			return -1;
	}

	Point TextureListView::GetTexPosFromIndex(int index)
	{
		int x = (index % _texPerRow) * (TEX_SIZE + TEX_SPACING) + TEX_SPACING;
		int y = (index / _texPerRow) * (TEX_SIZE + TEX_SPACING) + TEX_SPACING;
		return Point(x, y);
	}

	System::Void TextureListView::TextureListView_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		_texPerRow = Max((ClientRectangle.Width - TEX_SPACING) / (TEX_SIZE + TEX_SPACING), 1);
	}

}
