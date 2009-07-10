
#include "Engine/ModelEntity.h"
#include "Engine/Model.h"
#include "Engine/World.h"
#include "Engine/EngineInternal.h"
#include "RenderSystem.h"
#include "EntityRenderer.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	EntityRenderer::EntityRenderer()
	{
	}

	bool EntityRenderer::Init()
	{
		return true;
	}

	void EntityRenderer::Deinit()
	{
	}

	void EntityRenderer::Render(const Camera& camera)
	{
	}

	void EntityRenderer::ReloadShaders()
	{
	}

}
