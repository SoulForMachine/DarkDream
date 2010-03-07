
#ifndef _ENGINE_PARTICLE_RENDERER_H_
#define _ENGINE_PARTICLE_RENDERER_H_

#include "BaseLib/GL/GLRenderer.h"
#include "BaseLib/FreeList.h"
#include "Engine/Common.h"
#include "Engine/ParticleSystem.h"


namespace Engine
{

	class Camera;


	class ENGINE_API ParticleRenderer
	{
	public:
		ParticleRenderer();

		bool Init();
		void Deinit();

		void Render(const Camera& camera, ParticleSystem::Emitter** emitters, int count);

	private:
		static const int MAX_BATCHES = 64;
		static const int MAX_VERTICES = 1 << 16;

		struct ParticleBatch
		{
			uint vertexOffset;	// index of first vertex in buffer
			uint vertexCount;	// vertex count for this batch
			Texture2DResPtr texture;
			ParticleSystem::Emitter::EmitterShader shader;
		};

		struct ParticleVertex
		{
			math3d::vec3f position;
			math3d::vec2f uv;
			float alpha;
		};

		void Clear();
		int BatchEmitters(ParticleSystem::Emitter** emitters, int count);
		void FillBatchGeometry(ParticleVertex* vertices, ParticleSystem::Emitter** emitters, int count);
		void RenderBatches();

		FreePool<FreeListNode<ParticleBatch> > _batchPool;
		FreeList<ParticleBatch> _batchList;

		GL::Renderer* _renderer;
		GL::Buffer* _partVertexBuf;
		GL::Buffer* _partIndexBuf;
		GL::VertexFormat* _partVertFmt;
		GL::SamplerState* _partTexState;
		VertexASMProgResPtr _partVertProg;
		FragmentASMProgResPtr _partFragProg;
		uint _vertexCount;
		const Camera* _camera;
	};

}


#endif // _ENGINE_PARTICLE_RENDERER_H_
