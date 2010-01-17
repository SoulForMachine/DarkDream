#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	/// <summary>
	/// Summary for TriggerPanel
	/// </summary>
	public ref class TriggerPanel : public System::Windows::Forms::UserControl
	{
	public:
		TriggerPanel(void)
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
		~TriggerPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  _treeTriggers;
	private: System::Windows::Forms::ContextMenuStrip^  _menuLogicGroup;

	private: System::Windows::Forms::ToolStripMenuItem^  _mnuTriggerBox;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuSwitch;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuTimer;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuCounter;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuDamageBox;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuMoveController;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuGate;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuSpawner;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuCameraShake;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuSoundPlayer;










	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuRemoveLogicGroup;

	private: System::Windows::Forms::PropertyGrid^  _propTrigger;

	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuAddLogicGroup;
	private: System::Windows::Forms::ContextMenuStrip^  _menuTrigger;

	private: System::Windows::Forms::ToolStripMenuItem^  _mnuRemoveTrigger;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuRenameLogicGroup;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuRenameTrigger;


	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->_treeTriggers = (gcnew System::Windows::Forms::TreeView());
			this->_menuLogicGroup = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->_mnuTriggerBox = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuSwitch = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuTimer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuCounter = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuDamageBox = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuMoveController = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuGate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuSpawner = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuCameraShake = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuSoundPlayer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_mnuAddLogicGroup = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuRemoveLogicGroup = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_propTrigger = (gcnew System::Windows::Forms::PropertyGrid());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->_menuTrigger = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->_mnuRemoveTrigger = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuRenameLogicGroup = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuRenameTrigger = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuLogicGroup->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->_menuTrigger->SuspendLayout();
			this->SuspendLayout();
			// 
			// _treeTriggers
			// 
			this->_treeTriggers->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->_treeTriggers->Location = System::Drawing::Point(17, 48);
			this->_treeTriggers->Name = L"_treeTriggers";
			this->_treeTriggers->Size = System::Drawing::Size(349, 359);
			this->_treeTriggers->TabIndex = 0;
			// 
			// _menuLogicGroup
			// 
			this->_menuLogicGroup->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(14) {this->_mnuAddLogicGroup, 
				this->_mnuRemoveLogicGroup, this->_mnuRenameLogicGroup, this->toolStripMenuItem1, this->_mnuTriggerBox, this->_mnuSwitch, this->_mnuTimer, 
				this->_mnuCounter, this->_mnuDamageBox, this->_mnuMoveController, this->_mnuGate, this->_mnuSpawner, this->_mnuCameraShake, this->_mnuSoundPlayer});
			this->_menuLogicGroup->Name = L"_menuTriggers";
			this->_menuLogicGroup->Size = System::Drawing::Size(184, 318);
			// 
			// _mnuTriggerBox
			// 
			this->_mnuTriggerBox->Name = L"_mnuTriggerBox";
			this->_mnuTriggerBox->Size = System::Drawing::Size(183, 22);
			this->_mnuTriggerBox->Text = L"Trigger Box";
			// 
			// _mnuSwitch
			// 
			this->_mnuSwitch->Name = L"_mnuSwitch";
			this->_mnuSwitch->Size = System::Drawing::Size(183, 22);
			this->_mnuSwitch->Text = L"Switch";
			// 
			// _mnuTimer
			// 
			this->_mnuTimer->Name = L"_mnuTimer";
			this->_mnuTimer->Size = System::Drawing::Size(183, 22);
			this->_mnuTimer->Text = L"Timer";
			// 
			// _mnuCounter
			// 
			this->_mnuCounter->Name = L"_mnuCounter";
			this->_mnuCounter->Size = System::Drawing::Size(183, 22);
			this->_mnuCounter->Text = L"Counter";
			// 
			// _mnuDamageBox
			// 
			this->_mnuDamageBox->Name = L"_mnuDamageBox";
			this->_mnuDamageBox->Size = System::Drawing::Size(183, 22);
			this->_mnuDamageBox->Text = L"Damage Box";
			// 
			// _mnuMoveController
			// 
			this->_mnuMoveController->Name = L"_mnuMoveController";
			this->_mnuMoveController->Size = System::Drawing::Size(183, 22);
			this->_mnuMoveController->Text = L"Move Controller";
			// 
			// _mnuGate
			// 
			this->_mnuGate->Name = L"_mnuGate";
			this->_mnuGate->Size = System::Drawing::Size(183, 22);
			this->_mnuGate->Text = L"Gate";
			// 
			// _mnuSpawner
			// 
			this->_mnuSpawner->Name = L"_mnuSpawner";
			this->_mnuSpawner->Size = System::Drawing::Size(183, 22);
			this->_mnuSpawner->Text = L"Spawner";
			// 
			// _mnuCameraShake
			// 
			this->_mnuCameraShake->Name = L"_mnuCameraShake";
			this->_mnuCameraShake->Size = System::Drawing::Size(183, 22);
			this->_mnuCameraShake->Text = L"Camera Shake";
			// 
			// _mnuSoundPlayer
			// 
			this->_mnuSoundPlayer->Name = L"_mnuSoundPlayer";
			this->_mnuSoundPlayer->Size = System::Drawing::Size(183, 22);
			this->_mnuSoundPlayer->Text = L"Sound Player";
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(180, 6);
			// 
			// _mnuAddLogicGroup
			// 
			this->_mnuAddLogicGroup->Name = L"_mnuAddLogicGroup";
			this->_mnuAddLogicGroup->Size = System::Drawing::Size(183, 22);
			this->_mnuAddLogicGroup->Text = L"Add Logic Group";
			// 
			// _mnuRemoveLogicGroup
			// 
			this->_mnuRemoveLogicGroup->Name = L"_mnuRemoveLogicGroup";
			this->_mnuRemoveLogicGroup->Size = System::Drawing::Size(183, 22);
			this->_mnuRemoveLogicGroup->Text = L"Remove Logic Group";
			// 
			// _propTrigger
			// 
			this->_propTrigger->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->_propTrigger->Location = System::Drawing::Point(17, 12);
			this->_propTrigger->Name = L"_propTrigger";
			this->_propTrigger->Size = System::Drawing::Size(349, 387);
			this->_propTrigger->TabIndex = 2;
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->_treeTriggers);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->_propTrigger);
			this->splitContainer1->Size = System::Drawing::Size(387, 847);
			this->splitContainer1->SplitterDistance = 423;
			this->splitContainer1->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(13, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 20);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Triggers";
			// 
			// _menuTrigger
			// 
			this->_menuTrigger->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_mnuRemoveTrigger, 
				this->_mnuRenameTrigger});
			this->_menuTrigger->Name = L"_menuRemoveTrigger";
			this->_menuTrigger->Size = System::Drawing::Size(162, 48);
			// 
			// _mnuRemoveTrigger
			// 
			this->_mnuRemoveTrigger->Name = L"_mnuRemoveTrigger";
			this->_mnuRemoveTrigger->Size = System::Drawing::Size(161, 22);
			this->_mnuRemoveTrigger->Text = L"Remove Trigger";
			// 
			// _mnuRenameLogicGroup
			// 
			this->_mnuRenameLogicGroup->Name = L"_mnuRenameLogicGroup";
			this->_mnuRenameLogicGroup->Size = System::Drawing::Size(183, 22);
			this->_mnuRenameLogicGroup->Text = L"Rename Logic Group";
			// 
			// _mnuRenameTrigger
			// 
			this->_mnuRenameTrigger->Name = L"_mnuRenameTrigger";
			this->_mnuRenameTrigger->Size = System::Drawing::Size(161, 22);
			this->_mnuRenameTrigger->Text = L"Rename Trigger";
			// 
			// TriggerPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->Controls->Add(this->splitContainer1);
			this->Name = L"TriggerPanel";
			this->Size = System::Drawing::Size(387, 847);
			this->_menuLogicGroup->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->_menuTrigger->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
