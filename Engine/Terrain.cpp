
#include "BaseLib/Memory.h"
#include "EngineInternal.h"
#include "Engine/Common.h"
#include "Engine/RenderSystem.h"
#include "Engine/World.h"
#include "Terrain.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	Terrain::Terrain()
		: _patches(mapPool)
	{
	}

	bool Terrain::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();

		_patchIndexCount = PATCH_WIDTH * PATCH_HEIGHT * 6;
		_patchIndexBuf = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, _patchIndexCount * sizeof(ushort), 0, GL::USAGE_STATIC_DRAW);
		if(!_patchIndexBuf)
			return false;

		ushort* indices = (ushort*)_patchIndexBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(!indices)
		{
			Deinit();
			return false;
		}

		/*
			+---+
			|\ 2|
			| \ |
			|1 \|
			+---+
		*/
		int i = 0;
		for(int h = 0; h < PATCH_HEIGHT; ++h)
		{
			for(int w = 0; w < PATCH_WIDTH; ++w)
			{
				indices[0] = i + 0;
				indices[1] = i + 1;
				indices[2] = i + PATCH_WIDTH + 1;

				indices[3] = i + 1;
				indices[4] = i + PATCH_WIDTH + 2;
				indices[5] = i + PATCH_WIDTH + 1;

				indices += 6;
				i++;
			}

			i++;
		}

		if(!_patchIndexBuf->UnmapBuffer())
		{
			Deinit();
			return false;
		}

		return true;
	}

	void Terrain::Deinit()
	{
		if(_patchIndexBuf)
		{
			_renderer->DestroyBuffer(_patchIndexBuf);
			_patchIndexBuf = 0;
		}

		for(TerrainPatchList::Iterator it = _patches.Begin(); it != _patches.End(); ++it)
		{
			_renderer->DestroyBuffer(it->vertBuf);
			delete[] it->elevation;
		}
		_patches.Clear();

		_renderer = 0;
	}

	bool Terrain::AddPatch(float* heights)
	{
		if(_patches.GetCount() == MAX_PATCHES)
			return false;

		TerrainPatch patch;
		int vert_count = (PATCH_WIDTH + 1) * (PATCH_HEIGHT + 1);
		patch.vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, vert_count * sizeof(PatchVertex), 0, GL::USAGE_STATIC_DRAW);
		if(!patch.vertBuf)
			return false;

		PatchVertex* vertices = (PatchVertex*)patch.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(!vertices)
		{
			_renderer->DestroyBuffer(patch.vertBuf);
			return false;
		}

		patch.elevation = new(mapPool) float[vert_count];
		float x_pos = (float)_patches.GetCount() * PATCH_WIDTH;
		patch.boundBox.minPt.set(x_pos, heights? heights[0]: 0.0f, 0.0f);
		patch.boundBox.maxPt.set(x_pos + PATCH_WIDTH, heights? heights[0]: 0.0f, (float)PATCH_HEIGHT);

		int i = 0;
		for(int h = 0; h < PATCH_HEIGHT + 1; ++h)
		{
			for(int w = 0; w < PATCH_WIDTH + 1; ++w)
			{
				float elev = heights? heights[h * (PATCH_WIDTH + 1) + w]: 0.0f;

				vertices->position.x = (float)w;
				vertices->position.y = elev;
				vertices->position.z = (float)h;
				vertices->position.w = 1.0f;

				vertices->normal = vec3f::y_axis;

				++vertices;

				patch.elevation[i++] = elev;
				if(elev < patch.boundBox.minPt.y)
					patch.boundBox.minPt.y = elev;
				else if(elev > patch.boundBox.maxPt.y)
					patch.boundBox.maxPt.y = elev;
			}
		}

		if(!patch.vertBuf->UnmapBuffer())
		{
			_renderer->DestroyBuffer(patch.vertBuf);
			return false;
		}

		_patches.PushBack(patch);

		return true;
	}

	void Terrain::RemovePatch(size_t index)
	{
		assert(index >= 0 && index < _patches.GetCount());
		TerrainPatch& patch = _patches.GetByIndex(index);
		_renderer->DestroyBuffer(patch.vertBuf);
		delete[] patch.elevation;
		_patches.Remove(index);
	}

	bool Terrain::PickTerrainPoint(int screen_x, int screen_y, math3d::vec3f& point)
	{
		// calculate world space point and direction vector of the picking ray

		float vp_x, vp_y, vp_width, vp_height;
		_renderer->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec3f ndc(
			(2.0f * ((float)screen_x - vp_x) - vp_width) / vp_width,
			(2.0f * ((float)screen_y - vp_y) - vp_height) / vp_height,
			-1.0);
		mat4f view_proj = engineAPI.world->GetCamera().GetViewProjectionTransform();
		view_proj.inverse();
		vec3f ray_pt;
		transform(ray_pt, ndc, view_proj);
		vec3f ray_dir = ray_pt - engineAPI.world->GetCamera().GetPosition();
		ray_dir.normalize();

		// determine which terrain patches are potentialy intersected

		bool pt_inside = (ray_pt.z >= 0.0f && ray_pt.z <= (float)PATCH_HEIGHT);
		int start_i, end_i;

		if(pt_inside)
		{
			vec3f pt;
			start_i = int(ray_pt.x) / PATCH_WIDTH;
			if(	intersect_ray_plane(pt, ray_pt, ray_dir, vec4f(0.0f, 0.0f, -1.0f, 0.0f)) ||
				intersect_ray_plane(pt, ray_pt, ray_dir, vec4f(0.0f, 0.0f, 1.0f, (float)-PATCH_HEIGHT)))
			{
				end_i = int(pt.x) / PATCH_WIDTH;
			}
			else
			{
				end_i = (ray_dir.x > 0.0f)? _patches.GetCount() - 1: 0;
			}
		}
		else
		{
			vec3f pt;
			if(intersect_ray_plane(pt, ray_pt, ray_dir, vec4f(0.0f, 0.0f, -1.0f, 0.0f)))
			{
				start_i = int(pt.x) / PATCH_WIDTH;
			}
			else
			{
				return false;
			}

			if(intersect_ray_plane(pt, ray_pt, ray_dir, vec4f(0.0f, 0.0f, 1.0f, (float)-PATCH_HEIGHT)))
			{
				end_i = int(pt.x) / PATCH_WIDTH;
			}
			else
			{
				return false;
			}
		}

		if(	(start_i >= (int)_patches.GetCount() && end_i >= (int)_patches.GetCount()) ||
			(start_i < 0 && end_i < 0) )
		{
			return false;
		}

		start_i = min(max(start_i, 0), (int)_patches.GetCount() - 1);
		end_i = min(max(end_i, 0), (int)_patches.GetCount() - 1);

		if(start_i > end_i)
		{
			int t = start_i;
			start_i = end_i;
			end_i = t;
		}

		// go through potential terrain patches and find intersection

		int i = 0;
		bool has_inters = false;
		for(TerrainPatchList::ConstIterator it = _patches.Begin(); it != _patches.End(); ++it)
		{
			if(i >= start_i)
			{
				if(IntersectPatch(ray_pt, ray_dir, *it, point))
				{
					has_inters = true;
					break;
				}
			}

			if(++i > end_i)
				break;
		}

		return has_inters;
	}

	bool Terrain::IntersectPatch(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, math3d::vec3f& point)
	{
		float t;
		int inters_count = 0;
		vec2f pts[2];
		vec2f ray_pt_2d(ray_pt.x, ray_pt.z);
		vec2f ray_dir_2d(ray_dir.x, ray_dir.z);

		// check if ray starting point is inside patch in 2D

		if(	ray_pt_2d.x >= patch.boundBox.minPt.x &&
			ray_pt_2d.x <= patch.boundBox.maxPt.x &&
			ray_pt_2d.y >= patch.boundBox.minPt.z &&
			ray_pt_2d.y <= patch.boundBox.maxPt.z )
		{
			inters_count++;
			pts[0] = ray_pt_2d;
		}

		// find intersections of ray and patch in 2D

		if(fcmp_ne(ray_dir_2d.x, 0.0f))
		{
			t = (patch.boundBox.minPt.x - ray_pt_2d.x) / ray_dir_2d.x;
			pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
			if(	pts[inters_count].y >= patch.boundBox.minPt.z &&
				pts[inters_count].y <= patch.boundBox.maxPt.z )
			{
				inters_count++;
			}

			if(inters_count < 2)
			{
				t = (patch.boundBox.maxPt.x - ray_pt_2d.x) / ray_dir_2d.x;
				pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
				if(	pts[inters_count].y >= patch.boundBox.minPt.z &&
					pts[inters_count].y <= patch.boundBox.maxPt.z )
				{
					inters_count++;
				}
			}
		}

		if(fcmp_ne(ray_dir_2d.y, 0.0f))
		{
			if(inters_count < 2)
			{
				t = (patch.boundBox.minPt.z - ray_pt_2d.y) / ray_dir_2d.y;
				pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
				if(	pts[inters_count].x >= patch.boundBox.minPt.x &&
					pts[inters_count].x <= patch.boundBox.maxPt.x )
				{
					inters_count++;
				}

				if(inters_count < 2)
				{
					t = (patch.boundBox.maxPt.z - ray_pt_2d.y) / ray_dir_2d.y;
					pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
					if(	pts[inters_count].x >= patch.boundBox.minPt.x &&
						pts[inters_count].x <= patch.boundBox.maxPt.x )
					{
						inters_count++;
					}
				}
			}
		}

		if(inters_count == 0)
		{
			return false;
		}
		else if(inters_count == 1)
		{
			int i = int((pts[0].x - patch.boundBox.minPt.x) / PATCH_WIDTH) + 1;
			int j = int(pts[0].y / PATCH_HEIGHT) + 1;
			assert(i >= 0 && i <= PATCH_WIDTH);
			assert(j >= 0 && j <= PATCH_HEIGHT);

			vec3f pt(
				pts[0].x,
				patch.elevation[j * (PATCH_WIDTH + 1) + i],
				pts[0].y);

			if(fcmp_eq(dot(pt - ray_pt, ray_dir), 1.0f))
			{
				point = pt;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// point closer to ray start point should be the start point
			vec2f p1 = pts[0] - ray_pt_2d;
			vec2f p2 = pts[1] - ray_pt_2d;
			if(p1.length_sq() > p2.length_sq())
			{
				// swap points
				vec2f t = pts[0];
				pts[0] = pts[1];
				pts[1] = t;
			}

			// indices of start and end cell of the patch
			int i1, j1, i2, j2;
			i1 = int((pts[0].x - patch.boundBox.minPt.x) / PATCH_WIDTH);
			j1 = int(pts[0].y / PATCH_HEIGHT);
			i2 = int((pts[1].x - patch.boundBox.minPt.x) / PATCH_WIDTH);
			j2 = int(pts[1].y / PATCH_HEIGHT);
			assert(i1 >= 0 && i1 < PATCH_WIDTH);
			assert(j1 >= 0 && j1 < PATCH_HEIGHT);
			assert(i2 >= 0 && i2 < PATCH_WIDTH);
			assert(j2 >= 0 && j2 < PATCH_HEIGHT);

			// test for intersection all cells on the line connecting the start and end cell
			int dy = j2 - j1;
			int dx = i2 - i1;
			float t = 0.5f;                      // offset for rounding

			if(IntersectPatchCell(ray_pt, ray_dir, patch, i1, j1, point))
				return true;

			if(abs(dx) > abs(dy))						// slope < 1
			{
				float m = (float) dy / (float) dx;      // compute slope
				t += j1;
				dx = (dx < 0) ? -1 : 1;
				m *= dx;
				while(i1 != i2)
				{
					i1 += dx;                           // step to next x value
					t += m;                             // add slope to y value
					if(IntersectPatchCell(ray_pt, ray_dir, patch, i1, (int)t, point))
						return true;
				}
			}
			else										// slope >= 1
			{
				float m = (float) dx / (float) dy;      // compute slope
				t += i1;
				dy = (dy < 0) ? -1 : 1;
				m *= dy;
				while(j1 != j2)
				{
					j1 += dy;                           // step to next y value
					t += m;                             // add slope to x value
					if(IntersectPatchCell(ray_pt, ray_dir, patch, (int)t, j1, point))
						return true;
				}
			}

			return false;
		}
	}

	bool Terrain::IntersectPatchCell(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, int cell_x, int cell_y, math3d::vec3f& point)
	{
		int i = cell_y * PATCH_WIDTH + cell_x;

		vec3f triangle[3];
		triangle[0].set((float)cell_x, patch.elevation[i], (float)cell_y);
		triangle[1].set((float)cell_x + 1, patch.elevation[i + 1], (float)cell_y);
		triangle[2].set((float)cell_x, patch.elevation[i + PATCH_WIDTH + 1], (float)cell_y + 1);

		if(intersect_ray_triangle(point, ray_pt, ray_dir, triangle))
			return true;

		triangle[0].set((float)cell_x + 1, patch.elevation[i + 1], (float)cell_y);
		triangle[1].set((float)cell_x + 1, patch.elevation[i + PATCH_WIDTH + 2], (float)cell_y + 1);
		triangle[2].set((float)cell_x, patch.elevation[i + PATCH_WIDTH + 1], (float)cell_y + 1);

		if(intersect_ray_triangle(point, ray_pt, ray_dir, triangle))
			return true;

		return false;
	}

}
