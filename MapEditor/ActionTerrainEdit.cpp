
#include "stdafx.h"
#include "ActionTerrainEdit.h"

using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	ActionTerrainEdit::ActionTerrainEdit(EM_TerrainEdit::Parameters^ params)
	{
		_parameters = params;
		_strengthMatrix = 0;
		_oldElevation = 0;
		_undoElevation = 0;
	}

	ActionTerrainEdit::~ActionTerrainEdit()
	{
		delete[] _oldElevation;
		delete[] _undoElevation;
		delete[] _strengthMatrix;
	}

	void ActionTerrainEdit::BeginAction()
	{
		Terrain& terrain = engineAPI->world->GetTerrain();
		const Terrain::TerrainPatch* patches = terrain.GetPatches();
		int patch_count = terrain.GetPatchCount();
		_oldElevation = new(tempPool) float[(patch_count * Terrain::PATCH_WIDTH + 1) * (Terrain::PATCH_HEIGHT + 1)];
		terrain.GetElevation(0, 0, Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldElevation);

		GetBrushRect(_undoRect);

		BuildStrengthMatrix(_undoRect);

		_oldRadius = _parameters->radius;
		_oldHardness = _parameters->hardness;
	}

	void ActionTerrainEdit::EndAction()
	{
		int patch_count = engineAPI->world->GetTerrain().GetPatchCount();
		_undoElevation = new(mainPool) float[(_undoRect.Width + 1) * (_undoRect.Height + 1)];
		int n = 0;
		for(int y = _undoRect.Top; y <= _undoRect.Bottom; ++y)
		{
			for(int x = _undoRect.Left; x <= _undoRect.Right; ++x)
			{
				_undoElevation[n++] = _oldElevation[y * (patch_count * Terrain::PATCH_WIDTH + 1) + x];
			}
		}

		delete[] _oldElevation;
		_oldElevation = 0;
		delete[] _strengthMatrix;
		_strengthMatrix = 0;
	}

	void ActionTerrainEdit::CancelAction()
	{
		engineAPI->world->GetTerrain().SetElevation(0, 0, Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldElevation);
		delete[] _oldElevation;
		_oldElevation = 0;
		delete[] _strengthMatrix;
		_strengthMatrix = 0;
	}

	void ActionTerrainEdit::Update(float dt)
	{
		System::Drawing::Rectangle rect;
		GetBrushRect(rect);

		if(	_parameters->radius != _oldRadius ||
			_parameters->hardness != _oldHardness )
		{
			_oldRadius = _parameters->radius;
			_oldHardness = _parameters->hardness;
			BuildStrengthMatrix(rect);
		}

		_undoRect = System::Drawing::Rectangle::Union(_undoRect, rect);

		int size = (rect.Width + 1) * (rect.Height + 1);
		float* elevation = new(tempPool) float[size];
		for(int n = 0; n < size; ++n)
		{
			elevation[n] = _strengthMatrix[n] * dt;
		}
		engineAPI->world->GetTerrain().OffsetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, elevation);
		delete[] elevation;
	}

	void ActionTerrainEdit::Revert()
	{
	
	}

	void ActionTerrainEdit::BuildStrengthMatrix(System::Drawing::Rectangle rect)
	{
		delete[] _strengthMatrix;
		_strengthMatrix = new(tempPool) float[(rect.Width + 1)* (rect.Height + 1)];
		vec2f center(_parameters->posX, _parameters->posZ);
		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;
		for(int y = rect.Top; y <= rect.Bottom; ++y)
		{
			for(int x = rect.Left; x <= rect.Right; ++x)
			{
				vec2f pt((float)x, (float)y);
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
				_strengthMatrix[n++] = strength * _parameters->strength;
			}
		}
	}

	void ActionTerrainEdit::GetBrushRect(System::Drawing::Rectangle% rect)
	{
		// brush rect is always the same size for given radius no matter where the center point is;
		// points lying on right and bottom lines are not included

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
