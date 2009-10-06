
#include "stdafx.h"
#include "ActionRemovePatch.h"


namespace MapEditor
{

	using namespace math3d;
	using namespace Memory;
	using namespace Engine;


	ActionRemovePatch::ActionRemovePatch(int index)
	{
		_patchIndex = index;

		_elevation = new(mainPool) float[(Terrain::PATCH_WIDTH + 1) * (Terrain::PATCH_HEIGHT + 1)];
		engineAPI->world->GetTerrain().GetElevation(index * Terrain::PATCH_WIDTH, 0, (index + 1) * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _elevation);

		_entities = new(mainPool) List<Engine::Entity*>;
	}

	ActionRemovePatch::~ActionRemovePatch()
	{
		delete[] _elevation;

		for(List<Entity*>::Iterator it = _entities->Begin(); it != _entities->End(); ++it)
			delete *it;
		delete _entities;
	}

	bool ActionRemovePatch::BeginAction()
	{
		RemovePatch();
		return true;
	}

	void ActionRemovePatch::EndAction()
	{
	}

	void ActionRemovePatch::Undo()
	{
		engineAPI->world->GetTerrain().AddPatch(_patchIndex, _elevation);

		for(List<Entity*>::ConstIterator it = _entities->Begin(); it != _entities->End(); ++it)
		{
			engineAPI->world->AddEntity(*it);
		}
		_entities->Clear();
	}

	void ActionRemovePatch::Redo()
	{
		RemovePatch();
	}

	ActionType ActionRemovePatch::GetActionType()
	{
		return ActionType::REMOVE_PATCH;
	}

	void ActionRemovePatch::RemovePatch()
	{
		Terrain& terrain = engineAPI->world->GetTerrain();
		const Terrain::TerrainPatch* patches = terrain.GetPatches();

		// save all entities on this patch for undo and remove them from world
		const EntityHashMap& ents = engineAPI->world->GetEntities();
		for(EntityHashMap::ConstIterator it = ents.Begin(); it != ents.End(); ++it)
		{
			const vec3f& pos = (*it)->GetPosition();
			if(	pos.x >= patches[_patchIndex].boundBox.minPt.x &&
				pos.x <= patches[_patchIndex].boundBox.maxPt.x &&
				pos.z >= patches[_patchIndex].boundBox.minPt.z &&
				pos.z <= patches[_patchIndex].boundBox.maxPt.z )
			{
				_entities->PushBack(*it);
			}
		}

		for(List<Entity*>::ConstIterator it = _entities->Begin(); it != _entities->End(); ++it)
		{
			engineAPI->world->RemoveEntity(*it);
		}

		terrain.RemovePatch(_patchIndex);
	}

}
