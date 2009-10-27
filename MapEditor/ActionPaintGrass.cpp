
#include "stdafx.h"
#include "ActionPaintGrass.h"

using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	ActionPaintGrass::ActionPaintGrass(EM_PaintGrass::Parameters^ params)
	{
		_parameters = params;
		_undoGrassData = 0;
		_oldGrassData = 0;
	}

	ActionPaintGrass::~ActionPaintGrass()
	{
		delete[] _undoGrassData;
		delete[] _oldGrassData;
	}

	bool ActionPaintGrass::BeginAction()
	{
		// save all grass
		Terrain& terrain = engineAPI->world->GetTerrain();
		const Terrain::TerrainPatch* patches = terrain.GetPatches();
		int patch_count = terrain.GetPatchCount();
		_oldGrassData = new(tempPool) Terrain::GrassBlade[patch_count * Terrain::PATCH_WIDTH * 2 * Terrain::PATCH_HEIGHT];
		terrain.GetGrassBlades(0, 0, patch_count * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldGrassData);

		GetBrushRect(_undoRect);

		return true;
	}

	void ActionPaintGrass::EndAction()
	{
		// save only modified part of grass for undo
		int patch_count = engineAPI->world->GetTerrain().GetPatchCount();
		_undoGrassData = new(mainPool) Terrain::GrassBlade[_undoRect.Width * 2 * _undoRect.Height];
		int n = 0;
		for(int y = _undoRect.Top; y < _undoRect.Bottom; ++y)
		{
			for(int x = _undoRect.Left * 2; x < _undoRect.Right * 2; ++x)
			{
				_undoGrassData[n++] = _oldGrassData[y * (patch_count * Terrain::PATCH_WIDTH * 2) + x];
			}
		}

		delete[] _oldGrassData;
		_oldGrassData = 0;
	}

	void ActionPaintGrass::CancelAction()
	{
		Terrain& terrain = engineAPI->world->GetTerrain();
		int patch_count = terrain.GetPatchCount();
		terrain.SetGrassBlades(0, 0, patch_count * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldGrassData);

		delete[] _oldGrassData;
		_oldGrassData = 0;
	}

	void ActionPaintGrass::Update(float dt)
	{
		System::Drawing::Rectangle rect;
		GetBrushRect(rect);

		_undoRect = System::Drawing::Rectangle::Union(_undoRect, rect);

		if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
			dt = -dt;

		Terrain::GrassBlade* grass_data = new(tempPool) Terrain::GrassBlade[rect.Width * 2 * rect.Height];
		engineAPI->world->GetTerrain().GetGrassBlades(rect.X, rect.Y, rect.Right, rect.Bottom, grass_data);

		vec2f center(_parameters->posX, _parameters->posZ);
		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;
		for(int y = rect.Top; y < rect.Bottom; ++y)
		{
			for(int x = rect.Left * 2; x < rect.Right * 2; ++x)
			{
				vec2f pt(x * 0.5f, float(y) + ((x & 1)? 0.5f: 0.0f));
				float dist = (pt - center).length();
				float strength;
				if(	dist > inner_rad &&
					dist < _parameters->radius )
				{
					strength = (dist - inner_rad) / (_parameters->radius - inner_rad);
					strength = cos(Min(strength, 1.0f) * PI) * 0.5f + 0.5f;
				}
				else
				{
					strength = (dist <= inner_rad)? 1.0f: 0.0f;
				}

				if(_parameters->texIndex == -1)
				{
					grass_data[n].size = 0.0f;
					grass_data[n].texIndex = 0;
				}
				else if(_parameters->texIndex != grass_data[n].texIndex)
				{
					if(strength > 0.0f)
					{
						grass_data[n].size = strength * _parameters->strength  * dt;
						grass_data[n].texIndex = _parameters->texIndex;
					}
				}
				else
				{
					grass_data[n].size += strength * _parameters->strength * dt;
					grass_data[n].texIndex = _parameters->texIndex;
				}

				clamp(grass_data[n].size, 0.0f, _parameters->scale);

				n++;
			}
		}

		engineAPI->world->GetTerrain().SetGrassBlades(rect.X, rect.Y, rect.Right, rect.Bottom, grass_data);
		delete[] grass_data;
	}

	void ActionPaintGrass::Undo()
	{
		Terrain::GrassBlade* cur_grass = new(tempPool) Terrain::GrassBlade[_undoRect.Width * 2 * _undoRect.Height];
		engineAPI->world->GetTerrain().GetGrassBlades(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, cur_grass);
		engineAPI->world->GetTerrain().SetGrassBlades(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, _undoGrassData);
		memcpy(_undoGrassData, cur_grass, _undoRect.Width * 2 * _undoRect.Height * sizeof(Terrain::GrassBlade));
		delete[] cur_grass;
	}

	void ActionPaintGrass::Redo()
	{
		Undo();
	}

	ActionType ActionPaintGrass::GetActionType()
	{
		return ActionType::PAINT_GRASS;
	}

	void ActionPaintGrass::GetBrushRect(System::Drawing::Rectangle% rect)
	{
		rect.X = Max((int)ceil(_parameters->posX - _parameters->radius), 0);
		rect.Y = Max((int)ceil(_parameters->posZ - _parameters->radius), 0);

		int right;
		float t = _parameters->posX + _parameters->radius;
		if(frac(t) == 0.0f)
			right = int(t) - 1;
		else
			right = (int)floor(t);
		right = Min(right, engineAPI->world->GetTerrain().GetPatchCount() * Terrain::PATCH_WIDTH);
		rect.Width = right - rect.Left;

		int bottom;
		t = _parameters->posZ + _parameters->radius;
		if(frac(t) == 0.0f)
			bottom = int(t) - 1;
		else
			bottom = (int)floor(t);
		bottom = Min(bottom, Terrain::PATCH_HEIGHT);
		rect.Height = bottom - rect.Top;
	}

}
