
#ifndef _RENDERING_CONTEXT_H_
#define _RENDERING_CONTEXT_H_

#include "GLCommon.h"
#include "GLFramebuffer.h"
#include "GLBuffer.h"
#include "GLVertexFormat.h"
#include "GLASMProgram.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include "GLSamplerState.h"
#include "GLQuery.h"


namespace GL
{

	struct FramebufferFormat
	{
		ColorBufferType colorBufferType;
		bool sRGB;
		bool doubleBuffer;
		int colorBits;
		int depthBits;
		int stencilBits;
		int multisampleSamples;
		SwapMethod swapMethod;
	};

	struct ContextInfo
	{
		const char* extensions;
		const char* wglExtensions;
		int versionMajor;
		int versionMinor;
		const char* vendor;
		const char* renderer;
		const char* glslVersion;

		FramebufferFormat framebufferFormat;

		// extensions
		
		// v1.2
		bool extDrawRangeElements;
		bool extTexture3D;
		bool extBlendMinmax;
		bool extBlendSubtract;

		// v1.3
		bool extMultisample;
		bool extMultitexture;
		bool extTextureBorderClamp;
		bool extTextureCompression;
		bool extTextureCubeMap;
		bool extTextureCompressionS3TC;

		// v1.4
		bool extDepthTexture;
		bool extShadow;
		bool extMultiDrawArrays;
		bool extTextureLodBias;
		bool extGenerateMipmap;
		bool extBlendFuncSeparate;
		bool extBlendColor;

		// v1.5
		bool extOcclusionQuery;
		bool extVertexBufferObject;
		bool extShadowFuncs;

		// v2.0
		bool extVersion_2_0;
		bool extDrawBuffers;
		bool extTextureNonPowerOfTwo;
		bool extBlendEquationSeparate;

		// v2.1
		bool extVersion_2_1;
		bool extPixelBufferObject;

		// v3.0
		bool extTextureFloat;
		bool extFramebufferObject;
		bool extColorBufferFloat;
		bool extGPUShader4;
		bool extTransformFeedback;
		bool extDrawBuffers2;
		bool extFramebufferSRGB;
		bool extFramebufferMultisample;
		bool extFramebufferBlit;
		bool extConditionalRender;
		bool extMapBufferRange;
		bool extTextureCompressionRGTC;

		// v3.1
		bool extCopyBuffer;
		bool extUniformBufferObject;
		bool extTextureBufferObject;
		bool extDrawInstanced;
		bool extTextureRectangle;
		bool extPrimitiveRestart;

		// ARB programs
		bool extVertexProgram;
		bool extVertexProgram1_1;
		bool extVertexProgram2;
		bool extVertexProgram2Option;
		bool extVertexProgram3;
	//	bool extVertexProgram4;
		bool extFragmentProgram;
		bool extFragmentProgramOption;
		bool extFragmentProgram2;
	//	bool extFragmentProgram4;
		bool extFragmentProgramShadow;
		bool extGPUProgram4;
	//	bool extGeometryProgram4;
		bool extGPUProgramParameters;

		// other
		bool extTextureFilterAnisotropic;
		bool extInstancedArrays;
		bool extGeometryShader4;

		// wgl
		bool wextExtensionsString;
		bool wextPixelFormat;
		bool wextPixelFormatFloat; //extColorBufferFloat
		bool wextMultisample;
		bool wextSwapControl;
		bool wextFramebufferSRGB;
		bool wextCreateContext;

		// GL_EXT_draw_range_elements
		int maxRecomendedVertexCount;
		int maxRecomendedIndexCount;

		int maxTextureSize;
		int max3DTextureSize;
		int maxCubeMapSize;
		int maxRectangleTextureSize;
		float maxTextureLODBias;
		int maxViewportWidth;
		int maxViewportHeight;
		int maxVertexAttribs;
		int maxTextureCoords;
		int maxTextureImageUnits; // combined
		float maxTextureAnisotropy;
		int maxDrawBuffers;
		int maxTextureBufferSize;
		int maxElementVertices;
		int maxElementIndices;
		int maxUniformBlockSize;
		int maxCombinedUniformBlocks;
	};


	class Renderer;

	BASELIB_API Renderer* CreateRenderer(uint version, void* instance_handle, void* window_handle, const FramebufferFormat& format);
	BASELIB_API void DestroyRenderer(Renderer* renderer);



	class BASELIB_API Renderer
	{
	public:
		bool SetCurrentContext();
		const ContextInfo& GetInfo() const
			{ return _info; }

