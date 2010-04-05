#include "StdAfx.h"
#include "EditorCommon/UtilityTempl.h"
#include "AddAnimForm.h"

using namespace Engine;
using namespace EditorCommon;


namespace EntityEditor
{

	System::Void AddAnimForm::AddAnimForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
		_radioStandardAnim->Checked = true;

		for(int i = 0; i < Animation::ANIM_TYPE_COUNT; ++i)
		{
			_cmbStandardAnim->Items->Add(gcnew String(Animation::GetAnimTypeName((Animation::AnimType)i)));
		}
		_cmbStandardAnim->SelectedIndex = 0;
	}

	System::Void AddAnimForm::_radioStandardAnim_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_radioStandardAnim->Checked)
		{
			_cmbStandardAnim->Enabled = true;
			_textAnimationName->Enabled = false;
		}
		else
		{
			_cmbStandardAnim->Enabled = false;
			_textAnimationName->Enabled = true;
		}
	}

	System::Void AddAnimForm::_btnBrowseFile_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_openFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Animations";
		if(_openFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
		{
			tchar* file_name = EditorUtil::GetRelativePath(_openFileDialog->FileName);
			_textAnimationFile->Text = gcnew String(file_name);
			delete[] file_name;
		}
	}

	System::Void AddAnimForm::_btnOK_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_radioStandardAnim->Checked)
			_animName = _cmbStandardAnim->Text;
		else
			_animName = _textAnimationName->Text;

		if(_animName == "")
		{
			MessageBox::Show(
				this, "You must enter a name for animation.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Information);
			DialogResult = Forms::DialogResult::None;
			return;
		}

		_fileName = _textAnimationFile->Text;
		if(_fileName == "")
		{
			MessageBox::Show(
				this, "You must enter animation file name.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Information);
			DialogResult = Forms::DialogResult::None;
			return;
		}
	}

	System::Void AddAnimForm::_radioCustomAnim_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_textAnimationName->Focus();
	}

	System::Void AddAnimForm::_openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		// the file must be within game's base directory
		if(!EditorUtil::IsInGameBaseDir(_openFileDialog->FileName))
		{
			MessageBox::Show(
				this, "File must be within game's base directory.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			e->Cancel = true;
		}
	}

}
