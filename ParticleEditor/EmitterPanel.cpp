#include "StdAfx.h"
#include "Utility.h"
#include "PropertiesPanel.h"
#include "EmitterPanel.h"

using namespace Engine;


namespace ParticleEditor
{

	ref class EmitterListItem: public ListViewItem
	{
	public:
		EmitterListItem(ParticleSystem::Emitter* emitter)
		{
			this->Text = gcnew String(emitter->GetName());
			this->Checked = emitter->IsEnabled();
			_emitter = emitter;
		}

		ParticleSystem::Emitter* GetEmitter()
			{ return _emitter; }

	private:
		ParticleSystem::Emitter* _emitter;
	};


	static float g_attribValueRanges[ParticleSystem::Emitter::ATTRIB_COUNT][2] =
	{
		{ -10.0f, 10.0f },
		{ 0.0f, 100.0f },
		{ 0.0f, 360.0f },
		{ 0.0f, 200.0f },
		{ 0.0f, 10.0f },
		{ 0.0f, 10.0f },
		{ 0.0f, 10.0f },
		{ -360.0f, 360.0f },
		{ -360.0f, 360.0f },
		{ -360.0f, 360.0f },
		{ 0.0f, 1.0f },
		{ -360.0f, 360.0f },
		{ 0.0f, 1.0f },
		{ -10.0f, 10.0f },
		{ 0.0f, 1.0f },
	};


	EmitterPanel::EmitterPanel(PropertiesPanel^ properties_panel)
	{
		InitializeComponent();

		_textName->MaxLength = ParticleSystem::Emitter::EMITTER_NAME_MAX_LEN;

		_cmbEmitterType->Items->Clear();
		for(int i = 0; i < ParticleSystem::Emitter::EMITTER_TYPE_COUNT; ++i)
		{
			String^ name = gcnew String(ParticleSystem::Emitter::GetEmitterTypeString((ParticleSystem::Emitter::EmitterType)i));
			_cmbEmitterType->Items->Add(name);
		}

		_cmbShader->Items->Clear();
		for(int i = 0; i < ParticleSystem::Emitter::EMITTER_SHADER_COUNT; ++i)
		{
			String^ name = gcnew String(ParticleSystem::Emitter::GetEmitterShaderString((ParticleSystem::Emitter::EmitterShader)i));
			_cmbShader->Items->Add(name);
		}

		_listEmitterProperties->Items->Clear();
		for(int i = 0; i < ParticleSystem::Emitter::ATTRIB_COUNT; ++i)
		{
			_listEmitterProperties->Items->Add(gcnew String(ParticleSystem::Emitter::GetAttributeName(i)));
		}

		_particleSys = 0;
		_selectedEmitter = 0;
		_propertiesPanel = properties_panel;
		UpdateControls();
	}

	EmitterPanel::~EmitterPanel()
	{
		if (components)
		{
			delete components;
		}
	}

	void EmitterPanel::SetParticleSystem(Engine::ParticleSystem* part_sys)
	{
		_particleSys = part_sys;

		RefreshEmitterList();
	}

