#include "StdAfx.h"
#include "PropertiesPanel.h"

#define GRAPH_MARGIN		20
#define GRAPH_LEFT_MARGIN	40
#define GRAPH_PT_RECT_WIDTH	10


using namespace System::Drawing::Drawing2D;
using namespace System::Windows;
using namespace Engine;
using namespace Memory;
using namespace math3d;


namespace ParticleEditor
{

	PropertiesPanel::PropertiesPanel(void)
	{
		InitializeComponent();

		_particleSystem = 0;
		_attribute = 0;
		_graphTime = 15.0f;
		_minValue = 0.0f;
		_maxValue = 100.0f;
		_selecting = false;
		_moving = false;
		_selectedPoints = gcnew Generic::List<GraphPoint>;

		_penThickBlack = gcnew Pen(Color::Black, 2.0f);

		_labelAttribName->Text = "";

		System::Reflection::MethodInfo^ method = Control::typeid->GetMethod(
               "SetStyle",
			   Reflection::BindingFlags::NonPublic | 
			   Reflection::BindingFlags::Instance);
		array<Object^, 1>^ params = gcnew array<Object^, 1>(2);
		params[0] = ControlStyles::AllPaintingInWmPaint | ControlStyles::OptimizedDoubleBuffer;
		params[1] = true;
		method->Invoke(_panelGraph, params);

		UpdateValueFields();
	}

	void PropertiesPanel::SetParticleSystem(Engine::ParticleSystem* part_sys)
	{
		_particleSystem = part_sys;

		if(part_sys)
		{
			const AABBox& bbox = part_sys->GetObjectBoundingBox();
			_numBBoxWidth->Text = bbox.GetWidth().ToString();
			_numBBoxHeight->Text = bbox.GetHeight().ToString();
			_numBBoxDepth->Text = bbox.GetDepth().ToString();
		}
	}

	void PropertiesPanel::SetGraphAttribute(ParticleSystem::Attribute* attrib, String^ name, float time, float min_val, float max_val)
	{
		_attribute = attrib;
		_graphTime = time;
		_minValue = min_val;
		_maxValue = max_val;
		_labelAttribName->Text = name;
		_selectedPoints->Clear();
		_numTime->Maximum = (Decimal)time;
		_numValue->Minimum = (Decimal)min_val;
		_numValue->Maximum = (Decimal)max_val;
		UpdateValueFields();
		_panelGraph->Invalidate();
	}

	System::Void PropertiesPanel::_panelGraph_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		int cl_width = _panelGraph->ClientRectangle.Width;
		int cl_height = _panelGraph->ClientRectangle.Height;
		e->Graphics->Clear(Color::White);
		e->Graphics->ScaleTransform(1.0f, -1.0f);
		e->Graphics->TranslateTransform(0.0f, - (float)cl_height);

		// draw graph
		float step = _graphWidth / 10.0f;
		float x = GRAPH_LEFT_MARGIN + step;
		for(int i = 0; i < 9; ++i)
		{
			e->Graphics->DrawLine(Pens::LightGray, (int)x, GRAPH_MARGIN, (int)x, GRAPH_MARGIN + _graphHeight);
			x += step;
		}

		step = (float)_graphHeight / 4;
		float y = GRAPH_MARGIN + step;
		for(int i = 0; i < 3; ++i)
		{
			e->Graphics->DrawLine(Pens::LightGray, GRAPH_LEFT_MARGIN, (int)y, GRAPH_LEFT_MARGIN + _graphWidth, (int)y);
			y += step;
		}
		e->Graphics->DrawRectangle(Pens::Black, GRAPH_LEFT_MARGIN, GRAPH_MARGIN, _graphWidth, _graphHeight);

