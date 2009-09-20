
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	public ref class EM_AddPatch: public EditMode
	{
	public:
		EM_AddPatch(EditModeEventListener^ listener, bool persistent);
		~EM_AddPatch();

		virtual EditModeEnum GetModeEnum() override
			{ return EditModeEnum::ADD_PATCH; }

		virtual void Activate() override;
		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Render() override;

	private:
		int _patchIndex;
		GL::Renderer* _renderer;
		GL::Buffer* _vertBuf;
		GL::VertexFormat* _vertFmt;
		const Engine::ASMProgRes* _vertProg;
		const Engine::ASMProgRes* _fragProg;
	};

}