		// vertex stream
		void VertexSource(int stream, const Buffer* buffer, size_t stride, size_t offset);
		void IndexSource(const Buffer* buffer, DataType index_type);
		void ActiveVertexFormat(const VertexFormat* format);
		void VertexAttribDivisor(uint attrib, uint divisor);
		void EnablePrimitiveRestart(bool enable);
		void PrimitiveRestartIndex(uint index);

		// conditional render
		void BeginConditionalRender(const Query* query, CondRenderMode mode);
		void EndConditionalRender();

		// transform feedback
		void BeginTransformFeedback(PrimitiveType primitive);
		void EndTransformFeedback();

		// viewport transform
		void Viewport(int x, int y, int width, int height);

		// back face culling
		void EnableFaceCulling(bool enable);
		void CullFace(PolygonFace face);
		void FrontFace(VertexOrient orient);

		// rasterization
		void RasterizationMode(RasterMode mode);
		void EnableRasterizerDiscard(bool enable);

		// multisampling
		void EnableMultisampling(bool enable);
		void EnableSampleAlphaToCoverage(bool enable);
		void EnableSampleAlphaToOne(bool enable);
		void EnableSampleCoverage(bool enable);
		void SampleCoverage(float value, bool invert);

		// scissor test
		void EnableScissorTest(bool enable);
		void Scissor(int x, int y, size_t width, size_t height);

		// depth test
		void EnableDepthTest(bool enable);
		void DepthTestFunc(CompareFunc func);
		void DepthRange(float dnear, float dfar);
		void DepthOffset(float factor, float units);

		// stencil test
		void EnableStencilTest(bool enable);
		void StencilFunc(PolygonFace face, CompareFunc func, int ref, uint mask);
		void StencilOperation(PolygonFace face, StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass);

		// blending
		void EnableBlending(bool enable);
		void EnableBlending(uint buffer, bool enable);
		void BlendingColor(float r, float g, float b, float a);
		void BlendingFunc(BlendFunc src_factor, BlendFunc dest_factor);
		void BlendingFunc(BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor);
		void BlendingOperation(BlendOp op);
		void BlendingOperation(BlendOp op_rgb, BlendOp op_alpha);

		// color clamping
		void ClampColor(ClampTarget target, ClampValue value);

		// logic operation
		void EnableLogicOperation(bool enable);
		void LogicOperation(LogicOp op);

		// framebuffer
		void DefaultFramebufferDrawBuffer(RenderbufferType buffer);
		void DefaultFramebufferReadBuffer(RenderbufferType buffer);
		void ActiveDrawFramebuffer(const Framebuffer* fbuf);
		void ActiveReadFramebuffer(const Framebuffer* fbuf);
		void EnableFramebufferSRGB(bool enable);
		void EnableColorWrite(bool r, bool g, bool b, bool a);
		void EnableColorWrite(uint buffer, bool r, bool g, bool b, bool a);
		void EnableDepthWrite(bool enable);
		void EnableStencilWrite(PolygonFace face, uint mask);
		void ClearColorBuffer(float r, float g, float b, float a);
		void ClearDepthStencilBuffer(uint buffers, float d, int s);
		void ReadPixels(int x, int y, size_t width, size_t height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* buffer);
		void ReadPixels(int x, int y, size_t width, size_t height, ImageFormat format, DataType type, const PixelStore* pixel_store, Buffer* buffer, size_t buffer_offset);
		void BlitFramebuffer(int src_x0, int src_y0, int src_x1, int src_y1, int dst_x0, int dst_y0, int dst_x1, int dst_y1, uint buffers, TexFilter filter);
		void SwapBuffers();
		void SwapInterval(int interval);

		// shaders
		void ActiveVertexASMProgram(const ASMProgram* vert_prog);
		void ActiveGeometryASMProgram(const ASMProgram* geom_prog);
		void ActiveFragmentASMProgram(const ASMProgram* frag_prog);
		void ActiveGLSLProgram(const GLSLProgram* program);
		void BindTransformFeedbackBuffer(uint index, Buffer* buffer);
		void BindTransformFeedbackBuffer(uint index, Buffer* buffer, size_t offset, size_t size);
		void BindUniformBuffer(uint index, Buffer* buffer);
		void BindUniformBuffer(uint index, Buffer* buffer, size_t offset, size_t size);

		// textures and samplers
		void SetSamplerTexture(int sampler, const Texture* texture);
		void SetSamplerState(int sampler, const SamplerState* state);