	void EmitterPanel::UpdateControls()
	{
		if(_particleSys && _selectedEmitter)
		{
			for each(Control^ ctrl in _groupEmitterProp->Controls)
				ctrl->Enabled = true;

			_btnRemove->Enabled = true;
			_btnDown->Enabled = true;
			_btnUp->Enabled = true;

			_textName->Text = gcnew String(_selectedEmitter->GetName());
			_cmbEmitterType->SelectedIndex = (int)_selectedEmitter->GetEmitterType();
			_cmbShader->SelectedIndex = (int)_selectedEmitter->GetShader();
			
			if(_selectedEmitter->GetTexture())
			{
				_radioTexture->Checked = true;
				_textResource->Text = gcnew String(_selectedEmitter->GetTexture()->GetFileName());
			}
			else if(_selectedEmitter->GetModel())
			{
				_radioModel->Checked = true;
				_textResource->Text = gcnew String(_selectedEmitter->GetModel()->GetFileName());
			}
			else
			{
				_radioTexture->Checked = true;
				_textResource->Text = "";
			}

			_checkLoop->Checked = _selectedEmitter->GetLoop();
			_checkImplode->Checked = _selectedEmitter->GetImplode();
			_checkEmitFromEdge->Checked = _selectedEmitter->EmitFromEdge();
			_checkAnimTex->Checked = _selectedEmitter->IsAnimatedTex();
			_numEmitterLifeStart->Text = _selectedEmitter->GetLifeStart().ToString();
			_numEmitterLife->Text = _selectedEmitter->GetLife().ToString();
			_listEmitterProperties->SelectedIndex = -1;

			_numParticleLife->Text = _selectedEmitter->GetParticleLife().ToString();
			_checkRandomOrient->Checked = _selectedEmitter->GetParticleRandomOrient();
		}
		else
		{
			for each(Control^ ctrl in _groupEmitterProp->Controls)
				ctrl->Enabled = false;

			_btnRemove->Enabled = false;
			_btnDown->Enabled = false;
			_btnUp->Enabled = false;

			_textName->Text = "";
			_cmbEmitterType->SelectedIndex = -1;
			_cmbShader->SelectedIndex = -1;
			
			_radioTexture->Checked = true;
			_textResource->Text = "";

			_checkLoop->Checked = false;
			_checkImplode->Checked = false;
			_checkEmitFromEdge->Checked = false;
			_checkAnimTex->Checked = false;
			_numEmitterLifeStart->Text = "0.0";
			_numEmitterLife->Text = "0.0";
			_listEmitterProperties->SelectedIndex = -1;

			_numParticleLife->Text = "0.0";
			_checkRandomOrient->Checked = false;
		}
	}

	void EmitterPanel::RefreshEmitterList()
	{
		_listEmitters->Items->Clear();
		if(_particleSys)
		{
			List<ParticleSystem::Emitter*>& emitters = _particleSys->GetEmitterList();
			for(List<ParticleSystem::Emitter*>::Iterator it = emitters.Begin(); it != emitters.End(); ++it)
			{
				EmitterListItem^ item = gcnew EmitterListItem(*it);
				_listEmitters->Items->Add(item);
			}
			_listEmitters->SelectedIndices->Clear();
		}

		_selectedEmitter = 0;
	}

	String^ EmitterPanel::MakeEmitterName()
	{
		int n = 1;
		bool exist;
		String^ name;

		do
		{
			exist = false;
			name = "Emitter" + n.ToString();
			for each(EmitterListItem^ item in _listEmitters->Items)
			{
				if(name == item->Text)
				{
					exist = true;
					break;
				}
			}
			n++;
		}
		while(exist);

		return name;
	}

