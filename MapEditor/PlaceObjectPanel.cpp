#include "StdAfx.h"
#include "Utility.h"
#include "PlaceObjectPanel.h"

using namespace System;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Drawing2D;
using namespace math3d;
using namespace Memory;


namespace MapEditor
{

	ref class ObjectTreeNode: public TreeNode
	{
	public:
		ObjectTreeNode(String^ text, String^ rel_path, bool dir)
			: TreeNode(text, dir? 0: 1, dir? 0: 1)
		{
			_directory = dir;
			_relPath = rel_path;
		}

		bool IsDirectory()
			{ return _directory; }
		String^ GetRelativePath()
			{ return _relPath; }

	private:
		bool _directory;
		String^ _relPath;
	};


	PlaceObjectPanel::PlaceObjectPanel()
	{
		InitializeComponent();

		System::Reflection::MethodInfo^ method = Control::typeid->GetMethod(
               "SetStyle",
			   Reflection::BindingFlags::NonPublic | 
			   Reflection::BindingFlags::Instance);
		array<Object^, 1>^ params = gcnew array<Object^, 1>(2);
		params[0] = ControlStyles::AllPaintingInWmPaint | ControlStyles::OptimizedDoubleBuffer;
		params[1] = true;
		method->Invoke(_panelObjectView, params);
	//	_panelObjectView->SetStyle(ControlStyles::AllPaintingInWmPaint | ControlStyles::DoubleBuffer, true);

		int cl_width = _panelObjectView->ClientRectangle.Width;
		int cl_height = _panelObjectView->ClientRectangle.Height;
		_renderer = engineAPI->renderSystem->GetRenderer();
		_objViewColorBuf = _renderer->CreateRenderbuffer();
		_objViewColorBuf->Storage(0, GL::PIXEL_FORMAT_RGBA8, cl_width, cl_height);
		_objViewDepthBuf = _renderer->CreateRenderbuffer();
		_objViewDepthBuf->Storage(0, GL::PIXEL_FORMAT_DEPTH24, cl_width, cl_height);
		_objViewFrameBuf = _renderer->CreateFramebuffer(GL::OBJ_DRAW_FRAMEBUFFER);
		_objViewFrameBuf->AttachRenderbuffer(GL::BUFFER_COLOR0, _objViewColorBuf);
		_objViewFrameBuf->AttachRenderbuffer(GL::BUFFER_DEPTH, _objViewDepthBuf);
		_fbufOk = (_objViewFrameBuf->CheckStatus() == GL::FBUF_STATUS_COMPLETE);

		_objViewCam = new(mainPool) Engine::Camera;
		float ratio = float(cl_width) / cl_height;
		_objViewCam->Perspective(60.0f, ratio, 0.1f, 1000.0f);
		_objViewBmp = gcnew Bitmap(cl_width, cl_height, PixelFormat::Format32bppArgb);
		_objRotX = 0.0f;
		_objRotY = 0.0f;

		_modelEntity = new(mainPool) Engine::ModelEntity;
		_modelLoaded = false;
		_filterText = "";
		RefreshObjectTree();
	}

	PlaceObjectPanel::~PlaceObjectPanel()
	{
		if (components)
		{
			delete components;
		}

		_renderer->DestroyRenderbuffer(_objViewColorBuf);
		_renderer->DestroyRenderbuffer(_objViewDepthBuf);
		_renderer->DestroyFramebuffer(_objViewFrameBuf);
		//! resiti ovo sranje
		_modelEntity->ModelEntity::~ModelEntity();
		::operator delete(_modelEntity);
		_objViewCam->Camera::~Camera();
		::operator delete(_objViewCam);
		delete _objViewBmp;
	}

	void PlaceObjectPanel::RefreshObjectTree()
	{
		_treeObjects->BeginUpdate();
		_treeObjects->Nodes->Clear();

		const tchar* base = engineAPI->fileSystem->GetBaseDirPath();
		String^ path = gcnew String(base) + "Entities";

		try
		{
			AddDir(path, _treeObjects->Nodes);
		}
		catch(...)
		{
		}

		if(_treeObjects->Nodes->Count > 0)
			_treeObjects->Nodes[0]->Expand();

		_treeObjects->EndUpdate();
		_panelObjectView->Refresh();
	}

	void PlaceObjectPanel::AddDir(String^ path, TreeNodeCollection^ nodes)
	{
		// add directory
		DirectoryInfo^ dir = gcnew DirectoryInfo(path);
		ObjectTreeNode^ dir_node = gcnew ObjectTreeNode(dir->Name, "", true);
		nodes->Add(dir_node);

		// add sub-directories
		array<DirectoryInfo^>^ dirs = dir->GetDirectories();
		for each(DirectoryInfo^ di in dirs)
			AddDir(di->FullName, dir_node->Nodes);

		// add files
		array<FileInfo^>^ files = dir->GetFiles();
		for each(FileInfo^ fi in files)
		{
			String^ name = gcnew String(fi->Name);
			int dot = name->LastIndexOf('.');
			if(dot != -1)
				name = name->Remove(dot); // remove extension from name

			if(MatchFilter(name))
			{
				tchar* rel_path = GetRelativePath(fi->FullName);
				ObjectTreeNode^ node = gcnew ObjectTreeNode(name, gcnew String(rel_path), false);
				delete[] rel_path;
				dir_node->Nodes->Add(node);
			}
		}
	}

