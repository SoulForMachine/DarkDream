
#include "stdafx.h"
#include "ActionPaintGrass.h"

using namespace System;
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
		Memory::Delete(_undoGrassData);
		Memory::Delete(_oldGrassData);
	}

	bool ActionPaintGrass::BeginAction()
	{
		// save all grass
		Terrain& terrain = engineAPI->world->GetTerrain();
		const Terrain::TerrainPatch* patches = terrain.GetPatches();
		int patch_count = terrain.GetPatchCount();
		_oldGrassData = NewArray<Terrain::GrassBlade>(tempPool, patch_count * Terrain::PATCH_WIDTH * 2 * Terrain::PATCH_HEIGHT);
		terrain.GetGrassBlades(0, 0, patch_count * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldGrassData);

		GetBrushRect(_undoRect);

		return true;
	}

	void ActionPaintGrass::EndAction()
	{
		// save only modified part of grass for undo
		int patch_count = engineAPI->world->GetTerrain().GetPatchCount();
		_undoGrassData = NewArray<Terrain::GrassBlade>(mainPool, _undoRect.Width * 2 * _undoRect.Height);
		int n = 0;
		for(int y = _undoRect.Top; y < _undoRect.Bottom; ++y)
		{
			for(int x = _undoRect.Left * 2; x < _undoRect.Right * 2; ++x)
			{
				_undoGrassData[n++] = _oldGrassData[y * (patch_count * Terrain::PATCH_WIDTH * 2) + x];
			}
		}

		Memory::Delete(_oldGrassData);
		_oldGrassData = 0;
	}

	void ActionPaintGrass::CancelAction()
	{
		Terrain& terrain = engineAPI->world->GetTerrain();
		int patch_count = terrain.GetPatchCount();
		terrain.SetGrassBlades(0, 0, patch_count * Terrain::PATCH_WIDTH, Terrain::PATCH_HEIGHT, _oldGrassData);

		Memory::Delete(_oldGrassData);
		_oldGrassData = 0;
	}

	void ActionPaintGrass::Update(float dt)
	{
		static const float rand_ind[32][16] =
		{

			0.405969f, 0.392239f, 0.878866f, 0.459129f, 0.848916f, 0.919238f, 0.912206f, 0.962871f, 0.030044f, 0.090258f, 0.864877f, 0.066566f, 0.795290f, 0.721085f, 0.308040f, 0.254518f, 
			0.763030f, 0.920112f, 0.191238f, 0.805926f, 0.444725f, 0.767358f, 0.620396f, 0.672658f, 0.911896f, 0.807828f, 0.293004f, 0.281759f, 0.924713f, 0.487052f, 0.925688f, 0.365963f, 
			0.310027f, 0.097929f, 0.591121f, 0.552560f, 0.502873f, 0.223166f, 0.458464f, 0.219205f, 0.284676f, 0.644093f, 0.794510f, 0.886058f, 0.894507f, 0.358188f, 0.784123f, 0.706835f, 
			0.274343f, 0.160676f, 0.754874f, 0.515924f, 0.641834f, 0.328807f, 0.729110f, 0.625624f, 0.019394f, 0.339213f, 0.215510f, 0.116131f, 0.881523f, 0.353697f, 0.145361f, 0.896133f, 
			0.271271f, 0.719669f, 0.377551f, 0.607439f, 0.089192f, 0.830156f, 0.528555f, 0.002136f, 0.872919f, 0.254207f, 0.571819f, 0.667156f, 0.840168f, 0.850344f, 0.618076f, 0.214381f, 
			0.104324f, 0.432277f, 0.350759f, 0.762068f, 0.038962f, 0.001915f, 0.137049f, 0.992254f, 0.371288f, 0.740539f, 0.947156f, 0.454277f, 0.984214f, 0.549647f, 0.599874f, 0.373834f, 
			0.851525f, 0.831727f, 0.514827f, 0.539019f, 0.134521f, 0.145037f, 0.019983f, 0.136243f, 0.563797f, 0.890113f, 0.005276f, 0.854133f, 0.406074f, 0.299797f, 0.887061f, 0.615522f, 
			0.720874f, 0.400167f, 0.967350f, 0.606636f, 0.806449f, 0.427717f, 0.544443f, 0.647135f, 0.305752f, 0.262866f, 0.449594f, 0.902004f, 0.778083f, 0.320822f, 0.635312f, 0.901707f, 
			0.554152f, 0.311421f, 0.179141f, 0.430738f, 0.378359f, 0.700991f, 0.865997f, 0.253290f, 0.496615f, 0.500567f, 0.743194f, 0.681664f, 0.138556f, 0.583597f, 0.610934f, 0.537461f, 
			0.198191f, 0.364522f, 0.982758f, 0.826194f, 0.639577f, 0.803604f, 0.413725f, 0.439104f, 0.106566f, 0.648633f, 0.070278f, 0.929952f, 0.911758f, 0.027086f, 0.912507f, 0.514832f, 
			0.631996f, 0.478758f, 0.016080f, 0.672620f, 0.249862f, 0.011424f, 0.349170f, 0.387477f, 0.441899f, 0.393304f, 0.716857f, 0.530354f, 0.618497f, 0.719060f, 0.549358f, 0.997187f, 
			0.645930f, 0.672242f, 0.099478f, 0.251292f, 0.528437f, 0.402472f, 0.178924f, 0.372586f, 0.381622f, 0.615047f, 0.811542f, 0.008567f, 0.296624f, 0.550430f, 0.842745f, 0.381385f, 
			0.863694f, 0.990070f, 0.496353f, 0.896292f, 0.769303f, 0.264870f, 0.815817f, 0.052818f, 0.686600f, 0.916524f, 0.313957f, 0.632342f, 0.858419f, 0.964106f, 0.706936f, 0.012441f, 
			0.367223f, 0.309409f, 0.050882f, 0.689192f, 0.540611f, 0.715318f, 0.873311f, 0.608447f, 0.391050f, 0.393827f, 0.992629f, 0.915124f, 0.502286f, 0.565565f, 0.316394f, 0.990734f, 
			0.183723f, 0.252211f, 0.004756f, 0.584194f, 0.750593f, 0.313983f, 0.976621f, 0.020989f, 0.876857f, 0.159834f, 0.736977f, 0.953991f, 0.365179f, 0.841656f, 0.782023f, 0.460172f, 
			0.247726f, 0.762032f, 0.227807f, 0.940736f, 0.927399f, 0.583915f, 0.595383f, 0.711921f, 0.215337f, 0.896633f, 0.008417f, 0.797368f, 0.124650f, 0.845254f, 0.698257f, 0.754809f, 
			0.841189f, 0.764018f, 0.872941f, 0.721540f, 0.257034f, 0.009021f, 0.576707f, 0.261184f, 0.335888f, 0.565719f, 0.570807f, 0.852204f, 0.723620f, 0.547577f, 0.219046f, 0.386267f, 
			0.126395f, 0.522031f, 0.035773f, 0.100333f, 0.695392f, 0.115187f, 0.074158f, 0.332568f, 0.071375f, 0.075987f, 0.770899f, 0.493649f, 0.004272f, 0.072283f, 0.239257f, 0.521573f, 
			0.062655f, 0.629607f, 0.987251f, 0.506893f, 0.305432f, 0.213176f, 0.901542f, 0.056404f, 0.065640f, 0.205036f, 0.247912f, 0.024248f, 0.376320f, 0.033496f, 0.238548f, 0.756364f, 
			0.639125f, 0.298803f, 0.394121f, 0.970172f, 0.728861f, 0.836136f, 0.566601f, 0.298257f, 0.039583f, 0.865921f, 0.066399f, 0.635790f, 0.629730f, 0.549633f, 0.303444f, 0.417837f, 
			0.142525f, 0.792256f, 0.082356f, 0.169104f, 0.850038f, 0.078190f, 0.797706f, 0.503161f, 0.415890f, 0.193946f, 0.951787f, 0.575081f, 0.657962f, 0.028769f, 0.927323f, 0.061911f, 
			0.027218f, 0.554432f, 0.663268f, 0.219340f, 0.005454f, 0.257556f, 0.003475f, 0.833641f, 0.432171f, 0.406110f, 0.041492f, 0.021890f, 0.011586f, 0.045822f, 0.520004f, 0.481041f, 
			0.925500f, 0.404498f, 0.066515f, 0.797681f, 0.517028f, 0.344514f, 0.413618f, 0.679883f, 0.916656f, 0.066306f, 0.420569f, 0.261709f, 0.357498f, 0.738385f, 0.267459f, 0.578680f, 
			0.901111f, 0.080810f, 0.433773f, 0.814882f, 0.679549f, 0.380199f, 0.413843f, 0.221910f, 0.561342f, 0.055835f, 0.250872f, 0.526401f, 0.774884f, 0.921771f, 0.760864f, 0.213783f, 
			0.443227f, 0.287280f, 0.455529f, 0.864305f, 0.248536f, 0.931311f, 0.914407f, 0.653411f, 0.151566f, 0.128634f, 0.573131f, 0.436439f, 0.873170f, 0.055269f, 0.929506f, 0.858692f, 
			0.005972f, 0.349128f, 0.115890f, 0.997496f, 0.547524f, 0.035393f, 0.492649f, 0.099316f, 0.156449f, 0.724996f, 0.520946f, 0.864315f, 0.552452f, 0.953383f, 0.460914f, 0.466634f, 
			0.908570f, 0.614083f, 0.175516f, 0.925066f, 0.435864f, 0.478445f, 0.859734f, 0.011182f, 0.631671f, 0.019665f, 0.456945f, 0.109075f, 0.244264f, 0.699284f, 0.116644f, 0.089241f, 
			0.242045f, 0.780626f, 0.496004f, 0.391266f, 0.021154f, 0.692164f, 0.740528f, 0.967745f, 0.483281f, 0.969657f, 0.270476f, 0.372262f, 0.793195f, 0.743059f, 0.102361f, 0.486106f, 
			0.408881f, 0.135786f, 0.904969f, 0.102674f, 0.235995f, 0.016939f, 0.076023f, 0.750193f, 0.007616f, 0.903747f, 0.158472f, 0.587542f, 0.406742f, 0.799827f, 0.105861f, 0.388844f, 
			0.674866f, 0.466963f, 0.537744f, 0.053052f, 0.688532f, 0.323580f, 0.563366f, 0.253022f, 0.790769f, 0.104693f, 0.356510f, 0.582956f, 0.121341f, 0.500414f, 0.440814f, 0.400801f, 
			0.069062f, 0.015625f, 0.325918f, 0.330822f, 0.813807f, 0.839310f, 0.706137f, 0.305055f, 0.136701f, 0.792779f, 0.548996f, 0.075125f, 0.425619f, 0.335440f, 0.627712f, 0.398265f, 
			0.159238f, 0.052287f, 0.445537f, 0.792835f, 0.271463f, 0.273302f, 0.232811f, 0.599184f, 0.890558f, 0.339914f, 0.796652f, 0.581978f, 0.834389f, 0.404426f, 0.723437f, 0.119053f, 
		};

		System::Drawing::Rectangle rect;
		GetBrushRect(rect);

		_undoRect = System::Drawing::Rectangle::Union(_undoRect, rect);

		if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
			dt = -dt;

		Terrain::GrassBlade* grass_data = NewArray<Terrain::GrassBlade>(tempPool, rect.Width * 2 * rect.Height);
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

				int tex_index;
				if(_parameters->randomTex)
				{
					tex_index = int(rand_ind[x % 32][y % 16] * _parameters->texCount);
					grass_data[n].size += strength * _parameters->strength * dt;
					grass_data[n].texIndex = tex_index;
				}
				else
				{
					tex_index = _parameters->texIndex;

					if(tex_index == -1)
					{
						grass_data[n].size = 0.0f;
						grass_data[n].texIndex = 0;
					}
					else if(tex_index != grass_data[n].texIndex)
					{
						if(strength > 0.0f)
						{
							grass_data[n].size = strength * _parameters->strength  * dt;
							grass_data[n].texIndex = tex_index;
						}
					}
					else
					{
						grass_data[n].size += strength * _parameters->strength * dt;
						grass_data[n].texIndex = tex_index;
					}
				}

				clamp(grass_data[n].size, 0.0f, _parameters->scale);

				n++;
			}
		}

		engineAPI->world->GetTerrain().SetGrassBlades(rect.X, rect.Y, rect.Right, rect.Bottom, grass_data);
		Memory::Delete(grass_data);
	}

	void ActionPaintGrass::Undo()
	{
		Terrain::GrassBlade* cur_grass = NewArray<Terrain::GrassBlade>(tempPool, _undoRect.Width * 2 * _undoRect.Height);
		engineAPI->world->GetTerrain().GetGrassBlades(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, cur_grass);
		engineAPI->world->GetTerrain().SetGrassBlades(_undoRect.X, _undoRect.Y, _undoRect.Right, _undoRect.Bottom, _undoGrassData);
		memcpy(_undoGrassData, cur_grass, _undoRect.Width * 2 * _undoRect.Height * sizeof(Terrain::GrassBlade));
		Memory::Delete(cur_grass);
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