	System::Void EmitterPanel::_listEmitters_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listEmitters->SelectedItems->Count == 1)
		{
			EmitterListItem^ item = (EmitterListItem^)_listEmitters->SelectedItems[0];
			_selectedEmitter = item->GetEmitter();
		}
		else
		{
			_selectedEmitter = 0;
		}

		UpdateControls();
	}

	System::Void EmitterPanel::_btnAdd_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ParticleSystem::Emitter* emitter = _particleSys->AddEmitter();
		if(emitter)
		{
			char* name = ConvertString<char>(MakeEmitterName());
			emitter->SetName(name);
			delete[] name;

			// set default attributes

			ParticleSystem::Attribute::Value vel[] = { { 0.0f, 0.5f }, { 1.0f, 0.5f } };
			emitter->Velocity.SetValues(vel, COUNTOF(vel));

			ParticleSystem::Attribute::Value size[] = { { 0.0f, 0.25f }, { 1.0f, 0.25f } };
			emitter->Size.SetValues(size, COUNTOF(size));

			ParticleSystem::Attribute::Value angle[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->EmitAngle.SetValues(angle, COUNTOF(angle));

			ParticleSystem::Attribute::Value rate[] = { { 0.0f, 20.0f }, { 1.0f, 20.0f } };
			emitter->EmitRate.SetValues(rate, COUNTOF(rate));

			ParticleSystem::Attribute::Value offsx[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->OffsetX.SetValues(offsx, COUNTOF(offsx));

			ParticleSystem::Attribute::Value offsy[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->OffsetY.SetValues(offsy, COUNTOF(offsy));

			ParticleSystem::Attribute::Value offsz[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->OffsetZ.SetValues(offsz, COUNTOF(offsz));

			ParticleSystem::Attribute::Value rotx[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->RotationX.SetValues(rotx, COUNTOF(rotx));

			ParticleSystem::Attribute::Value roty[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->RotationY.SetValues(roty, COUNTOF(roty));

			ParticleSystem::Attribute::Value rotz[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->RotationZ.SetValues(rotz, COUNTOF(rotz));

			ParticleSystem::Attribute::Value part_size[] = { { 0.0f, 0.2f }, { 1.0f, 0.2f } };
			emitter->ParticleSize.SetValues(part_size, COUNTOF(part_size));

			ParticleSystem::Attribute::Value rot_speed[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->ParticleRotationSpeed.SetValues(rot_speed, COUNTOF(rot_speed));

			ParticleSystem::Attribute::Value alpha[] = { { 0.0f, 1.0f }, { 1.0f, 1.0f } };
			emitter->ParticleAlpha.SetValues(alpha, COUNTOF(alpha));

			ParticleSystem::Attribute::Value grav[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->ParticleGravity.SetValues(grav, COUNTOF(grav));

			ParticleSystem::Attribute::Value frict[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f } };
			emitter->ParticleFriction.SetValues(frict, COUNTOF(frict));

			EmitterListItem^ item = gcnew EmitterListItem(emitter);
			_listEmitters->Items->Add(item);
			item->Selected = true;
		}
	}

	System::Void EmitterPanel::_btnRemove_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSys && _selectedEmitter && _listEmitters->SelectedItems->Count == 1)
		{
			_particleSys->RemoveEmitter(*_selectedEmitter);
			_listEmitters->Items->Remove(_listEmitters->SelectedItems[0]);
		}
	}

	System::Void EmitterPanel::_btnEnableAll_Click(System::Object^  sender, System::EventArgs^  e)
	{
		for each(EmitterListItem^ item in _listEmitters->Items)
		{
			item->Checked = true;
		}
	}

	System::Void EmitterPanel::_btnUp_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSys && _selectedEmitter && _listEmitters->SelectedItems->Count == 1 &&
			_listEmitters->SelectedItems[0]->Index > 0)
		{
			ParticleSystem::Emitter* emitter = _selectedEmitter;
			_particleSys->MoveEmitterUp(*emitter);

			int index = _listEmitters->SelectedItems[0]->Index - 1;
			_listEmitters->Items->Remove(_listEmitters->SelectedItems[0]);
			EmitterListItem^ item = gcnew EmitterListItem(emitter);
			_listEmitters->Items->Insert(index, item);
			item->Selected = true;
		}
	}

	System::Void EmitterPanel::_btnDown_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_particleSys && _selectedEmitter && _listEmitters->SelectedItems->Count == 1 &&
			_listEmitters->SelectedItems[0]->Index < _listEmitters->Items->Count - 1)
		{
			ParticleSystem::Emitter* emitter = _selectedEmitter;
			_particleSys->MoveEmitterDown(*emitter);

			int index = _listEmitters->SelectedItems[0]->Index + 1;
			_listEmitters->Items->Remove(_listEmitters->SelectedItems[0]);
			EmitterListItem^ item = gcnew EmitterListItem(emitter);
			_listEmitters->Items->Insert(index, item);
			item->Selected = true;
		}
	}

	System::Void EmitterPanel::_textName_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
		{
			char* name = ConvertString<char>(_textName->Text);
			_selectedEmitter->SetName(name);
			delete[] name;

			_listEmitters->SelectedItems[0]->Text = _textName->Text;
		}
	}

	System::Void EmitterPanel::_cmbEmitterType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter && _cmbEmitterType->SelectedIndex != -1)
		{
			_selectedEmitter->SetEmitterType((ParticleSystem::Emitter::EmitterType)_cmbEmitterType->SelectedIndex);
		}
	}

	System::Void EmitterPanel::_cmbShader_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter && _cmbShader->SelectedIndex != -1)
		{
			_selectedEmitter->SetShader((ParticleSystem::Emitter::EmitterShader)_cmbShader->SelectedIndex);
		}
	}

	System::Void EmitterPanel::_radioTexture_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter && _selectedEmitter->GetParticleType() == ParticleSystem::Emitter::PARTICLE_TYPE_MODEL)
		{
			_selectedEmitter->ClearModel();
		}
	}

	System::Void EmitterPanel::_radioModel_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter && _selectedEmitter->GetParticleType() == ParticleSystem::Emitter::PARTICLE_TYPE_TEXTURE)
		{
			_selectedEmitter->ClearTexture();
		}
	}

	System::Void EmitterPanel::_btnBrowseResource_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
		{
			_openFileDialog->Reset();
			if(_radioTexture->Checked)
			{
				_openFileDialog->Title = "Select Texture";
				_openFileDialog->Filter = "Texture Files (*.dds;*.tga;*.bmp)|*.dds;*.tga;*.bmp|All Files (*.*)|*.*";
				_openFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
			}
			else
			{
				_openFileDialog->Title = "Select Model";
				_openFileDialog->Filter = "Model Files (*.model)|*.model|All Files (*.*)|*.*";
				_openFileDialog->DefaultExt = "model";
				_openFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Models";
			}

			if(_openFileDialog->ShowDialog(this) == Windows::Forms::DialogResult::OK)
			{
				tchar* file_name = GetRelativePath(_openFileDialog->FileName);
				if(_radioTexture->Checked)
					_selectedEmitter->SetTexture(file_name);
				else
					_selectedEmitter->SetModel(file_name);
				_textResource->Text = gcnew String(file_name);
				delete[] file_name;
			}
		}
	}

	System::Void EmitterPanel::_checkLoop_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetLoop(_checkLoop->Checked);
	}

	System::Void EmitterPanel::_checkImplode_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetImplode(_checkImplode->Checked);
	}

	System::Void EmitterPanel::_checkEmitFromEdge_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetEmitFromEdge(_checkEmitFromEdge->Checked);
	}

	System::Void EmitterPanel::_checkAnimTex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetAnimatedTex(_checkAnimTex->Checked);
	}

	System::Void EmitterPanel::_numEmitterLifeStart_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetLifeStart(Decimal::ToSingle(_numEmitterLifeStart->Value));
	}

	System::Void EmitterPanel::_numEmitterLife_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetLife(Decimal::ToSingle(_numEmitterLife->Value));
	}

	System::Void EmitterPanel::_listEmitterProperties_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter && _listEmitterProperties->SelectedIndex != -1)
		{
			ParticleSystem::Attribute** attribs = _selectedEmitter->GetAttributeArray();
			int attr_index = _listEmitterProperties->SelectedIndex;
			String^ name = gcnew String(ParticleSystem::Emitter::GetAttributeName(attr_index));
			float time = (attr_index > 9)? _selectedEmitter->GetParticleLife(): _selectedEmitter->GetLife();
			_propertiesPanel->SetGraphAttribute(attribs[attr_index], name, time, g_attribValueRanges[attr_index][0], g_attribValueRanges[attr_index][1]);
		}
		else
		{
			_propertiesPanel->SetGraphAttribute(0, "", 0.0f, 0.0f, 0.0f);
		}
	}

	System::Void EmitterPanel::_numParticleLife_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetParticleLife(Decimal::ToSingle(_numParticleLife->Value));
	}

	System::Void EmitterPanel::_checkRandomOrient_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_selectedEmitter)
			_selectedEmitter->SetParticleRandomOrient(_checkRandomOrient->Checked);
	}

	System::Void EmitterPanel::_listEmitters_ItemChecked(System::Object^  sender, System::Windows::Forms::ItemCheckedEventArgs^  e) 
	{
		EmitterListItem^ item = (EmitterListItem^)e->Item;
		item->GetEmitter()->Enable(e->Item->Checked);
	}

	System::Void EmitterPanel::_openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		// the file must be within game's base directory
		if(!IsInGameBaseDir(_openFileDialog->FileName))
		{
			MessageBox::Show(
				this, "File must be within game's base directory.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			e->Cancel = true;
		}
	}

}
