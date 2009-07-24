
#pragma once

#include "Action.h"
#include "EM_TerrainEdit.h"


namespace MapEditor
{

	ref class ActionTerrainEdit: public Action
	{
	public:
		ActionTerrainEdit(EM_TerrainEdit::Parameters^ params)
			{ _parameters = params; }

		virtual void BeginAction() override;
		virtual void EndAction() override;
		virtual void CancelAction() override;
		virtual void Undo() override;
		virtual void Redo() override;

	private:
		EM_TerrainEdit::Parameters^ _parameters;
	};

}
