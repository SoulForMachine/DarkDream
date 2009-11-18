
#include "BaseLib/Math/math3d.h"
#include "EngineInternal.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "ParticleRenderer.h"

using namespace math3d;


namespace Engine
{

	ParticleRenderer::ParticleRenderer()
		: _batchPool(MAX_BATCHES),
		_batchList(_batchPool)
	{
		Clear();
	}

	bool ParticleRenderer::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();

		_partVertexBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, MAX_VERTICES * sizeof(ParticleVertex), 0, GL::USAGE_DYNAMIC_DRAW);
		if(!_partVertexBuf)
			return false;

		_partIndexBuf = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, MAX_VERTICES / 4 * 6 * sizeof(ushort), 0, GL::USAGE_STATIC_DRAW);
		if(!_partIndexBuf)
		{
			Deinit();
			return false;
		}

		ushort* indices = (ushort*)_partIndexBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(!indices)
		{
			Deinit();
			return false;
		}

		for(int i = 0; i < MAX_VERTICES / 4; ++i)
		{
			indices[0] = i * 4 + 0;
			indices[1] = i * 4 + 1;
			indices[2] = i * 4 + 3;
			indices[3] = i * 4 + 1;
			indices[4] = i * 4 + 2;
			indices[5] = i * 4 + 3;

			indices += 6;
		}

		if(!_partIndexBuf->UnmapBuffer())
		{
			Deinit();
			return false;
		}

		GL::VertexAttribDesc fmt_desc[] =
		{
			{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 },
			{ 0, 1, 2, GL::TYPE_FLOAT, false, false, 12 },
			{ 0, 2, 1, GL::TYPE_FLOAT, false, false, 20 },
		};
		_partVertFmt = _renderer->CreateVertexFormat(fmt_desc, COUNTOF(fmt_desc));

		GL::SamplerStateDesc sampl_desc = GL::GLState::defaultSamplerState;
		sampl_desc.addressU = GL::TEX_ADDRESS_REPEAT;
		sampl_desc.addressV = GL::TEX_ADDRESS_REPEAT;
		sampl_desc.minFilter = GL::TEX_FILTER_LINEAR_MIPMAP_LINEAR;
		sampl_desc.magFilter = GL::TEX_FILTER_LINEAR;
		_partTexState = _renderer->CreateSamplerState(sampl_desc);

		_partVertProg = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Particle.vp"), true);
		_partFragProg = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Particle.fp"), true);

		return true;
	}

	void ParticleRenderer::Deinit()
	{
		_renderer->DestroyBuffer(_partVertexBuf);
		_renderer->DestroyBuffer(_partIndexBuf);
		_renderer->DestroyVertexFormat(_partVertFmt);
		_renderer->DestroySamplerState(_partTexState);
		engineAPI.asmProgManager->ReleaseASMProgram(_partVertProg);
		engineAPI.asmProgManager->ReleaseASMProgram(_partFragProg);

		Clear();
	}

	void ParticleRenderer::Render(const Camera& camera, ParticleSystem::Emitter** emitters, int count)
	{
		_camera = &camera;
		ParticleSystem::Emitter** em_ptr = emitters;

		_renderer->VertexSource(0, _partVertexBuf, sizeof(ParticleVertex), 0);
		_renderer->IndexSource(_partIndexBuf, GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexFormat(_partVertFmt);
		_renderer->ActiveVertexASMProgram(_partVertProg->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_partFragProg->GetASMProgram());
		_renderer->EnableBlending(true);
		_renderer->EnableDepthTest(false);

		_partVertProg->GetASMProgram()->LocalMatrix4x4(0, camera.GetViewProjectionTransform());

		do
		{
			int n = BatchEmitters(em_ptr, count);
			if(n == -1)
				break;

			count -= n;
			em_ptr += n;

			RenderBatches();
		}
		while(count);

		_renderer->EnableBlending(false);
		_renderer->EnableDepthTest(true);
	}

	int ParticleRenderer::BatchEmitters(ParticleSystem::Emitter** emitters, int count)
	{
		ParticleVertex* vertices = (ParticleVertex*)_partVertexBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(!vertices)
			return -1;

		_batchList.Reset();
		_vertexCount = 0;
		int start;
		int i = 0;
		bool full_buf = false;

		while(i < count && _batchList.GetCount() < MAX_BATCHES && !full_buf)
		{
			const TextureRes* last_tex = emitters[i]->GetTexture();
			ParticleSystem::Emitter::EmitterShader last_shader = emitters[i]->GetShader();

			uint vert_count = emitters[i]->GetParticleCount() * 4;
			start = i++;
			while(i < count &&
				emitters[i]->GetTexture() == last_tex &&
				emitters[i]->GetShader() == last_shader)
			{
				++i;

				uint vc = vert_count + emitters[i]->GetParticleCount() * 4;
				if(_vertexCount + vc > MAX_VERTICES)
				{
					--i;
					full_buf = true;
					break;
				}

				vert_count = vc;
			}

			if(i - start > 0)
			{
				ParticleBatch pb =
				{
					_vertexCount,
					vert_count,
					last_tex,
					last_shader
				};

				_batchList.PushBack(pb);
				FillBatchGeometry(&vertices[_vertexCount], &emitters[start], i - start);
				_vertexCount += vert_count;
			}
		}

		if(!_partVertexBuf->UnmapBuffer())
			return -1;

		return i;
	}

	void ParticleRenderer::FillBatchGeometry(ParticleVertex* vertices, ParticleSystem::Emitter** emitters, int count)
	{
		mat4f cam_mat = _camera->GetViewingTransform();

		for(int i = 0; i < count; ++i)
		{
			int part_count = emitters[i]->GetParticleCount();
			const ParticleSystem::Particle* const * particles = emitters[i]->GetParticles();

			for(int j = 0; j < part_count; ++j)
			{
				vec3f vec_x = cam_mat.get_col(0).rvec3 * particles[j]->size;
				vec3f vec_y = cam_mat.get_col(1).rvec3 * particles[j]->size;
				float u1, u2;
				if(emitters[i]->IsAnimatedTex())
				{
					float frame_w = 1.0f / (float)emitters[i]->GetTextureFrameCount();
					u1 = int(particles[j]->frame) * frame_w;
					u2 = u1 + frame_w;
				}
				else
				{
					u1 = 0.0f;
					u2 = 1.0f;
				}

				vertices[0].position = particles[j]->position + (- vec_x - vec_y);
				vertices[0].uv.set(u1, 1.0f);
				vertices[0].alpha = particles[j]->alpha;

				vertices[1].position = particles[j]->position + (vec_x - vec_y);
				vertices[1].uv.set(u2, 1.0f);
				vertices[1].alpha = particles[j]->alpha;

				vertices[2].position = particles[j]->position + (vec_x + vec_y);
				vertices[2].uv.set(u2, 0.0f);
				vertices[2].alpha = particles[j]->alpha;

				vertices[3].position = particles[j]->position + (- vec_x + vec_y);
				vertices[3].uv.set(u1, 0.0f);
				vertices[3].alpha = particles[j]->alpha;

				vertices += 4;
			}
		}
	}

	void ParticleRenderer::RenderBatches()
	{
		for(FreeList<ParticleBatch>::ConstIterator it = _batchList.Begin(); it != _batchList.End(); ++it)
		{
			const ParticleBatch& batch = *it;

			if(batch.texture)
			{
				if(batch.shader == ParticleSystem::Emitter::EMITTER_SHADER_BLEND)
					_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
				else
					_renderer->BlendingFunc(GL::BLEND_FUNC_ONE, GL::BLEND_FUNC_ONE);

				_renderer->SetSamplerState(0, _partTexState);
				_renderer->SetSamplerTexture(0, batch.texture->GetTexture());

				_renderer->DrawIndexed(
					GL::PRIM_TRIANGLES,
					batch.vertexOffset,
					batch.vertexOffset + batch.vertexCount,
					batch.vertexOffset / 4 * 6 * sizeof(ushort),
					batch.vertexCount / 4 * 6);
			}
		}

		_renderer->SetSamplerTexture(0, 0);
	}

	void ParticleRenderer::Clear()
	{
		_renderer = 0;
		_partVertexBuf = 0;
		_partIndexBuf = 0;
		_partVertFmt = 0;
		_partTexState = 0;
		_partVertProg = 0;
		_partFragProg = 0;
		_vertexCount = 0;
		_camera = 0;
	}

}