		// drawing commands
		void Draw(PrimitiveType prim, int first, size_t count);
		void DrawInstanced(PrimitiveType prim, int first, size_t count, size_t inst_count);
		void DrawIndexed(PrimitiveType prim, size_t index_start, size_t count);
		void DrawIndexed(PrimitiveType prim, size_t start, size_t end, size_t index_start, size_t count);
		void DrawIndexedInstanced(PrimitiveType prim, size_t count, size_t index_start, size_t inst_count);

		// synchronization
		void Flush();
		void Finish();

		// buffer copying
		void CopyBufferData(Buffer* source, Buffer* dest, size_t source_offset, size_t dest_offset, size_t size);

		// state queries
		void GetViewport(int& x, int& y, int& width, int& height);
		void GetViewport(float& x, float& y, float& width, float& height);

		// object creation
		VertexFormat* CreateVertexFormat(const VertexAttribDesc* descriptors, int count);
		void DestroyVertexFormat(VertexFormat* vert_fmt);
		SamplerState* CreateSamplerState(const SamplerStateDesc& descriptor);
		void DestroySamplerState(SamplerState* samp_state);
		Texture1D* CreateTexture1D();
		Texture1DArray* CreateTexture1DArray();
		Texture2D* CreateTexture2D();
		Texture2DArray* CreateTexture2DArray();
		Texture3D* CreateTexture3D();
		TextureCube* CreateTextureCube();
		TextureBuffer* CreateTextureBuffer();
		TextureRectangle* CreateTextureRectangle();
		void DestroyTexture(Texture* texture);
		Buffer* CreateBuffer(ObjectType type, size_t size, const void* data, BufferUsage usage);
		void DestroyBuffer(Buffer* buffer);
		Framebuffer* CreateFramebuffer(ObjectType type);
		void DestroyFramebuffer(Framebuffer* framebuffer);
		Renderbuffer* CreateRenderbuffer();
		void DestroyRenderbuffer(Renderbuffer* renderbuffer);
		Query* CreateQuery(ObjectType type);
		void DestroyQuery(Query* query);
		GLSLShader* CreateShader(ObjectType type, size_t count, const char** source, const char*& error_string);
		void DestroyShader(GLSLShader* shader);
		GLSLProgram* CreateProgram(size_t count, GLSLShader** shaders);
		void DestroyProgram(GLSLProgram* program);
		ASMProgram* CreateASMProgram(ObjectType type, const char* source, const char*& error_string);
		void DestroyASMProgram(ASMProgram* program);

	private:
		friend BASELIB_API Renderer* CreateRenderer(uint version, void* instance_handle, void* window_handle, const FramebufferFormat& format);
		friend BASELIB_API void DestroyRenderer(Renderer* renderer);

		Renderer();
		~Renderer() {}
		Renderer(const Renderer&);
		bool Create(uint version, void* instance_handle, void* window_handle, const FramebufferFormat& format);
		void Destroy();

		bool InitOpenGL(uint version, HWND window_handle, const FramebufferFormat& format);
		bool CreateContext(uint version, HWND window_handle, const FramebufferFormat& format);
		void DeinitOpenGL();
		void GetContextInfo();
		bool LoadOpenGLExtensions();
		void Clear();
		void SetupDrawingState();

		HGLRC _hglrc;
		HDC _hdc;
		HINSTANCE _instanceHandle;
		ContextInfo _info;
		bool _initialized;

		static const int MAX_VERTEX_STREAMS = 8;

		struct VertexStream
		{
			const Buffer* buffer;
			size_t stride;
			size_t offset;
		};

		struct VertexAttrib
		{
			const Buffer* buffer;
			size_t stride;
			size_t bufferBase;
			int numComponents;
			DataType type;
			bool integer;
			bool normalized;
			size_t offset;
			bool enabled;
			bool toEnable;
		};

		struct Sampler
		{
			const Texture* texture;
			const SamplerState* state;
		};

		// current state
		GLState _glState;
		VertexStream _vertexStreams[MAX_VERTEX_STREAMS];
		const VertexFormat* _vertexFormat;
		VertexAttrib* _vertexAttribs;
		const Buffer* _indexBuffer;
		DataType _indexType;
		const Framebuffer* _drawFramebuffer;
		const Framebuffer* _readFramebuffer;
		const ASMProgram* _vertexASMProg;
		const ASMProgram* _geometryASMProg;
		const ASMProgram* _fragmentASMProg;
		bool _activeVertASMProg;
		bool _activeGeomASMProg;
		bool _activeFragASMProg;
		const GLSLProgram* _glslProg;
		Sampler* _samplers;
		int _viewport[4];
	};

}


#endif // _RENDERING_CONTEXT_H_