		if(_attribute)
		{
			// zero line
			float range = _maxValue - _minValue;
			float ny = GRAPH_MARGIN + (- _minValue) / range * _graphHeight;
			e->Graphics->DrawLine(Pens::Red, GRAPH_LEFT_MARGIN + 1, (int)ny, GRAPH_LEFT_MARGIN + _graphWidth - 1, (int)ny);

			GraphicsState^ state = e->Graphics->Save();
			e->Graphics->ResetTransform();

			/*x = GRAPH_LEFT_MARGIN;
			float val = 0.0f;
			float val_step = 
			for(int i = 0; i <= 10; ++i)
			{
				String^ text = (i * 10).ToString("f1");
				SizeF size = e->Graphics->MeasureString(text, _panelGraph->Font);
				e->Graphics->DrawString(text, _panelGraph->Font, Brushes::Black, x - size.Width / 2, (float)cl_height - (GRAPH_MARGIN - 2));
				x += step;
			}*/

			String^ text = "0";
			SizeF size = e->Graphics->MeasureString(text, _panelGraph->Font);
			e->Graphics->DrawString(text, _panelGraph->Font, Brushes::Black, (float)GRAPH_LEFT_MARGIN - size.Width / 2, (float)GRAPH_MARGIN + _graphHeight + 2);

			text = _graphTime.ToString("f1");
			size = e->Graphics->MeasureString(text, _panelGraph->Font);
			e->Graphics->DrawString(text, _panelGraph->Font, Brushes::Black, (float)GRAPH_LEFT_MARGIN + _graphWidth - size.Width / 2, (float)GRAPH_MARGIN + _graphHeight + 2);

			text = _minValue.ToString("f1");
			size = e->Graphics->MeasureString(text, _panelGraph->Font);
			e->Graphics->DrawString(text, _panelGraph->Font, Brushes::Black, GRAPH_LEFT_MARGIN - size.Width, (float)cl_height - GRAPH_MARGIN - _panelGraph->Font->Height / 2);

			/*float y = GRAPH_MARGIN;
			float val = _minValue;
			float val_step = (_maxValue - _minValue
			int count = _graphHeight / 50 - 1;
			for(int i = 0; i < count; ++i)
			{
			}*/

			text = _maxValue.ToString("f1");
			size = e->Graphics->MeasureString(text, _panelGraph->Font);
			e->Graphics->DrawString(text, _panelGraph->Font, Brushes::Black, GRAPH_LEFT_MARGIN - size.Width, (float)GRAPH_MARGIN - _panelGraph->Font->Height / 2);

			e->Graphics->Restore(state);

			int count = _attribute->GetValueCount() - 1;
			const ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			for(int i = 0; i < count; ++i)
			{
				float x1 = GRAPH_LEFT_MARGIN + values[i].variable * _graphWidth;
				float y1 = GRAPH_MARGIN + (values[i].value - _minValue) / range * _graphHeight;
				float x2 = GRAPH_LEFT_MARGIN + values[i + 1].variable * _graphWidth;
				float y2 = GRAPH_MARGIN + (values[i + 1].value - _minValue) / range * _graphHeight;

				e->Graphics->DrawLine(_penThickBlack, x1, y1, x2, y2);

				Brush^ brush = IsPointSelected(i)? Brushes::Red: Brushes::Black;
				e->Graphics->FillRectangle(brush, (int)x1 - GRAPH_PT_RECT_WIDTH / 2, (int)y1 - GRAPH_PT_RECT_WIDTH / 2, GRAPH_PT_RECT_WIDTH, GRAPH_PT_RECT_WIDTH);

				if(i == count - 1)
				{
					Brush^ brush = IsPointSelected(i + 1)? Brushes::Red: Brushes::Black;
					e->Graphics->FillRectangle(brush, (int)x2 - GRAPH_PT_RECT_WIDTH / 2, (int)y2 - GRAPH_PT_RECT_WIDTH / 2, GRAPH_PT_RECT_WIDTH, GRAPH_PT_RECT_WIDTH);
				}
			}

			// selection rectangle
			if(_selecting)
				e->Graphics->DrawRectangle(Pens::DarkBlue, _selectRect);
		}
	}

	System::Void PropertiesPanel::_panelGraph_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		_graphWidth = _panelGraph->ClientRectangle.Width - GRAPH_MARGIN - GRAPH_LEFT_MARGIN;
		_graphHeight = _panelGraph->ClientRectangle.Height - 2 * GRAPH_MARGIN;
		_panelGraph->Invalidate();
	}

	System::Void PropertiesPanel::_panelGraph_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!_attribute)
			return;

		int y = _panelGraph->ClientRectangle.Height - e->Y;
		int index = PointIndexFromPos(e->X, y);

		if(e->Button == Forms::MouseButtons::Left)
		{
			if(index != -1)
			{
				_panelGraph->Capture = true;
				if(!IsPointSelected(index))
				{
					_selectedPoints->Clear();
					SelectPoint(index);
					_panelGraph->Invalidate();
					UpdateValueFields();
				}
				_moving = true;
				_mouseStart.X = e->X;
				_mouseStart.Y = y;
			}
			else
			{
				_panelGraph->Capture = true;
				_selecting = true;
				_mouseStart.X = e->X;
				_mouseStart.Y = y;
				_selectRect.X = e->X;
				_selectRect.Y = y;
				_selectRect.Width = 0;
				_selectRect.Height = 0;
			}
		}
		else if(e->Button == Forms::MouseButtons::Right)
		{
			if(index != -1)
			{
				RemovePoint(index);
			}
			else
			{
				AddNewPoint(e->X, y);
			}
		}
	}

	System::Void PropertiesPanel::_panelGraph_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!_attribute)
			return;

		if(_selecting)
		{
			int y = _panelGraph->ClientRectangle.Height - e->Y;
			Point min_pt, max_pt;
			min_pt.X = Math::Min(_mouseStart.X, e->X);
			min_pt.Y = Math::Min(_mouseStart.Y, y);
			max_pt.X = Math::Max(_mouseStart.X, e->X);
			max_pt.Y = Math::Max(_mouseStart.Y, y);
			_selectRect.X = min_pt.X;
			_selectRect.Y = min_pt.Y;
			_selectRect.Width = max_pt.X - min_pt.X;
			_selectRect.Height = max_pt.Y - min_pt.Y;

			_panelGraph->Invalidate();
		}
		else if(_moving)
		{
			float range = _maxValue - _minValue;
			ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			int count = _attribute->GetValueCount();
			for each(GraphPoint pt in _selectedPoints)
			{
				if(pt.index > 0 && pt.index < count - 1)
				{
					float dx = float(e->X - _mouseStart.X) / _graphWidth;
					values[pt.index].variable = pt.x + dx;
					math3d::clamp(values[pt.index].variable, values[pt.index - 1].variable, values[pt.index + 1].variable);
				}

				float dy = float((_panelGraph->ClientRectangle.Height - e->Y) - _mouseStart.Y) / _graphHeight * range;
				values[pt.index].value = pt.y + dy;
				math3d::clamp(values[pt.index].value, _minValue, _maxValue);
			}

			UpdateValueFields();
			_panelGraph->Invalidate();
		}
	}

	System::Void PropertiesPanel::_panelGraph_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_selecting)
		{
			_selecting = false;
			_panelGraph->Capture = false;

			_selectedPoints->Clear();
			float range = _maxValue - _minValue;
			const ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			int count = _attribute->GetValueCount();
			for(int i = 0; i < count; ++i)
			{
				int x = int(GRAPH_LEFT_MARGIN + values[i].variable * _graphWidth);
				int y = int(GRAPH_MARGIN + (values[i].value - _minValue) / range * _graphHeight);
				if(_selectRect.Contains(x, y))
				{
					SelectPoint(i);
				}
			}
			UpdateValueFields();

			_panelGraph->Invalidate();
		}
		else if(_moving)
		{
			_moving = false;
			_panelGraph->Capture = false;

			// update selected points to new values
			const ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			for(int i = 0; i < _selectedPoints->Count; ++i)
			{
				int index = _selectedPoints[i].index;
				GraphPoint pt = { index , values[index].variable, values[index].value };
				_selectedPoints[i] = pt;
			}
		}
	}

	System::Void PropertiesPanel::_btnRun_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSystem)
			_particleSystem->Reset();
	}

	System::Void PropertiesPanel::_checkPause_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void PropertiesPanel::_numBBoxWidth_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSystem)
		{
			AABBox bbox = _particleSystem->GetObjectBoundingBox();
			bbox.minPt.x = -Decimal::ToSingle(_numBBoxWidth->Value) * 0.5f;
			bbox.maxPt.x = Decimal::ToSingle(_numBBoxWidth->Value) * 0.5f;
			_particleSystem->SetObjectBoundingBox(bbox);
		}
	}

	System::Void PropertiesPanel::_numBBoxHeight_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSystem)
		{
			AABBox bbox = _particleSystem->GetObjectBoundingBox();
			bbox.minPt.y = -Decimal::ToSingle(_numBBoxHeight->Value) * 0.5f;
			bbox.maxPt.y = Decimal::ToSingle(_numBBoxHeight->Value) * 0.5f;
			_particleSystem->SetObjectBoundingBox(bbox);
		}
	}

	System::Void PropertiesPanel::_numBBoxDepth_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSystem)
		{
			AABBox bbox = _particleSystem->GetObjectBoundingBox();
			bbox.minPt.z = -Decimal::ToSingle(_numBBoxDepth->Value) * 0.5f;
			bbox.maxPt.z = Decimal::ToSingle(_numBBoxDepth->Value) * 0.5f;
			_particleSystem->SetObjectBoundingBox(bbox);
		}
	}

	System::Void PropertiesPanel::_checkShowBBox_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void PropertiesPanel::_checkShowEmitter_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void PropertiesPanel::_numTime_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if(	e->KeyCode == Keys::Enter &&
			_attribute &&
			_selectedPoints->Count == 1 &&
			_selectedPoints[0].index > 0 &&
			_selectedPoints[0].index < _attribute->GetValueCount() - 1 )
		{
			ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			int index = _selectedPoints[0].index;
			values[index].variable = Decimal::ToSingle(_numTime->Value) / _graphTime;
			math3d::clamp(values[index].variable, 0.0f, 1.0f);

			GraphPoint pt = { index, values[index].variable, values[index].value };
			_selectedPoints[0] = pt;

			_panelGraph->Invalidate();
		}
	}

	System::Void PropertiesPanel::_numValue_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if(e->KeyCode == Keys::Enter && _attribute && _selectedPoints->Count == 1)
		{
			ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			int index = _selectedPoints[0].index;
			values[index].value = Decimal::ToSingle(_numValue->Value);
			math3d::clamp(values[index].value, _minValue, _maxValue);

			GraphPoint pt = { index, values[index].variable, values[index].value };
			_selectedPoints[0] = pt;

			_panelGraph->Invalidate();
		}
	}

	int PropertiesPanel::PointIndexFromPos(int x, int y)
	{
		float range = _maxValue - _minValue;
		const ParticleSystem::Attribute::Value* values = _attribute->GetValues();
		int count = _attribute->GetValueCount();
		for(int i = 0; i < count; ++i)
		{
			int px = int(GRAPH_LEFT_MARGIN + values[i].variable * _graphWidth);
			int py = int(GRAPH_MARGIN + (values[i].value - _minValue) / range * _graphHeight);
			Drawing::Rectangle rect(px - GRAPH_PT_RECT_WIDTH / 2, py - GRAPH_PT_RECT_WIDTH / 2, GRAPH_PT_RECT_WIDTH, GRAPH_PT_RECT_WIDTH);
			if(rect.Contains(x, y))
				return i;
		}

		return -1;
	}

	bool PropertiesPanel::IsPointSelected(int index)
	{
		for each(GraphPoint pt in _selectedPoints)
		{
			if(pt.index == index)
				return true;
		}

		return false;
	}

	void PropertiesPanel::UpdateValueFields()
	{
		if(_selectedPoints->Count == 1)
		{
			const ParticleSystem::Attribute::Value* values = _attribute->GetValues();

			if(	_selectedPoints[0].index > 0 &&
				_selectedPoints[0].index < _attribute->GetValueCount() - 1 )
			{
				_numTime->Enabled = true;
				_numTime->Text = (values[_selectedPoints[0].index].variable * _graphTime).ToString();
			}
			else
			{
				_numTime->Enabled = false;
			}

			_numValue->Enabled = true;
			_numValue->Text = values[_selectedPoints[0].index].value.ToString();
		}
		else
		{
			_numTime->Enabled = false;
			_numValue->Enabled = false;
		}
	}

	void PropertiesPanel::SelectPoint(int index)
	{
		if(_attribute)
		{
			GraphPoint pt = { index, _attribute->GetValues()[index].variable, _attribute->GetValues()[index].value };
			_selectedPoints->Add(pt);
		}
	}

	void PropertiesPanel::AddNewPoint(int x, int y)
	{
		if(_attribute)
		{
			int count = _attribute->GetValueCount();
			const ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			float range = _maxValue - _minValue;

			vec2f new_pt;
			int index = -1;
			for(int i = 0; i < count - 1; ++i)
			{
				float x1 = GRAPH_LEFT_MARGIN + values[i].variable * _graphWidth;
				float y1 = GRAPH_MARGIN + (values[i].value - _minValue) / range * _graphHeight;
				float x2 = GRAPH_LEFT_MARGIN + values[i + 1].variable * _graphWidth;
				float y2 = GRAPH_MARGIN + (values[i + 1].value - _minValue) / range * _graphHeight;

				vec2f pt1(x1, y1);
				vec2f pt2(x2, y2);
				vec2f dir = pt1 - pt2;
				float len = dir.length();
				rotate_90_ccw_2d(dir);
				dir.normalize();
				vec3f line(dir, -dot(dir, pt1));

				vec2f point((float)x, (float)y);
				nearest_point_on_line_2d(new_pt, point, line);
				if(	(point - new_pt).length() < 5.0f &&
					(pt1 - new_pt).length() <= len &&
					(pt2 - new_pt).length() <= len )
				{
					index = i;
					break;
				}
			}

			if(index != -1)
			{
				ParticleSystem::Attribute::Value* new_values = new(tempPool) ParticleSystem::Attribute::Value[count + 1];
				memcpy(new_values, values, (index + 1) * sizeof(ParticleSystem::Attribute::Value));
				new_values[index + 1].variable = float(new_pt.x - GRAPH_LEFT_MARGIN) / _graphWidth;
				new_values[index + 1].value = float(new_pt.y - GRAPH_MARGIN) / _graphHeight * range + _minValue;
				assert(count - index - 1 > 0);
				memcpy(&new_values[index + 2], &values[index + 1], (count - index - 1) * sizeof(ParticleSystem::Attribute::Value));
				_attribute->SetValues(new_values, count + 1);
				delete[] new_values;

				_selectedPoints->Clear();
				_panelGraph->Invalidate();
			}
		}
	}

	void PropertiesPanel::RemovePoint(int index)
	{
		if(_attribute && index > 0 && index < _attribute->GetValueCount() - 1)
		{
			int count = _attribute->GetValueCount();
			const ParticleSystem::Attribute::Value* values = _attribute->GetValues();
			ParticleSystem::Attribute::Value* new_values = new(tempPool) ParticleSystem::Attribute::Value[count - 1];
			memcpy(new_values, values, index * sizeof(ParticleSystem::Attribute::Value));
			memcpy(&new_values[index], &values[index + 1], (count - index - 1) * sizeof(ParticleSystem::Attribute::Value));
			_attribute->SetValues(new_values, count - 1);
			delete[] new_values;

			_selectedPoints->Clear();
			_panelGraph->Invalidate();
		}
	}

}
