
#pragma once

#include "Action.h"
#include "EM_TerrainEdit.h"


namespace MapEditor
{

	struct ActionTerrainEdit_ObjUndoData
	{
		Engine::Entity* entity;
		math3d::vec3f position;
	};


	ref class ActionTerrainEdit: public Action
	{
	public:
		ActionTerrainEdit(EM_TerrainEdit::Parameters^ params);
		~ActionTerrainEdit();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Update(float dt) override;
		virtual void CancelAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
		virtual ActionType GetActionType() override;

	private:
		void BuildRaiseLowerMatrix(System::Drawing::Rectangle rect);
		void BuildSmoothMatrix(System::Drawing::Rectangle rect);
		void BuildNoiseMatrix(System::Drawing::Rectangle rect);
		void BuildPlateauMatrix(System::Drawing::Rectangle rect);
		void BuildRelativePlateauMatrix(System::Drawing::Rectangle rect);
		void UpdateRaiseLower(System::Drawing::Rectangle rect, float dt);
		void UpdateSmooth(System::Drawing::Rectangle rect, float dt);
		void UpdateNoise(System::Drawing::Rectangle rect, float dt);
		void UpdatePlateau(System::Drawing::Rectangle rect, float dt);
		void UpdateRelativePlateau(System::Drawing::Rectangle rect, float dt);
		void MakeRamp();
		void GetBrushRect(System::Drawing::Rectangle% rect);
		void MoveAffectedObjects(System::Drawing::Rectangle rect);

		EM_TerrainEdit::Parameters^ _parameters;
		float* _oldElevation;
		float* _undoElevation;
		System::Drawing::Rectangle _undoRect;
		float* _strengthMatrix;
		HashMap<Engine::Entity*, ActionTerrainEdit_ObjUndoData>* _undoObjects;
	};

}
