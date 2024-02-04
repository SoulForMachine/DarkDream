

#include <cassert>
#include "BaseLib/Memory.h"
#include "BaseLib/Console.h"
#include "GLRenderer.h"

using namespace Memory;


namespace GL
{

	static
	LRESULT CALLBACK TmpWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_CREATE:
			return 0;
		case WM_DESTROY:
			return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


	GLState* __gl_state;
#ifdef _DEBUG
	GLenum __last_gl_error;
#endif

	const PixelStore GLState::defaultPixelStore =
	{
		false,	// swap bytes
		false,	// LSB first
		0,		// row length
		0,		// skip rows
		0,		// skip pixels
		4,		// alignment
		0,		// image height
		0		// skip images
	};

	const SamplerStateDesc GLState::defaultSamplerState =
	{
		TEX_ADDRESS_REPEAT,			// address U
		TEX_ADDRESS_REPEAT,			// address V
		TEX_ADDRESS_REPEAT,			// address W
		TEX_FILTER_LINEAR,			// min filter
		TEX_FILTER_LINEAR,			// mag filter
		{ 0.0f, 0.0f, 0.0f, 0.0f },	// border color
		-1000.0f,					// min LOD
		1000.0f,					// max LOD
		0,							// base level
		1000,						// max level
		0.0f,						// LOD bias
		DEPTH_TEX_LUMINANCE,		// depth texture mode
		DEPTH_CMP_NONE,				// depth comparison mode
		FUNC_LEQUAL,				// depth comparison function
		1.0f						// max anisotropy
	};

	void __SetPixelPackState(const PixelStore* pixel_store)
	{
		const PixelStore* ps = pixel_store? pixel_store: &GLState::defaultPixelStore;
		PixelStore* cur_ps = &__gl_state->pixelStorePack;

		if(ps->swapBytes != cur_ps->swapBytes)
		{
			glPixelStorei(GL_PACK_SWAP_BYTES, (GLint)ps->swapBytes);
			OPENGL_ERROR_CHECK
			cur_ps->swapBytes = ps->swapBytes;
		}

		if(ps->LSBFirst != cur_ps->LSBFirst)
		{
			glPixelStorei(GL_PACK_LSB_FIRST, (GLint)ps->LSBFirst);
			OPENGL_ERROR_CHECK
			cur_ps->LSBFirst = ps->LSBFirst;
		}

		if(ps->rowLength != cur_ps->rowLength)
		{
			glPixelStorei(GL_PACK_ROW_LENGTH, ps->rowLength);
			OPENGL_ERROR_CHECK
			cur_ps->rowLength = ps->rowLength;
		}

		if(ps->skipRows != cur_ps->skipRows)
		{
			glPixelStorei(GL_PACK_SKIP_ROWS, ps->skipRows);
			OPENGL_ERROR_CHECK
			cur_ps->skipRows = ps->skipRows;
		}

		if(ps->skipPixels != cur_ps->skipPixels)
		{
			glPixelStorei(GL_PACK_SKIP_PIXELS, ps->skipPixels);
			OPENGL_ERROR_CHECK
			cur_ps->skipPixels = ps->skipPixels;
		}

		if(ps->alignment != cur_ps->alignment)
		{
			glPixelStorei(GL_PACK_ALIGNMENT, ps->alignment);
			OPENGL_ERROR_CHECK
			cur_ps->alignment = ps->alignment;
		}

		if(ps->imageHeight != cur_ps->imageHeight)
		{
			glPixelStorei(GL_PACK_IMAGE_HEIGHT, ps->imageHeight);
			OPENGL_ERROR_CHECK
			cur_ps->imageHeight = ps->imageHeight;
		}

		if(ps->skipImages != cur_ps->skipImages)
		{
			glPixelStorei(GL_PACK_SKIP_IMAGES, ps->skipImages);
			OPENGL_ERROR_CHECK
			cur_ps->skipImages = ps->skipImages;
		}
	}

	void __SetPixelUnpackState(const PixelStore* pixel_store)
	{
		const PixelStore* ps = pixel_store? pixel_store: &GLState::defaultPixelStore;
		PixelStore* cur_ps = &__gl_state->pixelStoreUnpack;

		if(ps->swapBytes != cur_ps->swapBytes)
		{
			glPixelStorei(GL_UNPACK_SWAP_BYTES, (GLint)ps->swapBytes);
			OPENGL_ERROR_CHECK
			cur_ps->swapBytes = ps->swapBytes;
		}

		if(ps->LSBFirst != cur_ps->LSBFirst)
		{
			glPixelStorei(GL_UNPACK_LSB_FIRST, (GLint)ps->LSBFirst);
			OPENGL_ERROR_CHECK
			cur_ps->LSBFirst = ps->LSBFirst;
		}

		if(ps->rowLength != cur_ps->rowLength)
		{
			glPixelStorei(GL_UNPACK_ROW_LENGTH, ps->rowLength);
			OPENGL_ERROR_CHECK
			cur_ps->rowLength = ps->rowLength;
		}

		if(ps->skipRows != cur_ps->skipRows)
		{
			glPixelStorei(GL_UNPACK_SKIP_ROWS, ps->skipRows);
			OPENGL_ERROR_CHECK
			cur_ps->skipRows = ps->skipRows;
		}

		if(ps->skipPixels != cur_ps->skipPixels)
		{
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, ps->skipPixels);
			OPENGL_ERROR_CHECK
			cur_ps->skipPixels = ps->skipPixels;
		}

