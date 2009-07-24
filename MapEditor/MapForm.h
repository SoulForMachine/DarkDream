#pragma once

#include "MapRenderWindow.h"
#include "EditMode.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	ref class ToolPanel;
	ref class UndoManager;


	/// <summary>
	/// Summary for MapForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MapForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		MapForm(EditorCommon::FormDirector^ director, ToolPanel^ tool_panel);

		void Redraw()
			{ _renderWindow->Draw(); }
		void RedrawAsync();
		void Animate(bool anim)
			{ _renderWindow->Animate(anim); }
		void Wireframe(bool wireframe)
			{ _renderWindow->Wireframe(wireframe); }
		void ShowStats(bool stats)
			{ _renderWindow->ShowStats(stats); }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MapForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void SetCurrentEditMode(EditMode::EditModeEnum mode);

		EditorCommon::FormDirector^ _director;
		MapRenderWindow^ _renderWindow;
		ToolPanel^ _toolPanel;
		EditMode^ _currentEditMode;
		UndoManager^ _undoManager;

	private: System::Windows::Forms::ToolStrip^  _mainToolbar;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnViewMode;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnTerrainEdit;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnTerrainPaint;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnEditGrid;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnObjectPlacement;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnWaterPatch;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnRoads;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnRivers;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnFogLayers;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnTriggers;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnLights;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnParticleSystems;



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MapForm::typeid));
			this->_mainToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->_toolBtnViewMode = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnTerrainEdit = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnTerrainPaint = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnEditGrid = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnObjectPlacement = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnWaterPatch = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnRoads = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnRivers = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnFogLayers = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnTriggers = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnLights = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnParticleSystems = (gcnew System::Windows::Forms::ToolStripButton());
			this->_mainToolbar->SuspendLayout();
			this->SuspendLayout();
			// 
			// _mainToolbar
			// 
			this->_mainToolbar->AutoSize = false;
			this->_mainToolbar->ImageScalingSize = System::Drawing::Size(32, 32);
			this->_mainToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->_toolBtnViewMode, 
				this->_toolBtnTerrainEdit, this->_toolBtnTerrainPaint, this->_toolBtnEditGrid, this->_toolBtnObjectPlacement, this->_toolBtnWaterPatch, 
				this->_toolBtnRoads, this->_toolBtnRivers, this->_toolBtnFogLayers, this->_toolBtnTriggers, this->_toolBtnLights, this->_toolBtnParticleSystems});
			this->_mainToolbar->Location = System::Drawing::Point(0, 0);
			this->_mainToolbar->Name = L"_mainToolbar";
			this->_mainToolbar->Size = System::Drawing::Size(651, 38);
			this->_mainToolbar->TabIndex = 2;
			this->_mainToolbar->Text = L"toolStrip1";
			// 
			// _toolBtnViewMode
			// 
			this->_toolBtnViewMode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnViewMode->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnViewMode.Image")));
			this->_toolBtnViewMode->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnViewMode->Name = L"_toolBtnViewMode";
			this->_toolBtnViewMode->Size = System::Drawing::Size(36, 35);
			this->_toolBtnViewMode->Text = L"View Mode";
			this->_toolBtnViewMode->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnViewMode_Click);
			// 
			// _toolBtnTerrainEdit
			// 
			this->_toolBtnTerrainEdit->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnTerrainEdit->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnTerrainEdit.Image")));
			this->_toolBtnTerrainEdit->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnTerrainEdit->Name = L"_toolBtnTerrainEdit";
			this->_toolBtnTerrainEdit->Size = System::Drawing::Size(36, 35);
			this->_toolBtnTerrainEdit->Text = L"Edit Terrain";
			this->_toolBtnTerrainEdit->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnTerrainEdit_Click);
			// 
			// _toolBtnTerrainPaint
			// 
			this->_toolBtnTerrainPaint->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnTerrainPaint->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnTerrainPaint.Image")));
			this->_toolBtnTerrainPaint->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnTerrainPaint->Name = L"_toolBtnTerrainPaint";
			this->_toolBtnTerrainPaint->Size = System::Drawing::Size(36, 35);
			this->_toolBtnTerrainPaint->Text = L"Paint Terrain";
			this->_toolBtnTerrainPaint->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnTerrainPaint_Click);
			// 
			// _toolBtnEditGrid
			// 
			this->_toolBtnEditGrid->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnEditGrid->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnEditGrid.Image")));
			this->_toolBtnEditGrid->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnEditGrid->Name = L"_toolBtnEditGrid";
			this->_toolBtnEditGrid->Size = System::Drawing::Size(36, 35);
			this->_toolBtnEditGrid->Text = L"Edit Grid";
			this->_toolBtnEditGrid->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnEditGrid_Click);
			// 
			// _toolBtnObjectPlacement
			// 
			this->_toolBtnObjectPlacement->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnObjectPlacement->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnObjectPlacement.Image")));
			this->_toolBtnObjectPlacement->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnObjectPlacement->Name = L"_toolBtnObjectPlacement";
			this->_toolBtnObjectPlacement->Size = System::Drawing::Size(36, 35);
			this->_toolBtnObjectPlacement->Text = L"Place Objects";
			this->_toolBtnObjectPlacement->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnObjectPlacement_Click);
			// 
			// _toolBtnWaterPatch
			// 
			this->_toolBtnWaterPatch->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnWaterPatch->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnWaterPatch.Image")));
			this->_toolBtnWaterPatch->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnWaterPatch->Name = L"_toolBtnWaterPatch";
			this->_toolBtnWaterPatch->Size = System::Drawing::Size(36, 35);
			this->_toolBtnWaterPatch->Text = L"Water";
			this->_toolBtnWaterPatch->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnWaterPatch_Click);
			// 
			// _toolBtnRoads
			// 
			this->_toolBtnRoads->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnRoads->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnRoads.Image")));
			this->_toolBtnRoads->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnRoads->Name = L"_toolBtnRoads";
			this->_toolBtnRoads->Size = System::Drawing::Size(36, 35);
			this->_toolBtnRoads->Text = L"Roads";
			this->_toolBtnRoads->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnRoads_Click);
			// 
			// _toolBtnRivers
			// 
			this->_toolBtnRivers->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnRivers->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnRivers.Image")));
			this->_toolBtnRivers->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnRivers->Name = L"_toolBtnRivers";
			this->_toolBtnRivers->Size = System::Drawing::Size(36, 35);
			this->_toolBtnRivers->Text = L"Rivers";
			this->_toolBtnRivers->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnRivers_Click);
			// 
			// _toolBtnFogLayers
			// 
			this->_toolBtnFogLayers->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnFogLayers->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnFogLayers.Image")));
			this->_toolBtnFogLayers->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnFogLayers->Name = L"_toolBtnFogLayers";
			this->_toolBtnFogLayers->Size = System::Drawing::Size(36, 35);
			this->_toolBtnFogLayers->Text = L"Fog Layers";
			this->_toolBtnFogLayers->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnFogLayers_Click);
			// 
			// _toolBtnTriggers
			// 
			this->_toolBtnTriggers->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnTriggers->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnTriggers.Image")));
			this->_toolBtnTriggers->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnTriggers->Name = L"_toolBtnTriggers";
			this->_toolBtnTriggers->Size = System::Drawing::Size(36, 35);
			this->_toolBtnTriggers->Text = L"Triggers";
			this->_toolBtnTriggers->Click += gcnew System::EventHandler(this, &MapForm::_toolBtnTriggers_Click);
			// 
			// _toolBtnLights
			// 
			this->_toolBtnLights->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnLights->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnLights.Image")));
			this->_toolBtnLights->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnLights->Name = L"_toolBtnLights";
			this->_toolBtnLights->Size = System::Drawing::Size(36, 35);
			this->_toolBtnLights->Text = L"Lights";
			// 
			// _toolBtnParticleSystems
			// 
			this->_toolBtnParticleSystems->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnParticleSystems->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnParticleSystems.Image")));
			this->_toolBtnParticleSystems->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnParticleSystems->Name = L"_toolBtnParticleSystems";
			this->_toolBtnParticleSystems->Size = System::Drawing::Size(36, 35);
			this->_toolBtnParticleSystems->Text = L"Particle Systems";
			// 
			// MapForm
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(651, 437);
			this->CloseButton = false;
			this->Controls->Add(this->_mainToolbar);
			this->DockAreas = WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
			this->Name = L"MapForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->ShowInTaskbar = false;
			this->TabText = L"Map";
			this->Text = L"Map";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->HandleCreated += gcnew System::EventHandler(this, &MapForm::MapForm_HandleCreated);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MapForm::MapForm_MouseWheel);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MapForm::MapForm_Paint);
			this->Resize += gcnew System::EventHandler(this, &MapForm::MapForm_Resize);
			this->_mainToolbar->ResumeLayout(false);
			this->_mainToolbar->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void MapForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		private: System::Void MapForm_Resize(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MapForm_HandleCreated(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MapForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private: System::Void _toolBtnViewMode_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnTerrainEdit_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnTerrainPaint_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnEditGrid_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnObjectPlacement_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnWaterPatch_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnRoads_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnRivers_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnFogLayers_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void _toolBtnTriggers_Click(System::Object^  sender, System::EventArgs^  e);
	};
}
