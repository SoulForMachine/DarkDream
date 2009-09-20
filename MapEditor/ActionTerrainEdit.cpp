
#include "stdafx.h"
#include "ActionTerrainEdit.h"

using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	static vec3f g_rampStart;
	static vec3f g_rampEnd;


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

	bool ActionTerrainEdit::BeginAction()
	{
		// save whole terrain elevation
		Terrain& terrain = engineAPI->world->GetTerrain();
		const Terrain::TerrainPatch* patches = terrain.GetPatches();
		int patch_count = terrain.GetPatchCount();
		_oldElevation = new(tempPool) float[(patch_count * Terrain::PATCH_WIDTH + 1) * (Terrain::PATCH_HEIGHT + 1)];
		terrain.GetElevation(0, 0, patch_count * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldElevation);

		// for ramp brush type, save starting point
		if(_parameters->editType == EM_TerrainEdit::EditType::RAMP)
		{
			g_rampStart.set(_parameters->posX, _parameters->posY, _parameters->posZ);
			GetBrushRect(_undoRect);
		}

		return true;
	}

	void ActionTerrainEdit::EndAction()
	{
		// for ramp brush type, get the ending point and perform action
		if(_parameters->editType == EM_TerrainEdit::EditType::RAMP)
		{
			g_rampEnd.set(_parameters->posX, _parameters->posY, _parameters->posZ);

			System::Drawing::Rectangle rect;
			GetBrushRect(rect);
			_undoRect = System::Drawing::Rectangle::Union(_undoRect, rect);

			MakeRamp();
		}

		// save only modified part of terrain for undo
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
		if(_parameters->editType != EM_TerrainEdit::EditType::RAMP)
		{
			Terrain& terrain = engineAPI->world->GetTerrain();
			int patch_count = terrain.GetPatchCount();
			terrain.SetElevation(0, 0, patch_count * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldElevation);
		}

		delete[] _oldElevation;
		_oldElevation = 0;
		delete[] _strengthMatrix;
		_strengthMatrix = 0;
	}

	void ActionTerrainEdit::Update(float dt)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::RAMP)
		{
			System::Drawing::Rectangle rect;
			GetBrushRect(rect);

			_undoRect = System::Drawing::Rectangle::Union(_undoRect, rect);

			switch(_parameters->editType)
			{
			case EM_TerrainEdit::EditType::RAISE_LOWER:
				UpdateRaiseLower(rect, dt);
				break;
			case EM_TerrainEdit::EditType::SMOOTH:
				UpdateSmooth(rect, dt);
				break;
			case EM_TerrainEdit::EditType::NOISE:
				UpdateNoise(rect, dt);
				break;
			case EM_TerrainEdit::EditType::PLATEAU:
				UpdatePlateau(rect, dt);
				break;
			case EM_TerrainEdit::EditType::RELATIVE_PLATEAU:
				UpdateRelativePlateau(rect, dt);
				break;
			}
		}
	}

	void ActionTerrainEdit::Undo()
	{
		float* cur_elev = new(tempPool) float[(_undoRect.Width + 1) * (_undoRect.Height + 1)];
		engineAPI->world->GetTerrain().GetElevation(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, cur_elev);
		engineAPI->world->GetTerrain().SetElevation(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, _undoElevation);
		memcpy(_undoElevation, cur_elev, (_undoRect.Width + 1) * (_undoRect.Height + 1) * sizeof(float));
		delete[] cur_elev;
	}

	void ActionTerrainEdit::Redo()
	{
		Undo();
	}

	void ActionTerrainEdit::BuildRaiseLowerMatrix(System::Drawing::Rectangle rect)
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

	void ActionTerrainEdit::BuildSmoothMatrix(System::Drawing::Rectangle rect)
	{
		int count = (rect.Width + 1) * (rect.Height + 1);
		delete[] _strengthMatrix;
		_strengthMatrix = new(tempPool) float[count];
		vec2f center(_parameters->posX, _parameters->posZ);
		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;

		float* elevation = new(tempPool) float[count];
		engineAPI->world->GetTerrain().GetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, elevation);

		for(int y = rect.Top; y <= rect.Bottom; ++y)
		{
			for(int x = rect.Left; x <= rect.Right; ++x)
			{
				vec2f pt((float)x, (float)y);
				float dist = (pt - center).length();
				float strength;
				if(dist < _parameters->radius)
				{
					int sc = 0; // sample count
					float mean = 0.0f;

					if(x > rect.Left)
					{
						mean += elevation[n - 1];
						sc++;
					}

					mean += elevation[n];
					sc++;

					if(x < rect.Right)
					{
						mean += elevation[n + 1];
						sc++;
					}

					if(y > rect.Top)
					{
						if(x > rect.Left)
						{
							mean += elevation[n - (rect.Width + 1) - 1];
							sc++;
						}

						mean += elevation[n - (rect.Width + 1)];
						sc++;

						if(x < rect.Right)
						{
							mean += elevation[n - (rect.Width + 1) + 1];
							sc++;
						}
					}

					if(y < rect.Bottom)
					{
						if(x > rect.Left)
						{
							mean += elevation[n + (rect.Width + 1) - 1];
							sc++;
						}

						mean += elevation[n + (rect.Width + 1)];
						sc++;

						if(x < rect.Right)
						{
							mean += elevation[n + (rect.Width + 1) + 1];
							sc++;
						}
					}

					mean /= sc;

					if(dist > inner_rad)
						strength = - (1.0f - (dist - inner_rad) / (_parameters->radius - inner_rad)) * (elevation[n] - mean);
					else
						strength = - (elevation[n] - mean);
				}
				else
				{
					strength = 0.0f;
				}
				_strengthMatrix[n++] = strength * _parameters->strength;
			}
		}

		delete[] elevation;
	}

	void ActionTerrainEdit::BuildNoiseMatrix(System::Drawing::Rectangle rect)
	{
		int count = (rect.Width + 1) * (rect.Height + 1);
		delete[] _strengthMatrix;
		_strengthMatrix = new(tempPool) float[count];
		vec2f center(_parameters->posX, _parameters->posZ);
		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;

		srand(666);
		float noise = 0.0f;

		for(int y = rect.Top; y <= rect.Bottom; ++y)
		{
			for(int x = rect.Left; x <= rect.Right; ++x)
			{
				vec2f pt((float)x, (float)y);
				float dist = (pt - center).length();
				float strength;
				if(rand() % 10 < 3)
					noise = 1.0f;
				else
					noise = 0.5f;
				if(dist < _parameters->radius)
				{
					if(dist > inner_rad)
						strength = (1.0f - (dist - inner_rad) / (_parameters->radius - inner_rad)) * noise;
					else
						strength = noise;
				}
				else
				{
					strength = 0.0f;
				}
				_strengthMatrix[n++] = strength * _parameters->strength;
			}
		}
	}

	void ActionTerrainEdit::BuildPlateauMatrix(System::Drawing::Rectangle rect)
	{
		int count = (rect.Width + 1) * (rect.Height + 1);
		delete[] _strengthMatrix;
		_strengthMatrix = new(tempPool) float[count];
		vec2f center(_parameters->posX, _parameters->posZ);
		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;

		float* elevation = new(tempPool) float[count];
		engineAPI->world->GetTerrain().GetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, elevation);

		float height = (GetAsyncKeyState(VK_SHIFT) & 0x8000)? - _parameters->height: _parameters->height;

		for(int y = rect.Top; y <= rect.Bottom; ++y)
		{
			for(int x = rect.Left; x <= rect.Right; ++x)
			{
				vec2f pt((float)x, (float)y);
				float dist = (pt - center).length();
				if(dist < _parameters->radius)
				{
					if(dist > inner_rad)
					{
						float strength = (dist - inner_rad) / (_parameters->radius - inner_rad);
						strength = cos(Min(strength, 1.0f) * PI) * 0.5f + 0.5f;
						_strengthMatrix[n] = height + (elevation[n] - height) * (1 - strength);
					}
					else
						_strengthMatrix[n] = height;
				}
				else
				{
					_strengthMatrix[n] = elevation[n];
				}

				++n;
			}
		}

		delete[] elevation;
	}

	void ActionTerrainEdit::BuildRelativePlateauMatrix(System::Drawing::Rectangle rect)
	{
		int count = (rect.Width + 1) * (rect.Height + 1);
		delete[] _strengthMatrix;
		_strengthMatrix = new(tempPool) float[count];
		vec2f center(_parameters->posX, _parameters->posZ);
		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;

		float* elevation = new(tempPool) float[count];
		engineAPI->world->GetTerrain().GetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, elevation);

		float height = (GetAsyncKeyState(VK_SHIFT) & 0x8000)? - _parameters->height: _parameters->height;
		int patch_count = engineAPI->world->GetTerrain().GetPatchCount();
		int ter_w = patch_count * Terrain::PATCH_WIDTH + 1;

		for(int y = rect.Top; y <= rect.Bottom; ++y)
		{
			for(int x = rect.Left; x <= rect.Right; ++x)
			{
				vec2f pt((float)x, (float)y);
				float dist = (pt - center).length();
				if(dist < _parameters->radius)
				{
					if(dist > inner_rad)
					{
						float strength = (dist - inner_rad) / (_parameters->radius - inner_rad);
						strength = cos(Min(strength, 1.0f) * PI) * 0.5f + 0.5f;
						_strengthMatrix[n] = height + ((elevation[n] - _oldElevation[y * ter_w + x]) - height) * (1 - strength);
					}
					else
						_strengthMatrix[n] = height;

					_strengthMatrix[n] += _oldElevation[y * ter_w + x];
				}
				else
				{
					_strengthMatrix[n] = elevation[n];
				}

				++n;
			}
		}

		delete[] elevation;
	}

	void ActionTerrainEdit::UpdateRaiseLower(System::Drawing::Rectangle rect, float dt)
	{
		BuildRaiseLowerMatrix(rect);

		if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
			dt = -dt;

		int count = (rect.Width + 1) * (rect.Height + 1);
		float* offsets = new(tempPool) float[count];
		for(int n = 0; n < count; ++n)
		{
			offsets[n] = _strengthMatrix[n] * dt;
		}
		engineAPI->world->GetTerrain().OffsetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, offsets);
		delete[] offsets;
	}

	void ActionTerrainEdit::UpdateSmooth(System::Drawing::Rectangle rect, float dt)
	{
		BuildSmoothMatrix(rect);

		int count = (rect.Width + 1) * (rect.Height + 1);
		float* offsets = new(tempPool) float[count];
		for(int i = 0; i < count; ++i)
		{
			offsets[i] = _strengthMatrix[i] * dt;
		}
		engineAPI->world->GetTerrain().OffsetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, offsets);
		delete[] offsets;
	}

	void ActionTerrainEdit::UpdateNoise(System::Drawing::Rectangle rect, float dt)
	{
		BuildNoiseMatrix(rect);

		if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
			dt = -dt;

		int count = (rect.Width + 1) * (rect.Height + 1);
		float* offsets = new(tempPool) float[count];
		for(int i = 0; i < count; ++i)
		{
			offsets[i] = _strengthMatrix[i] * dt;
		}
		engineAPI->world->GetTerrain().OffsetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, offsets);
		delete[] offsets;
	}

	void ActionTerrainEdit::UpdatePlateau(System::Drawing::Rectangle rect, float dt)
	{
		BuildPlateauMatrix(rect);

		engineAPI->world->GetTerrain().SetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, _strengthMatrix);
	}

	void ActionTerrainEdit::UpdateRelativePlateau(System::Drawing::Rectangle rect, float dt)
	{
		BuildRelativePlateauMatrix(rect);

		engineAPI->world->GetTerrain().SetElevation(rect.X, rect.Y, rect.Right, rect.Bottom, _strengthMatrix);
	}

	void ActionTerrainEdit::MakeRamp()
	{
		vec2f dir(g_rampEnd.x - g_rampStart.x, g_rampEnd.z - g_rampStart.z);
		dir.normalize();
		vec2f start_pt(g_rampStart.x, g_rampStart.z);
		vec2f end_pt(g_rampEnd.x, g_rampEnd.z);
		vec3f start_line(dir, -dot(dir, start_pt));
		vec3f end_line(-dir, -dot(-dir, end_pt));
		vec3f mid_line(-dir.y, dir.x, -dot(vec2f(-dir.y, dir.x), start_pt));
		float mid_len = (end_pt - start_pt).length();

		int patch_count = engineAPI->world->GetTerrain().GetPatchCount();
		int ter_w = patch_count * Terrain::PATCH_WIDTH + 1;

		int count = (_undoRect.Width + 1) * (_undoRect.Height + 1);
		float* elevation = new(tempPool) float[count];

		float inner_rad = _parameters->hardness * _parameters->radius;
		int n = 0;

		for(int y = _undoRect.Top; y <= _undoRect.Bottom; ++y)
		{
			for(int x = _undoRect.Left; x <= _undoRect.Right; ++x)
			{
				vec2f pt((float)x, (float)y);
				float dist = point_to_line_dist_2d(pt, mid_line);

				if(	point_to_line_sgn_dist_2d(pt, start_line) > 0 &&
					point_to_line_sgn_dist_2d(pt, end_line) > 0 &&
					dist < _parameters->radius )
				{
					vec2f p;
					nearest_point_on_line_2d(p, pt, mid_line);
					float t = (p - start_pt).length() / mid_len;
					float elev = lerp(g_rampStart.y, g_rampEnd.y, t);

					if(dist > inner_rad)
					{
						float strength = (dist - inner_rad) / (_parameters->radius - inner_rad);
						strength = cos(Min(strength, 1.0f) * PI) * 0.5f + 0.5f;
						elevation[n] = elev + (_oldElevation[y * ter_w + x] - elev) * (1 - strength);
					}
					else
					{
						elevation[n] = elev;
					}
				}
				else
				{
					elevation[n] = _oldElevation[y * ter_w + x];
				}

				n++;
			}
		}

		engineAPI->world->GetTerrain().SetElevation(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, elevation);
		delete[] elevation;
	}

	void ActionTerrainEdit::GetBrushRect(System::Drawing::Rectangle% rect)
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
