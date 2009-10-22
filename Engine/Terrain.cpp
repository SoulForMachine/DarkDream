
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
	{
		_patchCount = 0;
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
			triangles are counter-clockwise

			+---+
		 Z^	|\ 2|
		  |	| \ |
		  |	|1 \|
		  |	+---+
		  +--------> X
		*/
		int i = 0;
		for(int h = 0; h < PATCH_HEIGHT; ++h)
		{
			for(int w = 0; w < PATCH_WIDTH; ++w)
			{
				indices[0] = i + 0;
				indices[1] = i + PATCH_WIDTH + 1;
				indices[2] = i + 1;

				indices[3] = i + 1;
				indices[4] = i + PATCH_WIDTH + 1;
				indices[5] = i + PATCH_WIDTH + 2;

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

		_patchCount = 0;
		_hlightPatch = 0;
		_texture = 0;
		_texTile = 32.0f;

		return true;
	}

	void Terrain::Deinit()
	{
		if(_patchIndexBuf)
		{
			_renderer->DestroyBuffer(_patchIndexBuf);
			_patchIndexBuf = 0;
		}

		for(int i = 0; i < _patchCount; ++i)
		{
			_renderer->DestroyBuffer(_patches[i].vertBuf);
			_renderer->DestroyBuffer(_patches[i].normalBuf);
			delete[] _patches[i].elevation;
		}

		_renderer = 0;

		engineAPI.textureManager->ReleaseTexture(_texture);
		_texture = 0;
	}

	/*
		If index is -1, new patch is added at the end.
		Returns an index of the new patch or -1 on error.
	*/
	int Terrain::AddPatch(int index, float* heights)
	{
		if(_patchCount == MAX_PATCHES)
			return -1;

		if(index == -1 || index > _patchCount)
			index = _patchCount;

		int n = _patchCount - index;
		if(n)
		{
			// move objects
			EntityHashMap& entities = engineAPI.world->GetEntities();
			for(EntityHashMap::Iterator it = entities.Begin(); it != entities.End(); ++it)
			{
				vec3f pos = (*it)->GetPosition();
				if(	pos.x >= _patches[index].boundBox.minPt.x &&
					pos.x <= _patches[index + n - 1].boundBox.maxPt.x &&
					pos.z >= 0 &&
					pos.z <= PATCH_HEIGHT )
				{
					pos.x += PATCH_WIDTH;
					(*it)->SetPosition(pos);
				}
			}

			TerrainPatch* tmp = new(tempPool) TerrainPatch[n];
			memcpy(tmp, &_patches[index], sizeof(TerrainPatch) * n);
			memcpy(&_patches[index + 1], tmp, sizeof(TerrainPatch) * n);
			delete[] tmp;
			// modify bounding boxes
			for(int i = 0; i < n; ++i)
			{
				_patches[index + 1 + i].boundBox.minPt.x = (float)(index + 1 + i) * PATCH_WIDTH;
				_patches[index + 1 + i].boundBox.maxPt.x = (float)(index + 1 + i) * PATCH_WIDTH + PATCH_WIDTH;
			}
		}

		TerrainPatch& patch = _patches[index];
		int vert_count = (PATCH_WIDTH + 1) * (PATCH_HEIGHT + 1);
		patch.vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, vert_count * sizeof(PatchVertex), 0, GL::USAGE_STATIC_DRAW);
		if(!patch.vertBuf)
			return -1;

		patch.normalBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, vert_count * sizeof(vec3f) * 2, 0, GL::USAGE_STATIC_DRAW);

		PatchVertex* vertices = (PatchVertex*)patch.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(!vertices)
		{
			_renderer->DestroyBuffer(patch.vertBuf);
			_renderer->DestroyBuffer(patch.normalBuf);
			delete[] patch.elevation;
			return -1;
		}

		patch.elevation = new(mapPool) float[vert_count];
		float x_pos = (float)index * PATCH_WIDTH;
		patch.boundBox.minPt.set(x_pos, heights? heights[0]: 0.0f, 0.0f);
		patch.boundBox.maxPt.set(x_pos + PATCH_WIDTH, heights? heights[0]: 0.0f, (float)PATCH_HEIGHT);
		PatchVertex* vert_ptr = vertices;

		int i = 0;
		for(int h = 0; h <= PATCH_HEIGHT; ++h)
		{
			for(int w = 0; w <= PATCH_WIDTH; ++w)
			{
				float elev;
				if(w == 0 && index > 0) // use hight from previous patch's edge
				{
					elev = _patches[index - 1].elevation[h * (PATCH_WIDTH + 1) + PATCH_WIDTH];
					/*if(heights)
						elev = (elev + heights[h * (PATCH_WIDTH + 1) + w]) * 0.5f;*/
				}
				else if(w == PATCH_WIDTH && index < _patchCount) // use hight from next patch's edge
				{
					elev = _patches[index + 1].elevation[h * (PATCH_WIDTH + 1)];
					/*if(heights)
						elev = (elev + heights[h * (PATCH_WIDTH + 1) + w]) * 0.5f;*/
				}
				else
				{
					elev = heights? heights[h * (PATCH_WIDTH + 1) + w]: 0.0f;
				}

				vert_ptr->position.x = (float)w;
				vert_ptr->position.y = elev;
				vert_ptr->position.z = (float)h;
				vert_ptr->position.w = 1.0f;

				vert_ptr->normal = vec4f(vec3f::y_axis, 0.0f);

				++vert_ptr;

				patch.elevation[i++] = elev;
				if(elev < patch.boundBox.minPt.y)
					patch.boundBox.minPt.y = elev;
				else if(elev > patch.boundBox.maxPt.y)
					patch.boundBox.maxPt.y = elev;
			}
		}

		_patchCount++;

		// if grid elevation is supplied, calculate the normals
		if(heights)
			UpdatePatchNormals(index, vertices, 0, 0, PATCH_WIDTH, PATCH_HEIGHT);

		if(index > 0)
		{
			// update normals on the edge of previous patch
			PatchVertex* prev_verts = (PatchVertex*)_patches[index - 1].vertBuf->MapBuffer(GL::ACCESS_READ_WRITE, false);
			if(prev_verts)
			{
				UpdatePatchNormals(index - 1, prev_verts, PATCH_WIDTH - 1, 0, PATCH_WIDTH, PATCH_HEIGHT);
				vec3f* normals = (vec3f*)_patches[index - 1].normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
				if(normals)
				{
					for(int h = 0; h <= PATCH_HEIGHT; ++h)
					{
						for(int w = PATCH_WIDTH - 1; w <= PATCH_WIDTH; ++w)
						{
							int i = h * (PATCH_WIDTH + 1) + w;
							normals[i * 2] = prev_verts[i].position.rvec3;
							normals[i * 2 + 1] = prev_verts[i].position.rvec3 + prev_verts[i].normal.rvec3;
						}
					}
					_patches[index - 1].normalBuf->UnmapBuffer();
				}
				_patches[index - 1].vertBuf->UnmapBuffer();
			}

			// update normals on the first two columns of new patch
			if(!heights)
				UpdatePatchNormals(index, vertices, 0, 0, 1, PATCH_HEIGHT);
		}

		if(index + 1 < _patchCount)
		{
			// update normals on the edge of next patch
			PatchVertex* next_verts = (PatchVertex*)_patches[index + 1].vertBuf->MapBuffer(GL::ACCESS_READ_WRITE, false);
			if(next_verts)
			{
				UpdatePatchNormals(index + 1, next_verts, 0, 0, 1, PATCH_HEIGHT);
				vec3f* normals = (vec3f*)_patches[index + 1].normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
				if(normals)
				{
					for(int h = 0; h <= PATCH_HEIGHT; ++h)
					{
						for(int w = 0; w <= 1; ++w)
						{
							int i = h * (PATCH_WIDTH + 1) + w;
							normals[i * 2] = next_verts[i].position.rvec3;
							normals[i * 2 + 1] = next_verts[i].position.rvec3 + next_verts[i].normal.rvec3;
						}
					}
					_patches[index + 1].normalBuf->UnmapBuffer();
				}
				_patches[index + 1].vertBuf->UnmapBuffer();
			}

			// update normals on the last two columns of new patch
			if(!heights)
				UpdatePatchNormals(index, vertices, PATCH_WIDTH - 1, 0, PATCH_WIDTH, PATCH_HEIGHT);
		}

		vec3f* normals = (vec3f*)patch.normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		for(int h = 0; h <= PATCH_HEIGHT; ++h)
		{
			for(int w = 0; w <= PATCH_WIDTH; ++w)
			{
				int i = h * (PATCH_WIDTH + 1) + w;
				normals[i * 2] = vertices[i].position.rvec3;
				normals[i * 2 + 1] = vertices[i].position.rvec3 + vertices[i].normal.rvec3;
			}
		}
		patch.normalBuf->UnmapBuffer();

		if(!patch.vertBuf->UnmapBuffer())
		{
			_renderer->DestroyBuffer(patch.vertBuf);
			_renderer->DestroyBuffer(patch.normalBuf);
			delete[] patch.elevation;
			_patchCount--;
			return -1;
		}

		_hlightPatch = 0;
		return _patchCount - 1;
	}

	void Terrain::RemovePatch(int index)
	{
		assert(index >= 0 && index < _patchCount);
		_renderer->DestroyBuffer(_patches[index].vertBuf);
		_renderer->DestroyBuffer(_patches[index].normalBuf);
		delete[] _patches[index].elevation;

		if(index < _patchCount - 1)
		{
			int n = _patchCount - 1 - index;
			if(n)
			{
				// move objects
				EntityHashMap& entities = engineAPI.world->GetEntities();
				for(EntityHashMap::Iterator it = entities.Begin(); it != entities.End(); ++it)
				{
					vec3f pos = (*it)->GetPosition();
					if(	pos.x >= _patches[index + 1].boundBox.minPt.x &&
						pos.x <= _patches[index + n].boundBox.maxPt.x &&
						pos.z >= 0 &&
						pos.z <= PATCH_HEIGHT )
					{
						pos.x -= PATCH_WIDTH;
						(*it)->SetPosition(pos);
					}
				}

				TerrainPatch* tmp = new(tempPool) TerrainPatch[n];
				memcpy(tmp, &_patches[index + 1], sizeof(TerrainPatch) * n);
				memcpy(&_patches[index], tmp, sizeof(TerrainPatch) * n);
				delete[] tmp;
				for(int i = 0; i < n; ++i)
				{
					// modify bounding boxes
					_patches[index + i].boundBox.minPt.x = (float)(index + i) * PATCH_WIDTH;
					_patches[index + i].boundBox.maxPt.x = (float)(index + i) * PATCH_WIDTH + PATCH_WIDTH;
				}
			}

			if(index > 0)
			{
				// weld edges of previous and next patch
				TerrainPatch& prev_patch = _patches[index - 1];
				TerrainPatch& next_patch = _patches[index];
				PatchVertex* prev_verts = (PatchVertex*)prev_patch.vertBuf->MapBuffer(GL::ACCESS_READ_WRITE, false);
				PatchVertex* next_verts = (PatchVertex*)next_patch.vertBuf->MapBuffer(GL::ACCESS_READ_WRITE, false);

				if(prev_verts && next_verts)
				{
					for(int h = 0; h <= PATCH_HEIGHT; ++h)
					{
						int i = h * (PATCH_WIDTH + 1);
						float height = (prev_patch.elevation[i + PATCH_WIDTH] + next_patch.elevation[i]) * 0.5f;

						prev_patch.elevation[i + PATCH_WIDTH] = height;
						if(height < prev_patch.boundBox.minPt.y)
							prev_patch.boundBox.minPt.y = height;
						else if(height > prev_patch.boundBox.maxPt.y)
							prev_patch.boundBox.maxPt.y = height;

						next_patch.elevation[i] = height;
						if(height < next_patch.boundBox.minPt.y)
							next_patch.boundBox.minPt.y = height;
						else if(height > next_patch.boundBox.maxPt.y)
							next_patch.boundBox.maxPt.y = height;

						prev_verts[i + PATCH_WIDTH].position.y = height;
						next_verts[i].position.y = height;
					}

					// update normals on the last two columns of previous patch
					UpdatePatchNormals(index - 1, prev_verts, PATCH_WIDTH - 1, 0, PATCH_WIDTH, PATCH_HEIGHT);
					vec3f* normals = (vec3f*)prev_patch.normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
					if(normals)
					{
						for(int h = 0; h <= PATCH_HEIGHT; ++h)
						{
							int i = h * (PATCH_WIDTH + 1) + PATCH_WIDTH;
							normals[i * 2] = prev_verts[i].position.rvec3;
							normals[i * 2 + 1] = prev_verts[i].position.rvec3 + prev_verts[i].normal.rvec3;
						}
						prev_patch.normalBuf->UnmapBuffer();
					}

					// update normals on the first two columns of next patch
					UpdatePatchNormals(index, next_verts, 0, 0, 1, PATCH_HEIGHT);
					normals = (vec3f*)next_patch.normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
					if(normals)
					{
						for(int h = 0; h <= PATCH_HEIGHT; ++h)
						{
							int i = h * (PATCH_WIDTH + 1);
							normals[i * 2] = next_verts[i].position.rvec3;
							normals[i * 2 + 1] = next_verts[i].position.rvec3 + next_verts[i].normal.rvec3;
						}
						next_patch.normalBuf->UnmapBuffer();
					}

					prev_patch.vertBuf->UnmapBuffer();
					next_patch.vertBuf->UnmapBuffer();
				}
			}
		}

		_patchCount--;
		_hlightPatch = 0;
	}

	/*
		Returns an index of the patch containing the point or -1
	*/
	int Terrain::PickTerrainPoint(int screen_x, int screen_y, math3d::vec3f& point)
	{
		// calculate world space point and direction vector of the picking ray

		float vp_x, vp_y, vp_width, vp_height;
		_renderer->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec4f ndc(
			(2.0f * ((float)screen_x - vp_x) - vp_width) / vp_width,
			(2.0f * ((float)screen_y - vp_y) - vp_height) / vp_height,
			-1.0f, 1.0f);
		mat4f view_proj = engineAPI.world->GetCamera().GetViewProjectionTransform();
		view_proj.inverse();
		vec3f ray_pt;
		vec4f temp;
		transform(temp, ndc, view_proj);
		ray_pt = temp.rvec3 / temp.w;
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
				end_i = (ray_dir.x > 0.0f)? _patchCount - 1: 0;
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
				return -1;
			}

			if(intersect_ray_plane(pt, ray_pt, ray_dir, vec4f(0.0f, 0.0f, 1.0f, (float)-PATCH_HEIGHT)))
			{
				end_i = int(pt.x) / PATCH_WIDTH;
			}
			else
			{
				return -1;
			}
		}

		if(	(start_i >= _patchCount && end_i >= _patchCount) ||
			(start_i < 0 && end_i < 0) )
		{
			return -1;
		}

		start_i = Min(Max(start_i, 0), _patchCount - 1);
		end_i = Min(Max(end_i, 0), _patchCount - 1);

		if(start_i > end_i)
		{
			int t = start_i;
			start_i = end_i;
			end_i = t;
		}

		// go through potential terrain patches and find intersection

		for(int i = start_i; i <= end_i; ++i)
		{
			if(IntersectPatch(ray_pt, ray_dir, _patches[i], point))
				return i;
		}

		return -1;
	}

	bool Terrain::IntersectPatch(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, math3d::vec3f& point)
	{
		float t;
		int inters_count = 0;
		vec2f pts[2];
		vec2f ray_pt_2d(ray_pt.x, ray_pt.z);
		vec2f ray_dir_2d(ray_dir.x, ray_dir.z);
		if(ray_dir_2d.length_sq() > 0.0f)
			ray_dir_2d.normalize();

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
			if(t >= 0.0f)
			{
				pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
				if(	pts[inters_count].y >= patch.boundBox.minPt.z &&
					pts[inters_count].y <= patch.boundBox.maxPt.z )
				{
					inters_count++;
				}
			}

			if(inters_count < 2)
			{
				t = (patch.boundBox.maxPt.x - ray_pt_2d.x) / ray_dir_2d.x;
				if(t >= 0.0f)
				{
					pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
					if(	pts[inters_count].y >= patch.boundBox.minPt.z &&
						pts[inters_count].y <= patch.boundBox.maxPt.z )
					{
						inters_count++;
					}
				}
			}
		}

		if(fcmp_ne(ray_dir_2d.y, 0.0f))
		{
			if(inters_count < 2)
			{
				t = (patch.boundBox.minPt.z - ray_pt_2d.y) / ray_dir_2d.y;
				if(t >= 0.0f)
				{
					pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
					if(	pts[inters_count].x >= patch.boundBox.minPt.x &&
						pts[inters_count].x <= patch.boundBox.maxPt.x )
					{
						inters_count++;
					}
				}

				if(inters_count < 2)
				{
					t = (patch.boundBox.maxPt.z - ray_pt_2d.y) / ray_dir_2d.y;
					if(t >= 0.0f)
					{
						pts[inters_count] = ray_pt_2d + ray_dir_2d * t;
						if(	pts[inters_count].x >= patch.boundBox.minPt.x &&
							pts[inters_count].x <= patch.boundBox.maxPt.x )
						{
							inters_count++;
						}
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
			int i = int(pts[0].x - patch.boundBox.minPt.x);
			int j = int(pts[0].y);
			assert(i >= 0 && i < PATCH_WIDTH);
			assert(j >= 0 && j < PATCH_HEIGHT);

			return IntersectPatchCell(ray_pt, ray_dir, patch, i, j, point);
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
			float x1, y1, x2, y2;
			x1 = pts[0].x - patch.boundBox.minPt.x;
			y1 = pts[0].y;
			x2 = pts[1].x - patch.boundBox.minPt.x;
			y2 = pts[1].y;

			// test all cells for intersection along the projected line
			float dx = x2 - x1;
			float dy = y2 - y1;

			int cell_x = Min((int)x1, PATCH_WIDTH - 1);
			int cell_y = Min((int)y1, PATCH_HEIGHT - 1);
			if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
				return true;
			
			if(fabs(dx) > fabs(dy))
			{
				float ystep = dy / dx;
				int xstep = (dx < 0)? -1: 1;
				ystep *= xstep;

				int ix1 = (int)x1, ix2 = (int)x2;
				if(ix1 == ix2)
					return false;
				ix1 += xstep;
				ix2 += xstep;

				float y = y1;
				if(dx > 0.0f)
					y += ystep * (1.0f - frac(x1));
				else
					y += ystep * frac(x1);
				float yprev = y1;

				while(ix1 != ix2)
				{
					if(Min(int(y), PATCH_HEIGHT - 1) != Min(int(yprev), PATCH_HEIGHT - 1))
					{
						cell_x = Min(ix1 - xstep, PATCH_WIDTH - 1);
						cell_y = Min(int(y), PATCH_HEIGHT - 1);
						if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
							return true;
					}

					cell_x = Min(ix1, PATCH_WIDTH - 1);
					cell_y = Min(int(y), PATCH_HEIGHT - 1);
					if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
						return true;

					ix1 += xstep;
					yprev = y;
					y += ystep;
				}

				if(Min(int(y), PATCH_HEIGHT - 1) != Min(int(yprev), PATCH_HEIGHT - 1))
				{
					cell_x = Min(ix1 - xstep, PATCH_WIDTH - 1);
					cell_y = Min(int(y), PATCH_HEIGHT - 1);
					if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
						return true;
				}
			}
			else
			{
				float xstep = dx / dy;
				int ystep = (dy < 0)? -1: 1;
				xstep *= ystep;

				int iy1 = (int)y1, iy2 = (int)y2;
				if(iy1 == iy2)
					return false;
				iy1 += ystep;
				iy2 += ystep;

				float x = x1;
				if(dy > 0.0f)
					x += xstep * (1.0f - frac(y1));
				else
					x += xstep * frac(y1);
				float xprev = x1;

				while(iy1 != iy2)
				{
					if(Min(int(x), PATCH_WIDTH - 1) != Min(int(xprev), PATCH_WIDTH - 1))
					{
						cell_x = Min(int(x), PATCH_WIDTH - 1);
						cell_y = Min(iy1 - ystep, PATCH_HEIGHT - 1);
						if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
							return true;
					}

					cell_x = Min(int(x), PATCH_WIDTH - 1);
					cell_y = Min(iy1, PATCH_HEIGHT - 1);
					if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
						return true;

					iy1 += ystep;
					xprev = x;
					x += xstep;
				}

				if(Min(int(x), PATCH_WIDTH - 1) != Min(int(xprev), PATCH_WIDTH - 1))
				{
					cell_x = Min(int(x), PATCH_WIDTH - 1);
					cell_y = Min(iy1 - ystep, PATCH_HEIGHT - 1);
					if(IntersectPatchCell(ray_pt, ray_dir, patch, cell_x, cell_y, point))
						return true;
				}
			}

			return false;
		}
	}

	bool Terrain::IntersectPatchCell(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, int cell_x, int cell_y, math3d::vec3f& point)
	{
		assert(cell_x >= 0 && cell_x < PATCH_WIDTH);
		assert(cell_y >= 0 && cell_y < PATCH_HEIGHT);

		int i = cell_y * (PATCH_WIDTH + 1) + cell_x;
		float x = (float)cell_x + patch.boundBox.minPt.x;

		vec3f triangle[3];
		triangle[0].set(x, patch.elevation[i], (float)cell_y);
		triangle[1].set(x, patch.elevation[i + PATCH_WIDTH + 1], (float)cell_y + 1);
		triangle[2].set(x + 1, patch.elevation[i + 1], (float)cell_y);

		if(intersect_ray_triangle(point, ray_pt, ray_dir, triangle))
			return true;

		triangle[0].set(x + 1, patch.elevation[i + 1], (float)cell_y);
		triangle[1].set(x, patch.elevation[i + PATCH_WIDTH + 1], (float)cell_y + 1);
		triangle[2].set(x + 1, patch.elevation[i + PATCH_WIDTH + 2], (float)cell_y + 1);

		if(intersect_ray_triangle(point, ray_pt, ray_dir, triangle))
			return true;

		return false;
	}

	bool Terrain::ElevationFromPoint(const vec2f& point, float& elevation)
	{
		if(	point.x < 0.0f || point.x > _patchCount * PATCH_WIDTH ||
			point.y < 0.0f || point.y > PATCH_HEIGHT )
		{
			return false;
		}

		int patch_i = Min(int(point.x) / PATCH_WIDTH, _patchCount - 1);
		TerrainPatch& patch = _patches[patch_i];
		int cell_x = int(point.x - patch.boundBox.minPt.x);
		int cell_y = int(point.y);
		int i = cell_y * (PATCH_WIDTH + 1) + cell_x;
		float x = (float)cell_x + patch.boundBox.minPt.x;

		vec3f triangle[3];
		triangle[0].set(x, patch.elevation[i], (float)cell_y);
		triangle[1].set(x, patch.elevation[i + PATCH_WIDTH + 1], (float)cell_y + 1);
		triangle[2].set(x + 1, patch.elevation[i + 1], (float)cell_y);

		vec2f triangle2d[3];
		triangle2d[0].set(triangle[0].x, triangle[0].z);
		triangle2d[1].set(triangle[1].x, triangle[1].z);
		triangle2d[2].set(triangle[2].x, triangle[2].z);

		if(point_in_triangle_2d(point, triangle2d))
		{
			vec4f plane;
			plane_from_triangle(plane, triangle);
			elevation = -(plane.x * point.x + plane.z * point.y + plane.w) / plane.y;
		}
		else
		{
			triangle[0].set(x + 1, patch.elevation[i + 1], (float)cell_y);
			triangle[1].set(x, patch.elevation[i + PATCH_WIDTH + 1], (float)cell_y + 1);
			triangle[2].set(x + 1, patch.elevation[i + PATCH_WIDTH + 2], (float)cell_y + 1);

			vec4f plane;
			plane_from_triangle(plane, triangle);
			elevation = -(plane.x * point.x + plane.z * point.y + plane.w) / plane.y;
		}

		return true;
	}

	void Terrain::SetElevation(int start_x, int start_y, int end_x, int end_y, const float* elevation)
	{
		const int X_COUNT = PATCH_WIDTH + 1;
		const int PART_X_COUNT = end_x - start_x + 1;
		int i1 = Max(start_x - 1, 0) / PATCH_WIDTH; // index of first patch
		int i2 = Min(end_x / PATCH_WIDTH, _patchCount - 1); // index of last patch
		int start_src_x = 0;

		for(int i = i1; i <= i2; ++i)
		{
			TerrainPatch& patch = _patches[i];

			int x1 = Max(start_x, i * PATCH_WIDTH) - i * PATCH_WIDTH;
			int x2 = Min(end_x, (i + 1) * PATCH_WIDTH) - i * PATCH_WIDTH;
			int y1 = start_y;
			int y2 = end_y;

			for(int y = y1, src_y = 0; y <= y2; ++y, ++src_y)
			{
				for(int x = x1, src_x = start_src_x; x <= x2; ++x, ++src_x)
				{
					float elev = elevation[src_y * PART_X_COUNT + src_x];
					patch.elevation[y * X_COUNT + x] = elev;

					if(elev < patch.boundBox.minPt.y)
						patch.boundBox.minPt.y = elev;
					else if(elev > patch.boundBox.maxPt.y)
						patch.boundBox.maxPt.y = elev;
				}
			}

			start_src_x += x2 - x1;
		}

		for(int i = i1; i <= i2; ++i)
		{
			TerrainPatch& patch = _patches[i];

			int x1 = Max(start_x, i * PATCH_WIDTH) - i * PATCH_WIDTH;
			int x2 = Min(end_x, (i + 1) * PATCH_WIDTH) - i * PATCH_WIDTH;
			int y1 = start_y;
			int y2 = end_y;

			PatchVertex* vertices = (PatchVertex*)patch.vertBuf->MapBuffer(GL::ACCESS_READ_WRITE, false);
			if(vertices)
			{
				for(int y = y1; y <= y2; ++y)
				{
					for(int x = x1; x <= x2; ++x)
					{
						vertices[y * X_COUNT + x].position.y = patch.elevation[y * X_COUNT + x];
					}
				}

				UpdatePatchNormals(i, vertices, Max(x1 - 1, 0), Max(y1 - 1, 0), Min(x2 + 1, PATCH_WIDTH), Min(y2 + 1, PATCH_HEIGHT));

				vec3f* normals = (vec3f*)patch.normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
				for(int y = Max(y1 - 1, 0); y <= Min(y2 + 1, PATCH_HEIGHT); ++y)
				{
					for(int x = Max(x1 - 1, 0); x <= Min(x2 + 1, PATCH_WIDTH); ++x)
					{
						normals[(y * X_COUNT + x) * 2 + 0] = vertices[y * X_COUNT + x].position.rvec3;
						normals[(y * X_COUNT + x) * 2 + 1] = vertices[y * X_COUNT + x].position.rvec3 + vertices[y * X_COUNT + x].normal.rvec3;
					}
				}
				patch.normalBuf->UnmapBuffer();

				patch.vertBuf->UnmapBuffer();
			}
		}
	}

	void Terrain::OffsetElevation(int start_x, int start_y, int end_x, int end_y, const float* offsets)
	{
		const int X_COUNT = PATCH_WIDTH + 1;
		const int PART_X_COUNT = end_x - start_x + 1;
		int i1 = Max(start_x - 1, 0) / PATCH_WIDTH; // index of first patch
		int i2 = Min(end_x / PATCH_WIDTH, _patchCount - 1); // index of last patch
		int start_src_x = 0;

		for(int i = i1; i <= i2; ++i)
		{
			TerrainPatch& patch = _patches[i];

			int x1 = Max(start_x, i * PATCH_WIDTH) - i * PATCH_WIDTH;
			int x2 = Min(end_x, (i + 1) * PATCH_WIDTH) - i * PATCH_WIDTH;
			int y1 = start_y;
			int y2 = end_y;

			for(int y = y1, src_y = 0; y <= y2; ++y, ++src_y)
			{
				for(int x = x1, src_x = start_src_x; x <= x2; ++x, ++src_x)
				{
					float elev = patch.elevation[y * X_COUNT + x] + offsets[src_y * PART_X_COUNT + src_x];
					patch.elevation[y * X_COUNT + x] = elev;

					if(elev < patch.boundBox.minPt.y)
						patch.boundBox.minPt.y = elev;
					else if(elev > patch.boundBox.maxPt.y)
						patch.boundBox.maxPt.y = elev;
				}
			}

			start_src_x += x2 - x1;
		}

		for(int i = i1; i <= i2; ++i)
		{
			TerrainPatch& patch = _patches[i];

			int x1 = Max(start_x, i * PATCH_WIDTH) - i * PATCH_WIDTH;
			int x2 = Min(end_x, (i + 1) * PATCH_WIDTH) - i * PATCH_WIDTH;
			int y1 = start_y;
			int y2 = end_y;

			PatchVertex* vertices = (PatchVertex*)patch.vertBuf->MapBuffer(GL::ACCESS_READ_WRITE, false);
			if(vertices)
			{
				for(int y = y1; y <= y2; ++y)
				{
					for(int x = x1; x <= x2; ++x)
					{
						vertices[y * X_COUNT + x].position.y = patch.elevation[y * X_COUNT + x];
					}
				}

				UpdatePatchNormals(i, vertices, Max(x1 - 1, 0), Max(y1 - 1, 0), Min(x2 + 1, PATCH_WIDTH), Min(y2 + 1, PATCH_HEIGHT));

				vec3f* normals = (vec3f*)patch.normalBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
				for(int y = Max(y1 - 1, 0); y <= Min(y2 + 1, PATCH_HEIGHT); ++y)
				{
					for(int x = Max(x1 - 1, 0); x <= Min(x2 + 1, PATCH_WIDTH); ++x)
					{
						normals[(y * X_COUNT + x) * 2 + 0] = vertices[y * X_COUNT + x].position.rvec3;
						normals[(y * X_COUNT + x) * 2 + 1] = vertices[y * X_COUNT + x].position.rvec3 + vertices[y * X_COUNT + x].normal.rvec3;
					}
				}
				patch.normalBuf->UnmapBuffer();

				patch.vertBuf->UnmapBuffer();
			}
		}
	}

	void Terrain::GetElevation(int start_x, int start_y, int end_x, int end_y, float* elevation)
	{
		const int X_COUNT = PATCH_WIDTH + 1;
		const int DEST_X_COUNT = end_x - start_x + 1;
		int i1 = Min(start_x / PATCH_WIDTH, _patchCount - 1); // index of first patch
		int i2 = Max(end_x - 1, 0) / PATCH_WIDTH; // index of last patch
		int start_dest_x = 0;

		for(int i = i1; i <= i2; ++i)
		{
			TerrainPatch& patch = _patches[i];

			int x1 = Max(start_x, i * PATCH_WIDTH) - i * PATCH_WIDTH;
			if(x1 == 0 && i > i1) // skip first column because it's the same as the last on previous patch
				++x1;
			int x2 = Min(end_x, (i + 1) * PATCH_WIDTH) - i * PATCH_WIDTH;
			int y1 = start_y;
			int y2 = end_y;

			for(int y = y1, dest_y = 0; y <= y2; ++y, ++dest_y)
			{
				for(int x = x1, dest_x = start_dest_x; x <= x2; ++x, ++dest_x)
				{
					elevation[dest_y * DEST_X_COUNT + dest_x] = patch.elevation[y * X_COUNT + x];
				}
			}

			start_dest_x += x2 - x1 + 1;
		}
	}

	void Terrain::SetTexture(const TextureRes* texture)
	{
		engineAPI.textureManager->ReleaseTexture(_texture);
		_texture = texture;
	}

	void Terrain::UpdatePatchNormals(int patch_index, PatchVertex* vertices, int start_x, int start_y, int end_x, int end_y)
	{
		// calculate triangle normals

		bool left_ext, right_ext, up_ext, down_ext;

		int x1 = start_x - 1;
		if(x1 < 0)
		{
			if(patch_index > 0)
			{
				left_ext = true;
			}
			else
			{
				left_ext = false;
				x1 = 0;
			}
		}
		else
		{
			left_ext = true;
		}

		int x2 = end_x;
		if(x2 >= PATCH_WIDTH)
		{
			if(patch_index < _patchCount - 1)
			{
				right_ext = true;
			}
			else
			{
				right_ext = false;
				x2 = PATCH_WIDTH - 1;
			}
		}
		else
		{
			right_ext = true;
		}

		int y1 = start_y - 1;
		if(y1 < 0)
		{
			down_ext = false;
			y1 = 0;
		}
		else
		{
			down_ext = true;
		}

		int y2 = end_y;
		if(y2 >= PATCH_HEIGHT)
		{
			up_ext = false;
			y2 = PATCH_HEIGHT - 1;
		}
		else
		{
			up_ext = true;
		}

		int width = x2 - x1 + 1;
		int height = y2 - y1 + 1;
		vec3f* tri_normals = new(tempPool) vec3f[width * height * 2];
		vec3f triangle[3];
		int n = 0;

		for(int y = y1; y <= y2; ++y)
		{
			for(int x = x1; x <= x2; ++x)
			{
				int i;
				TerrainPatch* patch;

				if(x < 0)
				{
					patch = &_patches[patch_index - 1];
					i = y * (PATCH_WIDTH + 1) + PATCH_WIDTH - 1;
				}
				else if(x >= PATCH_WIDTH)
				{
					patch = &_patches[patch_index + 1];
					i = y * (PATCH_WIDTH + 1);
				}
				else
				{
					patch = &_patches[patch_index];
					i = y * (PATCH_WIDTH + 1) + x;
				}

				triangle[0].set((float)x, patch->elevation[i], (float)y);
				triangle[1].set((float)x, patch->elevation[i + PATCH_WIDTH + 1], (float)y + 1);
				triangle[2].set((float)x + 1, patch->elevation[i + 1], (float)y);
				triangle_normal(tri_normals[n++], triangle);

				triangle[0].set((float)x + 1, patch->elevation[i + 1], (float)y);
				triangle[1].set((float)x, patch->elevation[i + PATCH_WIDTH + 1], (float)y + 1);
				triangle[2].set((float)x + 1, patch->elevation[i + PATCH_WIDTH + 2], (float)y + 1);
				triangle_normal(tri_normals[n++], triangle);
			}
		}

		// calculate vertex normals
		int xext = left_ext? 1: 0;
		int yext = down_ext? 1: 0;

		for(int y = start_y; y <= end_y; ++y)
		{
			int qy = y - start_y + yext; // quad y index

			for(int x = start_x; x <= end_x; ++x)
			{
				int qx = x - start_x + xext; // quad x index

				if(x == start_x && !left_ext)
				{
					if(y == start_y && !down_ext)
					{
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = tri_normals[0];
					}
					else if(y == end_y && !up_ext)
					{
						vec3f normal = 
							tri_normals[(height - 1) * width * 2] +
							tri_normals[(height - 1) * width * 2 + 1];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 2.0f;
					}
					else
					{
						vec3f normal = 
							tri_normals[(qy - 1) * width * 2] +
							tri_normals[(qy - 1) * width * 2 + 1] +
							tri_normals[qy * width * 2];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 3.0f;
					}
				}
				else if(x == end_x && !right_ext)
				{
					if(y == start_y && !down_ext)
					{
						vec3f normal = 
							tri_normals[(width - 1) * 2] +
							tri_normals[(width - 1) * 2 + 1];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 2.0f;
					}
					else if(y == end_y && !up_ext)
					{
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = tri_normals[(width * height - 1) * 2 + 1];
					}
					else
					{
						vec3f normal = 
							tri_normals[((qy - 1) * width + width - 1) * 2 + 1] +
							tri_normals[(qy * width + width - 1) * 2] +
							tri_normals[(qy * width + width - 1) * 2 + 1];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 3.0f;
					}
				}
				else
				{
					if(y == start_y && !down_ext)
					{
						vec3f normal = 
							tri_normals[(qx - 1) * 2] +
							tri_normals[(qx - 1) * 2 + 1] +
							tri_normals[qx * 2];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 3.0f;
					}
					else if(y == end_y && !up_ext)
					{
						vec3f normal = 
							tri_normals[((height - 1) * width + qx - 1) * 2 + 1] +
							tri_normals[((height - 1) * width + qx) * 2] +
							tri_normals[((height - 1) * width + qx) * 2 + 1];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 3.0f;
					}
					else
					{
						vec3f normal = 
							tri_normals[(qy * width + (qx - 1)) * 2] +
							tri_normals[(qy * width + (qx - 1)) * 2 + 1] +
							tri_normals[((qy - 1) * width + (qx - 1)) * 2 + 1] +
							tri_normals[((qy - 1) * width + qx) * 2] +
							tri_normals[((qy - 1) * width + qx) * 2 + 1] +
							tri_normals[(qy * width + qx) * 2];
						vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3 = normal / 6.0f;
					}
				}

				vertices[y * (PATCH_WIDTH + 1) + x].normal.rvec3.normalize();
			}
		}

		// 
		for(int y = start_y; y <= end_y; ++y)
		{
			for(int x = start_x; x <= end_x; ++x)
			{
				int i = y * (PATCH_WIDTH + 1) + x;
				if(y < PATCH_HEIGHT && y > 0)
				{
					if(	_patches[patch_index].elevation[i] > _patches[patch_index].elevation[i + (PATCH_WIDTH + 1)] &&
						_patches[patch_index].elevation[i] > _patches[patch_index].elevation[i - (PATCH_WIDTH + 1)] )
					{
						float val = _patches[patch_index].elevation[i] - _patches[patch_index].elevation[i + (PATCH_WIDTH + 1)];
						clamp(val, 0.0f, 1.0f);
						vertices[i].normal.w = val;
					}
					else
					{
						vertices[i].normal.w = 0.0f;
					}
				}
				else
				{
					vertices[i].normal.w = 0.0f;
				}
			}
		}

		delete[] tri_normals;
	}

}
