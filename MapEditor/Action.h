
#pragma once

namespace MapEditor
{

	ref class Action abstract
	{
	public:
		virtual void BeginAction() = 0;
		virtual void EndAction() = 0;
		virtual void Update(float dt) = 0;
		virtual void CancelAction() = 0;
		virtual void Revert() = 0;
	};

}
