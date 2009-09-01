
#pragma once

#include "Action.h"
#include "EM_TerrainEdit.h"


namespace MapEditor
{

	ref class ActionTerrainEdit: public Action
	{
	public:
		ActionTerrainEdit(EM_TerrainEdit::Parameters^ params);
		~ActionTerrainEdit();

		virtual void BeginAction() override;
		virtual void EndAction() override;
		virtual void Update(float dt) override;
		virtual void CancelAction() override;
		virtual void Revert() override;

	private:
		void BuildStrengthMatrix(System::Drawing::Rectangle rect);
		void GetBrushRect(System::Drawing::Rectangle% rect);

		EM_TerrainEdit::Parameters^ _parameters;
		float* _oldElevation;
		float* _undoElevation;
		System::Drawing::Rectangle _undoRect;
		float* _strengthMatrix;
		float _oldRadius;
		float _oldHardness;
	};

}