		if(ps->alignment != cur_ps->alignment)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, ps->alignment);
			OPENGL_ERROR_CHECK
			cur_ps->alignment = ps->alignment;
		}

		if(ps->imageHeight != cur_ps->imageHeight)
		{
			glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, ps->imageHeight);
			OPENGL_ERROR_CHECK
			cur_ps->imageHeight = ps->imageHeight;
		}

		if(ps->skipImages != cur_ps->skipImages)
		{
			glPixelStorei(GL_UNPACK_SKIP_IMAGES, ps->skipImages);
			OPENGL_ERROR_CHECK
			cur_ps->skipImages = ps->skipImages;
		}
	}

	Renderer* CreateRenderer(uint version, void* instance_handle, void* window_handle, const FramebufferFormat& format)
	{
		Renderer* renderer = New<Renderer>(glPool);
		bool result = renderer->Create(version, instance_handle, window_handle, format);
		if(!result)
		{
			Delete(renderer);
			renderer = 0;
		}
		return renderer;
	}

	void DestroyRenderer(Renderer* renderer)
	{
		if(renderer)
		{
			renderer->Destroy();
			Delete(renderer);
		}
	}

	Renderer::Renderer()
	{
		Clear();
	}

	bool Renderer::Create(uint version, void* instance_handle, void* window_handle, const FramebufferFormat& format)
	{
		_instanceHandle = (HINSTANCE)instance_handle;
		return InitOpenGL(version, (HWND)window_handle, format);
	}

	void Renderer::Destroy()
	{
		DeinitOpenGL();
	}

	bool Renderer::SetCurrentContext()
	{
		BOOL result = wglMakeCurrent(_hdc, _hglrc);
		__gl_state = &_glState;
		return (result == TRUE);
	}

	bool Renderer::InitOpenGL(uint version, HWND window_handle, const FramebufferFormat& format)
	{
		if(_initialized)
			return false;

		BOOL result;
		tchar tmp_wnd_class[] = _t("DD_TempWndClass");

		// create temporary window
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.hInstance = _instanceHandle;
		wc.lpfnWndProc = TmpWindowProc;
		wc.lpszClassName = tmp_wnd_class;
		wc.style = CS_OWNDC;

		result = RegisterClass(&wc);
		if(!result)
		{
			Console::PrintError("Failed to register temporary window class.");
			return false;
		}

		HWND tmp_hwnd =	CreateWindow(	tmp_wnd_class, _t(""),
									WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW,
									CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									0, 0, _instanceHandle, 0	);

		if(!tmp_hwnd)
		{
			UnregisterClass(tmp_wnd_class, _instanceHandle);
			Console::PrintError("Failed to create temporary window.");
			return false;
		}

		// create temporary OpenGL context
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nVersion = 1;
		pfd.nSize = sizeof(pfd);
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 16;

		HDC tmp_dc = GetDC(tmp_hwnd);
		if(!tmp_hwnd)
		{
			DestroyWindow(tmp_hwnd);
			UnregisterClass(tmp_wnd_class, _instanceHandle);
			Console::PrintError("Failed to get temporary window DC.");
			return false;
		}

		int pixel_format = ChoosePixelFormat(tmp_dc, &pfd);
		if(!pixel_format)
		{
			DestroyWindow(tmp_hwnd);
			UnregisterClass(tmp_wnd_class, _instanceHandle);
			Console::PrintError("ChoosePixelFormat() failed for temporary window.");
			return false;
		}

		result = SetPixelFormat(tmp_dc, pixel_format, &pfd);
		if(!result)
		{
			DestroyWindow(tmp_hwnd);
			UnregisterClass(tmp_wnd_class, _instanceHandle);
			Console::PrintError("SetPixelFormat() failed for temporary window.");
			return false;
		}

		HGLRC tmp_rc = wglCreateContext(tmp_dc);
		if(!tmp_rc)
		{
			DestroyWindow(tmp_hwnd);
			UnregisterClass(tmp_wnd_class, _instanceHandle);
			Console::PrintError("wglCreateContext() failed for temporary window.");
			return false;
		}

		result = wglMakeCurrent(tmp_dc, tmp_rc);
		if(!result)
		{
			wglDeleteContext(tmp_rc);
			DestroyWindow(tmp_hwnd);
			UnregisterClass(tmp_wnd_class, _instanceHandle);
			Console::PrintError("Failed to make temporary window rendering context current.");
			return false;
		}

		result = CreateContext(version, window_handle, format);

		// destroy temporary rendering context and window
		wglMakeCurrent(0, 0);
		wglDeleteContext(tmp_rc);
		DestroyWindow(tmp_hwnd);
		UnregisterClass(tmp_wnd_class, _instanceHandle);

		if(!result)
		{
			return false;
		}

		result = wglMakeCurrent(_hdc, _hglrc);
		if(!result)
		{
			DeinitOpenGL();
			Console::PrintError("Failed to make rendering context current.");
			return false;
		}

		if(!LoadOpenGLExtensions())
		{
			DeinitOpenGL();
			Console::PrintError("Failed to load required extensions.");
			return false;
		}

		GetContextInfo();

		_samplers = NewArray<Sampler>(glPool, _info.maxTextureImageUnits);
		_vertexAttribs = NewArray<VertexAttrib>(glPool, _info.maxVertexAttribs);
		_glState.samplers = NewArray<GLuint>(glPool, _info.maxTextureImageUnits);
		memset(_samplers, 0, sizeof(Sampler) * _info.maxTextureImageUnits);
		memset(_vertexAttribs, 0, sizeof(VertexAttrib) * _info.maxVertexAttribs);
		memset(_glState.samplers, 0, sizeof(GLuint) * _info.maxTextureImageUnits);

		glGetIntegerv(GL_VIEWPORT, _viewport);

		_initialized = true;

		return true;
	}

	bool Renderer::CreateContext(int version, HWND window_handle, const FramebufferFormat& format)
	{
		BOOL result;
		HDC hdc = GetDC(window_handle);
		if(!hdc)
		{
			Console::PrintError("Failed to get window DC.");
			return false;
		}

		// create rendering context with WGL_ARB_pixel_format extension
		if(!glextIsSupported("WGL_ARB_pixel_format"))
		{
			Console::PrintError("WGL_ARB_pixel_format not supported.");
			return false;
		}

		if(!glextLoad_WGL_ARB_pixel_format())
		{
			Console::PrintError("The driver does not have all function entries for WGL_ARB_pixel_format.");
			return false;
		}

		// check if WGL_EXT_framebuffer_sRGB is supported
		bool sRGB = glextIsSupported("WGL_EXT_framebuffer_sRGB");

		UINT num_formats;
		int pixel_format;
		int int_atribs[] =
		{
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_DRAW_TO_WINDOW_ARB, TRUE,
			WGL_SUPPORT_OPENGL_ARB, TRUE,
			WGL_DOUBLE_BUFFER_ARB, format.doubleBuffer,
			WGL_SWAP_METHOD_ARB, format.swapMethod,
			WGL_PIXEL_TYPE_ARB, format.colorBufferType,
			WGL_COLOR_BITS_ARB, format.colorBits,
			WGL_DEPTH_BITS_ARB, format.depthBits,
			WGL_STENCIL_BITS_ARB, format.stencilBits,
			WGL_SAMPLE_BUFFERS_ARB, format.multisampleSamples? 1: 0,
			WGL_SAMPLES_ARB, format.multisampleSamples,
			(sRGB? WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT: 0), (sRGB? (format.sRGB? GL_TRUE: GL_FALSE): 0),
			0, 0
		};
		result = wglChoosePixelFormatARB(hdc, int_atribs, 0, 1, &pixel_format, &num_formats);
		if(!result || num_formats < 1)
		{
			Console::PrintError("wglChoosePixelFormatARB() failed.");
			return false;
		}

		PIXELFORMATDESCRIPTOR pfd;
		result = DescribePixelFormat(hdc, pixel_format, sizeof(pfd), &pfd);
		if(!result)
		{
			Console::PrintError("DescribePixelFormat() failed.");
			return false;
		}

		result = SetPixelFormat(hdc, pixel_format, &pfd);
		if(!result)
		{
			Console::PrintError("SetPixelFormat() failed.");
			return false;
		}

		HGLRC hglrc = 0;
		if(version >= 300)
		{
			if(	glextIsSupported("WGL_ARB_create_context") &&
				glextLoad_WGL_ARB_create_context() &&
				wglCreateContextAttribsARB )
			{
				int attribs[] =
				{
					WGL_CONTEXT_MAJOR_VERSION_ARB, version / 100,
					WGL_CONTEXT_MINOR_VERSION_ARB, version % 100 / 10,
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					0
				};
				hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
			}
		}
		else
		{
			hglrc = wglCreateContext(hdc);
		}

		if(hglrc)
		{
			_hglrc = hglrc;
			_hdc = hdc;
			_info.framebufferFormat = format;
			return true;
		}
		else
		{
			_hglrc = 0;
			_hdc = 0;
			Console::PrintError("Rendering context creation failed for version %u.", version);
			return false;
		}
	}

	void Renderer::DeinitOpenGL()
	{
		if(_hglrc)
		{
			if(wglGetCurrentContext() == _hglrc)
			{
				wglMakeCurrent(0, 0);
				__gl_state = 0;
			}
			wglDeleteContext(_hglrc);
			Memory::Delete(_samplers);
			Memory::Delete(_vertexAttribs);
			Memory::Delete(_glState.samplers);
			Clear();
		}
	}

	void Renderer::GetContextInfo()
	{
		_info.extensions = (const char*)glGetString(GL_EXTENSIONS);
		_info.wglExtensions = (const char*)wglGetExtensionsStringARB(_hdc);
		_info.renderer = (const char*)glGetString(GL_RENDERER);
		_info.vendor = (const char*)glGetString(GL_VENDOR);
		_info.glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
		const char* version = (const char*)glGetString(GL_VERSION);
		_info.versionMajor = atoi(version);
		_info.versionMinor = 0;
		const char* minor_ptr = strchr(version, '.');
		if(minor_ptr)
			_info.versionMinor = atoi(minor_ptr + 1);

		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &_info.maxRecomendedVertexCount);
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT, &_info.maxRecomendedIndexCount);

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_info.maxTextureSize);
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &_info.max3DTextureSize);
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &_info.maxCubeMapSize);
		glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &_info.maxRectangleTextureSize);
		glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS_EXT, &_info.maxTextureLODBias);
		int vport_dims[2];
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS, vport_dims);
		_info.maxViewportWidth = vport_dims[0];
		_info.maxViewportHeight = vport_dims[1];
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &_info.maxVertexAttribs);
		glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &_info.maxTextureCoords);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB, &_info.maxTextureImageUnits);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_info.maxTextureAnisotropy);
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &_info.maxDrawBuffers);
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_ARB, &_info.maxTextureBufferSize);
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &_info.maxElementVertices);
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &_info.maxElementIndices);
	//	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &_info.maxUniformBlockSize);
	//	glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &_info.maxCombinedUniformBlocks);

		OPENGL_ERROR_CHECK
	}


	#define LOAD_EXTENSION(ext, flag) \
			_info.flag = glextIsSupported(#ext); \
			if(_info.flag){ \
				_info.flag = glextLoad_##ext(); \
				if(!_info.flag) \
					Console::PrintWarning("The driver does not have all function entries for " ESC_ORANGE #ext ESC_DEFAULT " extension."); } \
			else { Console::PrintLn("Extension " ESC_ORANGE #ext ESC_DEFAULT " not supported."); }

	#define LOAD_EXTENSION_REQ(ext, flag) \
			_info.flag = glextIsSupported(#ext); \
			if(_info.flag){ \
				_info.flag = glextLoad_##ext(); \
				if(!_info.flag){ \
					Console::PrintWarning("The driver does not have all function entries for " ESC_ORANGE #ext ESC_DEFAULT " extension."); \
					result = false; } } \
			else{ \
				Console::PrintLn("Extension " ESC_ORANGE #ext ESC_DEFAULT " not supported."); \
				result = false; }

	#define CHECK_EXTENSION(ext, flag) \
			_info.flag = glextIsSupported(#ext); \
			if(!_info.flag) \
				Console::PrintLn("Extension " ESC_ORANGE #ext ESC_DEFAULT " not supported.");

	#define CHECK_EXTENSION_REQ(ext, flag) \
			_info.flag = glextIsSupported(#ext); \
			if(!_info.flag){ \
				Console::PrintLn("Extension " ESC_ORANGE #ext ESC_DEFAULT " not supported."); \
				result = false; }

	bool Renderer::LoadOpenGLExtensions()
	{
		bool result = true;

		// v1.2
		LOAD_EXTENSION_REQ(GL_EXT_draw_range_elements, extDrawRangeElements)
		LOAD_EXTENSION_REQ(GL_EXT_texture3D, extTexture3D)
		LOAD_EXTENSION_REQ(GL_EXT_blend_minmax, extBlendMinmax)
		CHECK_EXTENSION_REQ(GL_EXT_blend_subtract, extBlendSubtract)

		// v1.3
		LOAD_EXTENSION_REQ(GL_ARB_multisample, extMultisample)
		LOAD_EXTENSION_REQ(GL_ARB_multitexture, extMultitexture)
		CHECK_EXTENSION_REQ(GL_ARB_texture_border_clamp, extTextureBorderClamp)
		LOAD_EXTENSION_REQ(GL_ARB_texture_compression, extTextureCompression)
		CHECK_EXTENSION_REQ(GL_ARB_texture_cube_map, extTextureCubeMap)
		CHECK_EXTENSION_REQ(GL_EXT_texture_compression_s3tc, extTextureCompressionS3TC)

		// v1.4
		CHECK_EXTENSION_REQ(GL_ARB_depth_texture, extDepthTexture)
		CHECK_EXTENSION_REQ(GL_ARB_shadow, extShadow)
		LOAD_EXTENSION_REQ(GL_EXT_multi_draw_arrays, extMultiDrawArrays)
		CHECK_EXTENSION_REQ(GL_EXT_texture_lod_bias, extTextureLodBias)
		CHECK_EXTENSION_REQ(GL_SGIS_generate_mipmap, extGenerateMipmap)
		LOAD_EXTENSION_REQ(GL_EXT_blend_func_separate, extBlendFuncSeparate)
		LOAD_EXTENSION_REQ(GL_EXT_blend_color, extBlendColor)

		// v1.5
		LOAD_EXTENSION_REQ(GL_ARB_occlusion_query, extOcclusionQuery)
		LOAD_EXTENSION_REQ(GL_ARB_vertex_buffer_object, extVertexBufferObject)
		CHECK_EXTENSION_REQ(GL_EXT_shadow_funcs, extShadowFuncs)

		// v2.0
		LOAD_EXTENSION_REQ(GL_ARB_draw_buffers, extDrawBuffers)
		CHECK_EXTENSION_REQ(GL_ARB_texture_non_power_of_two, extTextureNonPowerOfTwo)
		LOAD_EXTENSION_REQ(GL_EXT_blend_equation_separate, extBlendEquationSeparate)

		// v2.1
		CHECK_EXTENSION_REQ(GL_ARB_pixel_buffer_object, extPixelBufferObject)

		// v3.0
		CHECK_EXTENSION_REQ(GL_ARB_texture_float, extTextureFloat)
		LOAD_EXTENSION_REQ(GL_EXT_framebuffer_object, extFramebufferObject)
		LOAD_EXTENSION(GL_ARB_color_buffer_float, extColorBufferFloat)
		LOAD_EXTENSION(GL_EXT_gpu_shader4, extGPUShader4)
		LOAD_EXTENSION(GL_EXT_transform_feedback, extTransformFeedback)
		LOAD_EXTENSION(GL_EXT_draw_buffers2, extDrawBuffers2)
		CHECK_EXTENSION(GL_EXT_framebuffer_sRGB, extFramebufferSRGB)
		LOAD_EXTENSION_REQ(GL_EXT_framebuffer_multisample, extFramebufferMultisample)
		LOAD_EXTENSION(GL_EXT_framebuffer_blit, extFramebufferBlit)
		LOAD_EXTENSION(GL_NV_conditional_render, extConditionalRender)
		LOAD_EXTENSION(GL_ARB_map_buffer_range, extMapBufferRange)
		CHECK_EXTENSION(GL_ARB_texture_compression_rgtc, extTextureCompressionRGTC)

		// v3.1
		LOAD_EXTENSION(GL_ARB_copy_buffer, extCopyBuffer)
		LOAD_EXTENSION(GL_ARB_uniform_buffer_object, extUniformBufferObject)
		LOAD_EXTENSION(GL_ARB_texture_buffer_object, extTextureBufferObject)
		LOAD_EXTENSION(GL_ARB_draw_instanced, extDrawInstanced)
		CHECK_EXTENSION_REQ(GL_ARB_texture_rectangle, extTextureRectangle)
		LOAD_EXTENSION(GL_NV_primitive_restart, extPrimitiveRestart)

		// ARB programs
		LOAD_EXTENSION_REQ(GL_ARB_vertex_program, extVertexProgram)
		CHECK_EXTENSION(GL_NV_vertex_program1_1, extVertexProgram1_1)
		CHECK_EXTENSION(GL_NV_vertex_program2, extVertexProgram2)
		CHECK_EXTENSION(GL_NV_vertex_program2_option, extVertexProgram2Option)
		CHECK_EXTENSION(GL_NV_vertex_program3, extVertexProgram3)
	//	LOAD_EXTENSION(GL_NV_vertex_program4, extVertexProgram4)
		CHECK_EXTENSION_REQ(GL_ARB_fragment_program, extFragmentProgram)
		CHECK_EXTENSION(GL_NV_fragment_program_option, extFragmentProgramOption)
		CHECK_EXTENSION(GL_NV_fragment_program2, extFragmentProgram2)
	//	CHECK_EXTENSION(GL_NV_fragment_program4, extFragmentProgram4)
		CHECK_EXTENSION_REQ(GL_ARB_fragment_program_shadow, extFragmentProgramShadow)
		LOAD_EXTENSION(GL_NV_gpu_program4, extGPUProgram4)
	//	LOAD_EXTENSION(GL_NV_geometry_program4, extGeometryProgram4)
		LOAD_EXTENSION_REQ(GL_EXT_gpu_program_parameters, extGPUProgramParameters)

		// other
		CHECK_EXTENSION_REQ(GL_EXT_texture_filter_anisotropic, extTextureFilterAnisotropic)
		LOAD_EXTENSION(GL_ARB_geometry_shader4, extGeometryShader4)

		// wgl
		LOAD_EXTENSION_REQ(WGL_ARB_extensions_string, wextExtensionsString)
		LOAD_EXTENSION_REQ(WGL_ARB_pixel_format, wextPixelFormat)
		CHECK_EXTENSION(WGL_ARB_pixel_format_float, wextPixelFormatFloat)
		CHECK_EXTENSION_REQ(WGL_ARB_multisample, wextMultisample)
		LOAD_EXTENSION_REQ(WGL_EXT_swap_control, wextSwapControl)
		CHECK_EXTENSION(WGL_EXT_framebuffer_sRGB, wextFramebufferSRGB)
		LOAD_EXTENSION(WGL_ARB_create_context, wextCreateContext);

		_info.extVersion_2_0 = glextLoad_GL_VERSION_2_0();
		if(!_info.extVersion_2_0)
		{
			Console::PrintWarning("The driver does not have all function entries for " ESC_ORANGE "GL_VERSION_2_0.");
			result = false;
		}
		_info.extVersion_2_1 = glextLoad_GL_VERSION_2_1();
		if(!_info.extVersion_2_1)
		{
			Console::PrintWarning("The driver does not have all function entries for " ESC_ORANGE "GL_VERSION_2_1.");
			result = false;
		}

		return result;
	}

	void Renderer::Clear()
	{
		_hglrc = 0;
		_hdc = 0;
		_instanceHandle = 0;
		memset(&_info, 0, sizeof(_info));
		_initialized = false;

		memset(&_glState, 0, sizeof(_glState));
		_glState.pixelStorePack = GLState::defaultPixelStore;
		_glState.pixelStoreUnpack = GLState::defaultPixelStore;
		memset(&_vertexStreams, 0, sizeof(_vertexStreams));
		_vertexFormat = 0;
		_vertexAttribs = 0;
		_indexBuffer = 0;
		_indexType = TYPE_VOID;
		_drawFramebuffer = 0;
		_readFramebuffer = 0;
		_vertexASMProg = 0;
		_geometryASMProg = 0;
		_fragmentASMProg = 0;
		_activeVertASMProg = false;
		_activeGeomASMProg = false;
		_activeFragASMProg = false;
		_glslProg = 0;
		_samplers = 0;
	}

	void Renderer::VertexSource(int stream, const Buffer* buffer, int stride, int offset)
	{
		assert(stream >= 0 && stream < MAX_VERTEX_STREAMS);
		_vertexStreams[stream].buffer = buffer;
		_vertexStreams[stream].stride = stride;
		_vertexStreams[stream].offset = offset;
	}

	void Renderer::IndexSource(const Buffer* buffer, DataType index_type)
	{
		_indexBuffer = buffer;
		_indexType = index_type;
	}

	void Renderer::ActiveVertexFormat(const VertexFormat* format)
	{
		_vertexFormat = format;
	}

	void Renderer::EnablePrimitiveRestart(bool enable)
	{
		if(enable)
			glEnable(GL_PRIMITIVE_RESTART_NV);
		else
			glDisable(GL_PRIMITIVE_RESTART_NV);
		OPENGL_ERROR_CHECK
	}

	void Renderer::PrimitiveRestartIndex(uint index)
	{
		glPrimitiveRestartIndexNV(index);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BeginConditionalRender(const Query* query, CondRenderMode mode)
	{
		glBeginConditionalRenderNV(query->_id, mode);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EndConditionalRender()
	{
		glEndConditionalRenderNV();
		OPENGL_ERROR_CHECK
	}

	/*
		primitive must be one of PRIM_POINTS, PRIM_LINES or PRIM_TRIANGLES
	*/
	void Renderer::BeginTransformFeedback(PrimitiveType primitive)
	{
		glBeginTransformFeedbackEXT(primitive);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EndTransformFeedback()
	{
		glEndTransformFeedbackEXT();
		OPENGL_ERROR_CHECK
	}

	void Renderer::Viewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
		OPENGL_ERROR_CHECK
		_viewport[0] = x;
		_viewport[1] = y;
		_viewport[2] = width;
		_viewport[3] = height;
	}

	void Renderer::EnableFaceCulling(bool enable)
	{
		if(enable)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		OPENGL_ERROR_CHECK
	}

	void Renderer::CullFace(PolygonFace face)
	{
		glCullFace(face);
		OPENGL_ERROR_CHECK
	}

	void Renderer::FrontFace(VertexOrient orient)
	{
		glFrontFace(orient);
		OPENGL_ERROR_CHECK
	}

	void Renderer::RasterizationMode(RasterMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableRasterizerDiscard(bool enable)
	{
		if(enable)
			glEnable(GL_RASTERIZER_DISCARD_EXT);
		else
			glDisable(GL_RASTERIZER_DISCARD_EXT);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableMultisampling(bool enable)
	{
		if(enable)
			glEnable(GL_MULTISAMPLE_ARB);
		else
			glDisable(GL_MULTISAMPLE_ARB);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableSampleAlphaToCoverage(bool enable)
	{
		if(enable)
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
		else
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableSampleAlphaToOne(bool enable)
	{
		if(enable)
			glEnable(GL_SAMPLE_ALPHA_TO_ONE_ARB);
		else
			glDisable(GL_SAMPLE_ALPHA_TO_ONE_ARB);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableSampleCoverage(bool enable)
	{
		if(enable)
			glEnable(GL_SAMPLE_COVERAGE_ARB);
		else
			glDisable(GL_SAMPLE_COVERAGE_ARB);
		OPENGL_ERROR_CHECK
	}

	void Renderer::SampleCoverage(float value, bool invert)
	{
		glSampleCoverageARB(value, invert);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableScissorTest(bool enable)
	{
		if(enable)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		OPENGL_ERROR_CHECK
	}

	void Renderer::Scissor(int x, int y, int width, int height)
	{
		glScissor(x, y, width, height);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableDepthTest(bool enable)
	{
		if(enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DepthTestFunc(CompareFunc func)
	{
		glDepthFunc(func);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DepthRange(float dnear, float dfar)
	{
		glDepthRange(dnear, dfar);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DepthOffset(float factor, float units)
	{
		glPolygonOffset(factor, units);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableStencilTest(bool enable)
	{
		if(enable)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		OPENGL_ERROR_CHECK
	}

	void Renderer::StencilFunc(PolygonFace face, CompareFunc func, int ref, uint mask)
	{
		glStencilFuncSeparate(face, func, ref, mask);
		OPENGL_ERROR_CHECK
	}

	void Renderer::StencilOperation(PolygonFace face, StencilOp stencil_fail, StencilOp depth_fail, StencilOp depth_pass)
	{
		glStencilOpSeparate(face, stencil_fail, depth_fail, depth_pass);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableBlending(bool enable)
	{
		if(enable)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableBlending(uint buffer, bool enable)
	{
		if(enable)
			glEnableIndexedEXT(GL_BLEND, buffer);
		else
			glDisableIndexedEXT(GL_BLEND, buffer);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BlendingColor(float r, float g, float b, float a)
	{
		glBlendColorEXT(r, g, b, a);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BlendingFunc(BlendFunc src_factor, BlendFunc dest_factor)
	{
		glBlendFunc(src_factor, dest_factor);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BlendingFunc(BlendFunc src_rgb_factor, BlendFunc dest_rgb_factor, BlendFunc src_alpha_factor, BlendFunc dest_alpha_factor)
	{
		glBlendFuncSeparateEXT(src_rgb_factor, dest_rgb_factor, src_alpha_factor, dest_alpha_factor);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BlendingOperation(BlendOp op)
	{
		glBlendEquationEXT(op);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BlendingOperation(BlendOp op_rgb, BlendOp op_alpha)
	{
		glBlendEquationSeparateEXT(op_rgb, op_alpha);
		OPENGL_ERROR_CHECK
	}

	void Renderer::ClampColor(ClampTarget target, ClampValue value)
	{
		glClampColorARB(target, value);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableLogicOperation(bool enable)
	{
		if(enable)
			glEnable(GL_COLOR_LOGIC_OP);
		else
			glDisable(GL_COLOR_LOGIC_OP);
		OPENGL_ERROR_CHECK
	}

	void Renderer::LogicOperation(LogicOp op)
	{
		glLogicOp(op);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DefaultFramebufferDrawBuffer(RenderbufferType buffer)
	{
		if(_glState.drawFbuf != 0)
		{
			glBindFramebufferEXT(OBJ_DRAW_FRAMEBUFFER, 0);
			OPENGL_ERROR_CHECK
			_glState.drawFbuf = 0;
		}
		glDrawBuffer(buffer);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DefaultFramebufferReadBuffer(RenderbufferType buffer)
	{
		if(_glState.readFbuf != 0)
		{
			glBindFramebufferEXT(OBJ_READ_FRAMEBUFFER, 0);
			OPENGL_ERROR_CHECK
			_glState.readFbuf = 0;
		}
		glReadBuffer(buffer);
		OPENGL_ERROR_CHECK
	}

	/*
		if fbuf is 0, default framebuffer is used
	*/
	void Renderer::ActiveDrawFramebuffer(const Framebuffer* fbuf)
	{
		_drawFramebuffer = fbuf;
	}

	/*
		if fbuf is 0, default framebuffer is used
	*/
	void Renderer::ActiveReadFramebuffer(const Framebuffer* fbuf)
	{
		_readFramebuffer = fbuf;
	}

	void Renderer::EnableFramebufferSRGB(bool enable)
	{
		if(enable)
			glEnable(GL_FRAMEBUFFER_SRGB_EXT);
		else
			glDisable(GL_FRAMEBUFFER_SRGB_EXT);
		OPENGL_ERROR_CHECK
	}

	/*
		sets color writing for all draw buffers
	*/
	void Renderer::EnableColorWrite(bool r, bool g, bool b, bool a)
	{
		glColorMask(r, g, b, a);
		OPENGL_ERROR_CHECK
	}

	/*
		sets color writing for specified draw buffer
	*/
	void Renderer::EnableColorWrite(uint buffer, bool r, bool g, bool b, bool a)
	{
		glColorMaskIndexedEXT(buffer, r, g, b, a);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableDepthWrite(bool enable)
	{
		glDepthMask(enable);
		OPENGL_ERROR_CHECK
	}

	void Renderer::EnableStencilWrite(PolygonFace face, uint mask)
	{
		glStencilMaskSeparate(face, mask);
		OPENGL_ERROR_CHECK
	}

	void Renderer::ClearColorBuffer(float r, float g, float b, float a)
	{
		GLuint id = _drawFramebuffer? _drawFramebuffer->_id: 0;
		if(id != _glState.drawFbuf)
		{
			glBindFramebufferEXT(OBJ_DRAW_FRAMEBUFFER, id);
			OPENGL_ERROR_CHECK
			_glState.drawFbuf = id;
		}

		glClearColor(r, g, b, a);
		OPENGL_ERROR_CHECK
		glClear(COLOR_BUFFER_BIT);
		OPENGL_ERROR_CHECK
	}

	/*
		Always uses front stencil mask. Only DEPTH_BUFFER_BIT and STENCIL_BUFFER_BIT are allowed.
	*/
	void Renderer::ClearDepthStencilBuffer(uint buffers, float d, int s)
	{
		assert((buffers & ~(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT)) == 0);

		GLuint id = _drawFramebuffer? _drawFramebuffer->_id: 0;
		if(id != _glState.drawFbuf)
		{
			glBindFramebufferEXT(OBJ_DRAW_FRAMEBUFFER, id);
			OPENGL_ERROR_CHECK
			_glState.drawFbuf = id;
		}

		if(buffers & DEPTH_BUFFER_BIT)
		{
			glClearDepth(d);
			OPENGL_ERROR_CHECK
		}

		if(buffers & STENCIL_BUFFER_BIT)
		{
			glClearStencil(s);
			OPENGL_ERROR_CHECK
		}

		glClear(buffers);
		OPENGL_ERROR_CHECK
	}

	/*
		Reads current read framebuffer's pixels into provided system memory buffer.
		If pixel_store is 0, default values are used.
	*/
	void Renderer::ReadPixels(int x, int y, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* buffer)
	{
		GLuint id = _readFramebuffer? _readFramebuffer->_id: 0;
		if(id != _glState.readFbuf)
		{
			glBindFramebufferEXT(OBJ_READ_FRAMEBUFFER, id);
			OPENGL_ERROR_CHECK
			_glState.readFbuf = id;
		}

		// apply pixel store states
		__SetPixelPackState(pixel_store);

		// read framebuffer pixels into buffer
		glReadPixels(x, y, width, height, format, type, buffer);
		OPENGL_ERROR_CHECK
	}

	/*
		Reads current read framebuffer's pixels into provided buffer object.
		If pixel_store is 0, default values are used.
	*/
	void Renderer::ReadPixels(int x, int y, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, Buffer* buffer, size_t buffer_offset)
	{
		assert(buffer);
		if(buffer->_id != _glState.pixelPackBuf)
		{
			glBindBufferARB(OBJ_PIXEL_PACK_BUFFER, buffer->_id);
			OPENGL_ERROR_CHECK
			_glState.pixelPackBuf = buffer->_id;
		}

		ReadPixels(x, y, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	void Renderer::BlitFramebuffer(int src_x0, int src_y0, int src_x1, int src_y1, int dst_x0, int dst_y0, int dst_x1, int dst_y1, uint buffers, TexFilter filter)
	{
		GLuint id = _readFramebuffer? _readFramebuffer->_id: 0;
		if(id != _glState.readFbuf)
		{
			glBindFramebufferEXT(OBJ_READ_FRAMEBUFFER, id);
			OPENGL_ERROR_CHECK
			_glState.readFbuf = id;
		}

		id = _drawFramebuffer? _drawFramebuffer->_id: 0;
		if(id != _glState.drawFbuf)
		{
			glBindFramebufferEXT(OBJ_DRAW_FRAMEBUFFER, id);
			OPENGL_ERROR_CHECK
			_glState.drawFbuf = id;
		}

		glBlitFramebufferEXT(src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, buffers, filter);
		OPENGL_ERROR_CHECK
	}

	void Renderer::SwapBuffers()
	{
		assert(_hdc && _hglrc);
		::SwapBuffers(_hdc);
	}

	void Renderer::SwapInterval(int interval)
	{
		wglSwapIntervalEXT(interval);
		OPENGL_ERROR_CHECK
	}

	void Renderer::ActiveVertexASMProgram(const ASMProgram* vert_prog)
	{
		_vertexASMProg = vert_prog;
		if(vert_prog)
		{
			// disable GLSL
			_glslProg = 0;
			if(_glState.glslProg)
			{
				glUseProgram(0);
				OPENGL_ERROR_CHECK
				_glState.glslProg = 0;
			}

			// enable ARB vertex program
			_activeVertASMProg = true;
			glEnable(GL_VERTEX_PROGRAM_ARB);
			OPENGL_ERROR_CHECK
		}
		else
		{
			// disable ARB vertex program
			_activeVertASMProg = false;
			glDisable(GL_VERTEX_PROGRAM_ARB);
			OPENGL_ERROR_CHECK
		}
	}

	void Renderer::ActiveGeometryASMProgram(const ASMProgram* geom_prog)
	{
		_geometryASMProg = geom_prog;
		if(geom_prog)
		{
			// disable GLSL
			_glslProg = 0;
			if(_glState.glslProg)
			{
				glUseProgram(0);
				OPENGL_ERROR_CHECK
				_glState.glslProg = 0;
			}

			// enable ARB geometry program
			_activeGeomASMProg = true;
			glEnable(GL_GEOMETRY_PROGRAM_NV);
			OPENGL_ERROR_CHECK
		}
		else
		{
			// disable ARB geometry program
			_activeGeomASMProg = false;
			glDisable(GL_GEOMETRY_PROGRAM_NV);
			OPENGL_ERROR_CHECK
		}
	}

	void Renderer::ActiveFragmentASMProgram(const ASMProgram* frag_prog)
	{
		_fragmentASMProg = frag_prog;
		if(frag_prog)
		{
			// disable GLSL
			_glslProg = 0;
			if(_glState.glslProg)
			{
				glUseProgram(0);
				OPENGL_ERROR_CHECK
				_glState.glslProg = 0;
			}

			// enable ARB fragment program
			_activeFragASMProg = true;
			glEnable(GL_FRAGMENT_PROGRAM_ARB);
			OPENGL_ERROR_CHECK
		}
		else
		{
			// disable ARB fragment program
			_activeFragASMProg = false;
			glDisable(GL_FRAGMENT_PROGRAM_ARB);
			OPENGL_ERROR_CHECK
		}
	}

	void Renderer::ActiveGLSLProgram(const GLSLProgram* program)
	{
		// disable ARB programs
		if(_activeVertASMProg)
		{
			glDisable(GL_VERTEX_PROGRAM_ARB);
			OPENGL_ERROR_CHECK
			_vertexASMProg = 0;
			_activeVertASMProg = false;
		}

		if(_activeGeomASMProg)
		{
			glDisable(GL_GEOMETRY_PROGRAM_NV);
			OPENGL_ERROR_CHECK
			_geometryASMProg = 0;
			_activeGeomASMProg = false;
		}

		if(_activeFragASMProg)
		{
			glDisable(GL_FRAGMENT_PROGRAM_ARB);
			OPENGL_ERROR_CHECK
			_fragmentASMProg = 0;
			_activeFragASMProg = false;
		}

		// enable GLSL program
		_glslProg = program;
	}

	void Renderer::BindTransformFeedbackBuffer(uint index, Buffer* buffer)
	{
		glBindBufferBaseEXT(OBJ_TRANSFORM_FEEDBACK_BUFFER, index, buffer->_id);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BindTransformFeedbackBuffer(uint index, Buffer* buffer, size_t offset, size_t size)
	{
		glBindBufferRangeEXT(OBJ_TRANSFORM_FEEDBACK_BUFFER, index, buffer->_id, offset, size);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BindUniformBuffer(uint index, Buffer* buffer)
	{
		glBindBufferBaseEXT(OBJ_UNIFORM_BUFFER, index, buffer->_id);
		OPENGL_ERROR_CHECK
	}

	void Renderer::BindUniformBuffer(uint index, Buffer* buffer, size_t offset, size_t size)
	{
		glBindBufferRangeEXT(OBJ_UNIFORM_BUFFER, index, buffer->_id, offset, size);
		OPENGL_ERROR_CHECK
	}

	void Renderer::SetSamplerTexture(int sampler, const Texture* texture)
	{
		assert(sampler >= 0 && sampler < _info.maxTextureImageUnits);
		_samplers[sampler].texture = texture;
	}

	void Renderer::SetSamplerState(int sampler, const SamplerState* state)
	{
		assert(sampler >= 0 && sampler < _info.maxTextureImageUnits);
		_samplers[sampler].state = state;
	}

	void Renderer::Draw(PrimitiveType prim, int first, int count)
	{
		if(!_vertexFormat)
			return;

		SetupDrawingState();
		glDrawArrays(prim, first, count);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DrawInstanced(PrimitiveType prim, int first, int count, int inst_count)
	{
		if(!_vertexFormat)
			return;

		SetupDrawingState();
		glDrawArraysInstancedARB(prim, first, count, inst_count);
		OPENGL_ERROR_CHECK
	}

	void Renderer::DrawIndexed(PrimitiveType prim, int index_start, int count)
	{
		if(!_vertexFormat || !_indexBuffer)
			return;

		SetupDrawingState();

		if(_indexBuffer->_id != _glState.indexBuf)
		{
			glBindBufferARB(OBJ_INDEX_BUFFER, _indexBuffer->_id);
			OPENGL_ERROR_CHECK
			_glState.indexBuf = _indexBuffer->_id;
		}

		glDrawElements(prim, count, _indexType, BUFFER_OFFSET(index_start));
		OPENGL_ERROR_CHECK
	}

	/*
		Draws a range of elements - it has constraint that all the indices must lie between start and end inclusive
	*/
	void Renderer::DrawIndexed(PrimitiveType prim, uint start, uint end, size_t index_start, int count)
	{
		if(!_vertexFormat || !_indexBuffer)
			return;

		SetupDrawingState();

		if(_indexBuffer->_id != _glState.indexBuf)
		{
			glBindBufferARB(OBJ_INDEX_BUFFER, _indexBuffer->_id);
			OPENGL_ERROR_CHECK
			_glState.indexBuf = _indexBuffer->_id;
		}

		glDrawRangeElementsEXT(prim, start, end, count, _indexType, BUFFER_OFFSET(index_start));
		OPENGL_ERROR_CHECK
	}

	void Renderer::DrawIndexedInstanced(PrimitiveType prim, int count, size_t index_start, int inst_count)
	{
		if(!_vertexFormat || !_indexBuffer)
			return;

		SetupDrawingState();

		if(_indexBuffer->_id != _glState.indexBuf)
		{
			glBindBufferARB(OBJ_INDEX_BUFFER, _indexBuffer->_id);
			OPENGL_ERROR_CHECK
			_glState.indexBuf = _indexBuffer->_id;
		}

		glDrawElementsInstancedARB(prim, count, _indexType, BUFFER_OFFSET(index_start), inst_count);
		OPENGL_ERROR_CHECK
	}

	void Renderer::Flush()
	{
		glFlush();
		OPENGL_ERROR_CHECK
	}

	void Renderer::Finish()
	{
		glFinish();
		OPENGL_ERROR_CHECK
	}

	void Renderer::CopyBufferData(Buffer* source, Buffer* dest, size_t source_offset, size_t dest_offset, size_t size)
	{
		glBindBufferARB(GL_COPY_READ_BUFFER, source->_id);
		OPENGL_ERROR_CHECK
		glBindBufferARB(GL_COPY_WRITE_BUFFER, dest->_id);
		OPENGL_ERROR_CHECK
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, source_offset, dest_offset, size);
		OPENGL_ERROR_CHECK
	}

	void Renderer::SetupDrawingState()
	{
		// set current draw framebuffer
		GLuint fbuf_id = _drawFramebuffer? _drawFramebuffer->_id: 0;
		if(fbuf_id != _glState.drawFbuf)
		{
			glBindFramebufferEXT(OBJ_DRAW_FRAMEBUFFER, fbuf_id);
			OPENGL_ERROR_CHECK
			_glState.drawFbuf = fbuf_id;
		}

		// setup vertex attribute pointers
		for(int i = 0; i < _vertexFormat->_count; ++i)
		{
			VertexAttribDesc& desc = _vertexFormat->_descriptors[i];

			assert(desc.stream >= 0 && desc.stream < MAX_VERTEX_STREAMS);
			assert(desc.attribute >= 0 && desc.attribute < (uint)_info.maxVertexAttribs);

			VertexStream& vstream = _vertexStreams[desc.stream];
			VertexAttrib& vattrib = _vertexAttribs[desc.attribute];

			if(vstream.buffer)
			{
				if(	vstream.buffer != vattrib.buffer ||
					vstream.stride != vattrib.stride ||
					vstream.offset != vattrib.bufferBase ||
					desc.numComponents != vattrib.numComponents ||
					desc.type != vattrib.type ||
					desc.integer != vattrib.integer ||
					desc.normalized != vattrib.normalized ||
					desc.offset != vattrib.offset )
				{
					if(vstream.buffer->_id != _glState.vertexBuf)
					{
						glBindBufferARB(OBJ_VERTEX_BUFFER, vstream.buffer->_id);
						OPENGL_ERROR_CHECK
						_glState.vertexBuf = vstream.buffer->_id;
					}

					if(desc.integer)
						glVertexAttribIPointerEXT(desc.attribute, desc.numComponents, desc.type, vstream.stride, BUFFER_OFFSET(vstream.offset + desc.offset));
					else
						glVertexAttribPointerARB(desc.attribute, desc.numComponents, desc.type, desc.normalized, vstream.stride, BUFFER_OFFSET(vstream.offset + desc.offset));
					OPENGL_ERROR_CHECK

					vattrib.buffer = vstream.buffer;
					vattrib.stride = vstream.stride;
					vattrib.bufferBase = vstream.offset;
					vattrib.numComponents = desc.numComponents;
					vattrib.type = desc.type;
					vattrib.integer = desc.integer;
					vattrib.normalized = desc.normalized;
					vattrib.offset = desc.offset;
				}

				vattrib.toEnable = true;
			}
		}

		// enable/disable attribute arrays
		for(int i = 0; i < _info.maxVertexAttribs; ++i)
		{
			VertexAttrib& vattrib = _vertexAttribs[i];
			if(vattrib.toEnable && !vattrib.enabled)
			{
				vattrib.enabled = true;
				glEnableVertexAttribArrayARB(i);
				OPENGL_ERROR_CHECK
			}
			else if(!vattrib.toEnable && vattrib.enabled)
			{
				vattrib.enabled = false;
				glDisableVertexAttribArrayARB(i);
				OPENGL_ERROR_CHECK
			}
			vattrib.toEnable = false;
		}

		// set textures
		for(int i = 0; i < _info.maxTextureImageUnits; ++i)
		{
			Sampler& sampler = _samplers[i];

			if(sampler.texture)
			{
				if(i == 0)
				{
					GLuint* cur_id = 0;
					switch(sampler.texture->_target)
					{
					case OBJ_TEXTURE_1D:
						cur_id = &_glState.tex1D;
						break;
					case OBJ_TEXTURE_2D:
						cur_id = &_glState.tex2D;
						break;
					case OBJ_TEXTURE_3D:
						cur_id = &_glState.tex3D;
						break;
					case OBJ_TEXTURE_CUBE:
						cur_id = &_glState.texCube;
						break;
					case OBJ_TEXTURE_1D_ARRAY:
						cur_id = &_glState.tex1DArray;
						break;
					case OBJ_TEXTURE_2D_ARRAY:
						cur_id = &_glState.tex2DArray;
						break;
					case OBJ_TEXTURE_BUFFER:
						cur_id = &_glState.texBuffer;
						break;
					case OBJ_TEXTURE_RECTANGLE:
						cur_id = &_glState.texRect;
						break;
					}

					assert(cur_id);
					*cur_id = sampler.texture->_id;
				}

				bool set_tex = (sampler.texture->_id != _glState.samplers[i]);
				if(set_tex)
				{
					glActiveTextureARB(GL_TEXTURE0_ARB + i);
					OPENGL_ERROR_CHECK
					glBindTexture(sampler.texture->_target, sampler.texture->_id);
					OPENGL_ERROR_CHECK
					_glState.samplers[i] = sampler.texture->_id;
				}

				// set texture parameters
				if( sampler.state != sampler.texture->_curSamplerState &&
					sampler.texture->_target != OBJ_TEXTURE_BUFFER )	// texture buffers don't have parameters
				{
					sampler.texture->_curSamplerState = sampler.state;

					if(!set_tex)
					{
						glActiveTextureARB(GL_TEXTURE0_ARB + i);
						OPENGL_ERROR_CHECK
						glBindTexture(sampler.texture->_target, sampler.texture->_id);
						OPENGL_ERROR_CHECK
						_glState.samplers[i] = sampler.texture->_id;
					}

					// rectangular textures have restrictions
					if(sampler.texture->_target == OBJ_TEXTURE_RECTANGLE)
					{
						assert(sampler.state->_state.addressU != TEX_ADDRESS_REPEAT);
						assert(sampler.state->_state.addressU != TEX_ADDRESS_MIRRORED_REPEAT);
						assert(sampler.state->_state.addressV != TEX_ADDRESS_REPEAT);
						assert(sampler.state->_state.addressV != TEX_ADDRESS_MIRRORED_REPEAT);
						assert(sampler.state->_state.addressW != TEX_ADDRESS_REPEAT);
						assert(sampler.state->_state.addressW != TEX_ADDRESS_MIRRORED_REPEAT);
						assert( sampler.state->_state.minFilter == TEX_FILTER_NEAREST ||
								sampler.state->_state.minFilter == TEX_FILTER_LINEAR );
						// is anisotropy allowed?
					}

					glTexParameteri(sampler.texture->_target, GL_TEXTURE_WRAP_S, sampler.state->_state.addressU);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_WRAP_T, sampler.state->_state.addressV);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_WRAP_R, sampler.state->_state.addressW);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_MIN_FILTER, sampler.state->_state.minFilter);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_MAG_FILTER, sampler.state->_state.magFilter);
					glTexParameterfv(sampler.texture->_target, GL_TEXTURE_BORDER_COLOR, sampler.state->_state.borderColor);
					glTexParameterf(sampler.texture->_target, GL_TEXTURE_MIN_LOD, sampler.state->_state.minLOD);
					glTexParameterf(sampler.texture->_target, GL_TEXTURE_MAX_LOD, sampler.state->_state.maxLOD);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_BASE_LEVEL, sampler.state->_state.baseLevel);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_MAX_LEVEL, sampler.state->_state.maxLevel);
					glTexParameterf(sampler.texture->_target, GL_TEXTURE_LOD_BIAS, sampler.state->_state.lodBias);
					glTexParameteri(sampler.texture->_target, GL_DEPTH_TEXTURE_MODE_ARB, sampler.state->_state.depthTexMode);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_COMPARE_MODE_ARB, sampler.state->_state.texCmpMode);
					glTexParameteri(sampler.texture->_target, GL_TEXTURE_COMPARE_FUNC_ARB, sampler.state->_state.texCmpFunc);
					glTexParameterf(sampler.texture->_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, sampler.state->_state.maxAnisotropy);
					OPENGL_ERROR_CHECK
				}
			}
		}
		glActiveTextureARB(GL_TEXTURE0_ARB);
		OPENGL_ERROR_CHECK

		// set shaders
		if(_glslProg)
		{
			// use GLSL program
			if(_glslProg->_id != _glState.glslProg)
			{
				glUseProgram(_glslProg->_id);
				OPENGL_ERROR_CHECK
				_glState.glslProg = _glslProg->_id;
			}
		}
		else
		{
			// use ARB asm programs
			if(_vertexASMProg)
			{
				if(_vertexASMProg->_id != _glState.asmVertProg)
				{
					glBindProgramARB(OBJ_ASM_VERTEX_PROGRAM, _vertexASMProg->_id);
					OPENGL_ERROR_CHECK
					_glState.asmVertProg = _vertexASMProg->_id;
				}
			}

			if(_geometryASMProg)
			{
				if(_geometryASMProg->_id != _glState.asmGeomProg)
				{
					glBindProgramARB(OBJ_ASM_FRAGMENT_PROGRAM, _geometryASMProg->_id);
					OPENGL_ERROR_CHECK
					_glState.asmGeomProg = _geometryASMProg->_id;
				}
			}

			if(_fragmentASMProg)
			{
				if(_fragmentASMProg->_id != _glState.asmFragProg)
				{
					glBindProgramARB(OBJ_ASM_FRAGMENT_PROGRAM, _fragmentASMProg->_id);
					OPENGL_ERROR_CHECK
					_glState.asmFragProg = _fragmentASMProg->_id;
				}
			}
		}
	}

	void Renderer::GetViewport(int& x, int& y, int& width, int& height)
	{
		x = _viewport[0];
		y = _viewport[1];
		width = _viewport[2];
		height = _viewport[3];
	}

	void Renderer::GetViewport(float& x, float& y, float& width, float& height)
	{
		x = (float)_viewport[0];
		y = (float)_viewport[1];
		width = (float)_viewport[2];
		height = (float)_viewport[3];
	}

	void Renderer::GetViewport(int viewport[4])
	{
		viewport[0] = _viewport[0];
		viewport[1] = _viewport[1];
		viewport[2] = _viewport[2];
		viewport[3] = _viewport[3];
	}

	void Renderer::GetViewport(float viewport[4])
	{
		viewport[0] = (float)_viewport[0];
		viewport[1] = (float)_viewport[1];
		viewport[2] = (float)_viewport[2];
		viewport[3] = (float)_viewport[3];
	}

	VertexFormat* Renderer::CreateVertexFormat(const VertexAttribDesc* descriptors, int count)
	{
		if(count <= 0 || !descriptors)
			return 0;

		VertexFormat* vformat = New<VertexFormat>(glPool) ;
		vformat->_count = count;
		vformat->_descriptors = NewArray<VertexAttribDesc>(glPool, count);

		for(int i = 0; i < count; ++i)
		{
			vformat->_descriptors[i] = descriptors[i];
		}

		return vformat;
	}

	void Renderer::DestroyVertexFormat(VertexFormat* vert_fmt)
	{
		if(_vertexFormat == vert_fmt)
			_vertexFormat = 0;

		Delete(vert_fmt);
	}

	SamplerState* Renderer::CreateSamplerState(const SamplerStateDesc& descriptor)
	{
		SamplerState* sstate = New<SamplerState>(glPool);
		sstate->_state = descriptor;
		return sstate;
	}

	void Renderer::DestroySamplerState(SamplerState* samp_state)
	{
		for(int i = 0; i < _info.maxTextureImageUnits; ++i)
			if(_samplers[i].state == samp_state)
				_samplers[i].state = 0;

		Delete(samp_state);
	}

	Texture1D* Renderer::CreateTexture1D()
	{
		Texture1D* tex = New<Texture1D>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	Texture1DArray* Renderer::CreateTexture1DArray()
	{
		Texture1DArray* tex = New<Texture1DArray>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	Texture2D* Renderer::CreateTexture2D()
	{
		Texture2D* tex = New<Texture2D>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	Texture2DArray* Renderer::CreateTexture2DArray()
	{
		Texture2DArray* tex = New<Texture2DArray>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	Texture3D* Renderer::CreateTexture3D()
	{
		Texture3D* tex = New<Texture3D>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	TextureCube* Renderer::CreateTextureCube()
	{
		TextureCube* tex = New<TextureCube>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	TextureBuffer* Renderer::CreateTextureBuffer()
	{
		TextureBuffer* tex = New<TextureBuffer>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	TextureRectangle* Renderer::CreateTextureRectangle()
	{
		TextureRectangle* tex = New<TextureRectangle>(glPool);
		bool result = tex->Create();
		if(!result)
		{
			Delete(tex);
			tex = 0;
		}
		return tex;
	}

	void Renderer::DestroyTexture(Texture* texture)
	{
		if(texture)
		{
			// remove sampler references to this texture
			for(int i = 0; i < _info.maxTextureImageUnits; ++i)
			{
				if(_samplers[i].texture == texture)
					_samplers[i].texture = 0;

				if(_glState.samplers[i] == texture->_id)
					_glState.samplers[i] = 0;
			}

			GLuint* cur_id = 0;
			switch(texture->_target)
			{
			case OBJ_TEXTURE_1D:
				cur_id = &_glState.tex1D;
				break;
			case OBJ_TEXTURE_2D:
				cur_id = &_glState.tex2D;
				break;
			case OBJ_TEXTURE_3D:
				cur_id = &_glState.tex3D;
				break;
			case OBJ_TEXTURE_CUBE:
				cur_id = &_glState.texCube;
				break;
			case OBJ_TEXTURE_1D_ARRAY:
				cur_id = &_glState.tex1DArray;
				break;
			case OBJ_TEXTURE_2D_ARRAY:
				cur_id = &_glState.tex2DArray;
				break;
			case OBJ_TEXTURE_BUFFER:
				cur_id = &_glState.texBuffer;
				break;
			case OBJ_TEXTURE_RECTANGLE:
				cur_id = &_glState.texRect;
				break;
			}

			assert(cur_id);
			if(texture->_id == *cur_id)
				*cur_id = 0;

			texture->Destroy();
			Delete(texture);
		}
	}

	Buffer* Renderer::CreateBuffer(ObjectType type, size_t size, const void* data, BufferUsage usage)
	{
		Buffer* buf = New<Buffer>(glPool);
		bool result = buf->Create(type, size, data, usage);
		if(!result)
		{
			Delete(buf);
			buf = 0;
		}
		return buf;
	}

	void Renderer::DestroyBuffer(Buffer* buffer)
	{
		if(buffer)
		{
			// remove all references to this buffer
			
			for(int i = 0; i < MAX_VERTEX_STREAMS; ++i)
				if(_vertexStreams[i].buffer == buffer)
					_vertexStreams[i].buffer = 0;

			for(int i = 0; i < _info.maxVertexAttribs; ++i)
			{
				if(_vertexAttribs[i].buffer == buffer)
				{
					_vertexAttribs[i].buffer = 0;

					if(_vertexAttribs[i].enabled)
					{
						_vertexAttribs[i].enabled = false;
						glDisableVertexAttribArrayARB(i);
						OPENGL_ERROR_CHECK
					}
				}
			}

			if(_indexBuffer == buffer)
				_indexBuffer = 0;

			if(buffer->_id == _glState.vertexBuf)
				_glState.vertexBuf = 0;
			if(buffer->_id == _glState.indexBuf)
				_glState.indexBuf = 0;
			if(buffer->_id == _glState.pixelPackBuf)
				_glState.pixelPackBuf = 0;
			if(buffer->_id == _glState.pixelUnpackBuf)
				_glState.pixelUnpackBuf = 0;
			if(buffer->_id == _glState.textureBuf)
				_glState.textureBuf = 0;
			if(buffer->_id == _glState.uniformBuf)
				_glState.uniformBuf = 0;
			if(buffer->_id == _glState.transfFeedbackBuf)
				_glState.transfFeedbackBuf = 0;

			buffer->Destroy();
			Delete(buffer);
		}
	}

	Framebuffer* Renderer::CreateFramebuffer(ObjectType type)
	{
		Framebuffer* fbuf = New<Framebuffer>(glPool);
		bool result = fbuf->Create(type);
		if(!result)
		{
			Delete(fbuf);
			fbuf = 0;
		}
		return fbuf;
	}

	void Renderer::DestroyFramebuffer(Framebuffer* framebuffer)
	{
		if(framebuffer)
		{
			if(_drawFramebuffer == framebuffer)
				_drawFramebuffer = 0;

			if(_readFramebuffer == framebuffer)
				_readFramebuffer = 0;

			if(framebuffer->_id == _glState.drawFbuf)
				_glState.drawFbuf = 0;

			if(framebuffer->_id == _glState.readFbuf)
				_glState.readFbuf = 0;

			framebuffer->Destroy();
			Delete(framebuffer);
		}
	}

	Renderbuffer* Renderer::CreateRenderbuffer()
	{
		Renderbuffer* rbuf = New<Renderbuffer>(glPool);
		if(!rbuf->Create())
		{
			Delete(rbuf);
			rbuf = 0;
		}
		return rbuf;
	}

	void Renderer::DestroyRenderbuffer(Renderbuffer* renderbuffer)
	{
		if(renderbuffer)
		{
			if(renderbuffer->_id == _glState.renderbuffer)
				_glState.renderbuffer = 0;

			renderbuffer->Destroy();
			Delete(renderbuffer);
		}
	}

	Query* Renderer::CreateQuery(ObjectType type)
	{
		Query* query = New<Query>(glPool);
		query->Create(type);
		return query;
	}

	void Renderer::DestroyQuery(Query* query)
	{
		if(query)
		{
			query->Destroy();
			Delete(query);
		}
	}

	GLSLShader* Renderer::CreateShader(ObjectType type, int count, const char** source, const char*& error_string)
	{
		GLSLShader* shader = New<GLSLShader>(glPool);
		if(!shader->Create(type, count, source, error_string))
		{
			Delete(shader);
			shader = 0;
		}
		return shader;
	}

	void Renderer::DestroyShader(GLSLShader* shader)
	{
		if(shader)
		{
			shader->Destroy();
			Delete(shader);
		}
	}

	GLSLProgram* Renderer::CreateProgram(size_t count, GLSLShader** shaders)
	{
		GLSLProgram* prog = New<GLSLProgram>(glPool);
		if(!prog->Create(count, shaders))
		{
			Delete(prog);
			prog = 0;
		}
		return prog;
	}

	void Renderer::DestroyProgram(GLSLProgram* program)
	{
		if(program)
		{
			if(_glslProg == program)
				_glslProg = 0;

			if(_glState.glslProg == program->_id)
				_glState.glslProg = 0;

			program->Destroy();
			Delete(program);
		}
	}

	ASMProgram* Renderer::CreateASMProgram(ObjectType type, const char* source, const char*& error_string)
	{
		ASMProgram* prog = New<ASMProgram>(glPool);
		if(!prog->Create(type, source, error_string))
		{
			Delete(prog);
			prog = 0;
		}
		return prog;
	}

	void Renderer::DestroyASMProgram(ASMProgram* program)
	{
		if(program)
		{
			if(program->_target == OBJ_ASM_VERTEX_PROGRAM)
			{
				if(_vertexASMProg == program)
					_vertexASMProg = 0;

				if(_glState.asmVertProg == program->_id)
					_glState.asmVertProg = 0;
			}
			else if(program->_target == OBJ_ASM_GEOMETRY_PROGRAM)
			{
				if(_geometryASMProg == program)
					_geometryASMProg = 0;

				if(_glState.asmGeomProg == program->_id)
					_glState.asmGeomProg = 0;
			}
			else if(program->_target == OBJ_ASM_FRAGMENT_PROGRAM)
			{
				if(_fragmentASMProg == program)
					_fragmentASMProg = 0;

				if(_glState.asmFragProg == program->_id)
					_glState.asmFragProg = 0;
			}

			program->Destroy();
			Delete(program);
		}
	}

}