	System::Void PlaceObjectPanel::_buttonRefresh_Click(System::Object^  sender, System::EventArgs^  e)
	{
		RefreshObjectTree();
	}

	System::Void PlaceObjectPanel::_buttonDrop_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void PlaceObjectPanel::_textFilter_TextChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_filterText = _textFilter->Text;
		_filterText = _filterText->Trim();
		RefreshObjectTree();
	}

	bool PlaceObjectPanel::MatchFilter(String^ str)
    {
        if(_filterText != "")
            return str->ToUpper()->Contains(_filterText->ToUpper());
        else
            return true;
    }

	System::Void PlaceObjectPanel::_treeObjects_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
	{
		ObjectTreeNode^ node = (ObjectTreeNode^)e->Node;
		if(node->IsDirectory())
		{
			_modelEntity->Unload();
			_modelLoaded = false;
		}
		else
		{
			tchar* path = ConvertString<tchar>(node->GetRelativePath());
			_modelLoaded = _modelEntity->Load(path);
			if(_modelLoaded)
			{
				engineAPI->textureManager->LoadAll();
				engineAPI->modelManager->LoadAll();
				engineAPI->animationManager->LoadAll();
				_modelEntity->SetupModelData();
				_objRotX = 0.0f;
				_objRotY = 0.0f;
			}
			delete[] path;
		}
		_panelObjectView->Refresh();
	}

	System::Void PlaceObjectPanel::_panelObjectView_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		if(_modelEntity && _modelLoaded && _fbufOk)
		{
			int cl_width = _panelObjectView->ClientRectangle.Width;
			int cl_height = _panelObjectView->ClientRectangle.Height;
			int old_vp[4];
			_renderer->GetViewport(old_vp[0], old_vp[1], old_vp[2], old_vp[3]);
			_renderer->Viewport(0, 0, cl_width, cl_height);
			const AABBox& bbox = _modelEntity->GetWorldBoundingBox();
			vec3f at = (bbox.minPt + bbox.maxPt) / 2.0f;
			mat4f cam, rot, look;
			look.look_at(at + vec3f::z_axis * (bbox.maxPt.z - bbox.minPt.z) * 2.0f, at, vec3f::y_axis);
			rot.set_rotation_y(_objRotY);
			rot.rotate_x(_objRotX);
			mul(cam, rot, look);
			_objViewCam->SetViewingTransform(cam);

			_renderer->ActiveDrawFramebuffer(_objViewFrameBuf);
			_renderer->ClearColorBuffer(0.5f, 0.5f, 0.5f, 1.0f);
			_renderer->ClearDepthStencilBuffer(GL::DEPTH_BUFFER_BIT, 1.0f, 0);
			Engine::ModelEntity* ents[] = { _modelEntity };
			engineAPI->renderSystem->RenderEntities(0, *_objViewCam, ents, 1);
			_renderer->ActiveDrawFramebuffer(0);
			_renderer->Viewport(old_vp[0], old_vp[1], old_vp[2], old_vp[3]);

			_renderer->ActiveReadFramebuffer(_objViewFrameBuf);
			BitmapData^ bmp = _objViewBmp->LockBits(Drawing::Rectangle(0, 0, cl_width, cl_height), ImageLockMode::WriteOnly, _objViewBmp->PixelFormat);
			GL::PixelStore pstore = GL::GLState::defaultPixelStore;
			pstore.rowLength = Math::Abs(bmp->Stride) / 4;
			pstore.alignment = 4;
			_renderer->ReadPixels(0, 0, cl_width, cl_height, GL::IMAGE_FORMAT_BGRA, GL::TYPE_UNSIGNED_BYTE, &pstore, bmp->Scan0.ToPointer());
			_objViewBmp->UnlockBits(bmp);
			_renderer->ActiveReadFramebuffer(0);

			e->Graphics->ScaleTransform(1.0f, -1.0f);
			e->Graphics->TranslateTransform(0.0f, - (float)cl_height);
			e->Graphics->DrawImageUnscaled(_objViewBmp, 0, 0);
		}
		else
		{
			e->Graphics->Clear(Color::FromArgb(255, 128, 128, 128));
		}
	}

	System::Void PlaceObjectPanel::_panelObjectView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_prevX = e->X;
		_prevY = e->Y;
		_panelObjectView->Capture = true;
	}

	System::Void PlaceObjectPanel::_panelObjectView_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_panelObjectView->Capture = false;
	}

	System::Void PlaceObjectPanel::_panelObjectView_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(e->Button == Windows::Forms::MouseButtons::Left)
		{
			_objRotX += (e->Y - _prevY);
			if(_objRotX < 0.0f)
				_objRotX = 360.0f + _objRotX;
			else if(_objRotX > 360.0f)
				_objRotX -= 360.0f;
			_prevY = e->Y;

			_objRotY += (e->X - _prevX);
			if(_objRotY < 0.0f)
				_objRotY = 360.0f + _objRotY;
			else if(_objRotY > 360.0f)
				_objRotY -= 360.0f;
			_prevX = e->X;
			_panelObjectView->Invalidate();
		}
	}

	String^ PlaceObjectPanel::GetSelObjectPath()
	{
		if(_treeObjects->SelectedNode == nullptr)
			return nullptr;

		ObjectTreeNode^ node = (ObjectTreeNode^)_treeObjects->SelectedNode;
		if(node->IsDirectory())
			return nullptr;

		return node->GetRelativePath();
	}

}
