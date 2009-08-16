
// GL_VERSION_2_0

bool glextLoad_GL_VERSION_2_0()
{
	INIT_FUNC_PTR(glBlendEquationSeparate);
	INIT_FUNC_PTR(glDrawBuffers);
	INIT_FUNC_PTR(glStencilOpSeparate);
	INIT_FUNC_PTR(glStencilFuncSeparate);
	INIT_FUNC_PTR(glStencilMaskSeparate);
	INIT_FUNC_PTR(glAttachShader);
	INIT_FUNC_PTR(glBindAttribLocation);
	INIT_FUNC_PTR(glCompileShader);
	INIT_FUNC_PTR(glCreateProgram);
	INIT_FUNC_PTR(glCreateShader);
	INIT_FUNC_PTR(glDeleteProgram);
	INIT_FUNC_PTR(glDeleteShader);
	INIT_FUNC_PTR(glDetachShader);
	INIT_FUNC_PTR(glDisableVertexAttribArray);
	INIT_FUNC_PTR(glEnableVertexAttribArray);
	INIT_FUNC_PTR(glGetActiveAttrib);
	INIT_FUNC_PTR(glGetActiveUniform);
	INIT_FUNC_PTR(glGetAttachedShaders);
	INIT_FUNC_PTR(glGetAttribLocation);
	INIT_FUNC_PTR(glGetProgramiv);
	INIT_FUNC_PTR(glGetProgramInfoLog);
	INIT_FUNC_PTR(glGetShaderiv);
	INIT_FUNC_PTR(glGetShaderInfoLog);
	INIT_FUNC_PTR(glGetShaderSource);
	INIT_FUNC_PTR(glGetUniformLocation);
	INIT_FUNC_PTR(glGetUniformfv);
	INIT_FUNC_PTR(glGetUniformiv);
	INIT_FUNC_PTR(glGetVertexAttribdv);
	INIT_FUNC_PTR(glGetVertexAttribfv);
	INIT_FUNC_PTR(glGetVertexAttribiv);
	INIT_FUNC_PTR(glGetVertexAttribPointerv);
	INIT_FUNC_PTR(glIsProgram);
	INIT_FUNC_PTR(glIsShader);
	INIT_FUNC_PTR(glLinkProgram);
	INIT_FUNC_PTR(glShaderSource);
	INIT_FUNC_PTR(glUseProgram);
	INIT_FUNC_PTR(glUniform1f);
	INIT_FUNC_PTR(glUniform2f);
	INIT_FUNC_PTR(glUniform3f);
	INIT_FUNC_PTR(glUniform4f);
	INIT_FUNC_PTR(glUniform1i);
	INIT_FUNC_PTR(glUniform2i);
	INIT_FUNC_PTR(glUniform3i);
	INIT_FUNC_PTR(glUniform4i);
	INIT_FUNC_PTR(glUniform1fv);
	INIT_FUNC_PTR(glUniform2fv);
	INIT_FUNC_PTR(glUniform3fv);
	INIT_FUNC_PTR(glUniform4fv);
	INIT_FUNC_PTR(glUniform1iv);
	INIT_FUNC_PTR(glUniform2iv);
	INIT_FUNC_PTR(glUniform3iv);
	INIT_FUNC_PTR(glUniform4iv);
	INIT_FUNC_PTR(glUniformMatrix2fv);
	INIT_FUNC_PTR(glUniformMatrix3fv);
	INIT_FUNC_PTR(glUniformMatrix4fv);
	INIT_FUNC_PTR(glValidateProgram);
	INIT_FUNC_PTR(glVertexAttrib1d);
	INIT_FUNC_PTR(glVertexAttrib1dv);
	INIT_FUNC_PTR(glVertexAttrib1f);
	INIT_FUNC_PTR(glVertexAttrib1fv);
	INIT_FUNC_PTR(glVertexAttrib1s);
	INIT_FUNC_PTR(glVertexAttrib1sv);
	INIT_FUNC_PTR(glVertexAttrib2d);
	INIT_FUNC_PTR(glVertexAttrib2dv);
	INIT_FUNC_PTR(glVertexAttrib2f);
	INIT_FUNC_PTR(glVertexAttrib2fv);
	INIT_FUNC_PTR(glVertexAttrib2s);
	INIT_FUNC_PTR(glVertexAttrib2sv);
	INIT_FUNC_PTR(glVertexAttrib3d);
	INIT_FUNC_PTR(glVertexAttrib3dv);
	INIT_FUNC_PTR(glVertexAttrib3f);
	INIT_FUNC_PTR(glVertexAttrib3fv);
	INIT_FUNC_PTR(glVertexAttrib3s);
	INIT_FUNC_PTR(glVertexAttrib3sv);
	INIT_FUNC_PTR(glVertexAttrib4Nbv);
	INIT_FUNC_PTR(glVertexAttrib4Niv);
	INIT_FUNC_PTR(glVertexAttrib4Nsv);
	INIT_FUNC_PTR(glVertexAttrib4Nub);
	INIT_FUNC_PTR(glVertexAttrib4Nubv);
	INIT_FUNC_PTR(glVertexAttrib4Nuiv);
	INIT_FUNC_PTR(glVertexAttrib4Nusv);
	INIT_FUNC_PTR(glVertexAttrib4bv);
	INIT_FUNC_PTR(glVertexAttrib4d);
	INIT_FUNC_PTR(glVertexAttrib4dv);
	INIT_FUNC_PTR(glVertexAttrib4f);
	INIT_FUNC_PTR(glVertexAttrib4fv);
	INIT_FUNC_PTR(glVertexAttrib4iv);
	INIT_FUNC_PTR(glVertexAttrib4s);
	INIT_FUNC_PTR(glVertexAttrib4sv);
	INIT_FUNC_PTR(glVertexAttrib4ubv);
	INIT_FUNC_PTR(glVertexAttrib4uiv);
	INIT_FUNC_PTR(glVertexAttrib4usv);
	INIT_FUNC_PTR(glVertexAttribPointer);

	return true;
}

// GL_VERSION_2_1

bool glextLoad_GL_VERSION_2_1()
{
	INIT_FUNC_PTR(glUniformMatrix2x3fv);
	INIT_FUNC_PTR(glUniformMatrix3x2fv);
	INIT_FUNC_PTR(glUniformMatrix2x4fv);
	INIT_FUNC_PTR(glUniformMatrix4x2fv);
	INIT_FUNC_PTR(glUniformMatrix3x4fv);
	INIT_FUNC_PTR(glUniformMatrix4x3fv);

	return true;
}

// GL_ARB_multitexture

bool glextLoad_GL_ARB_multitexture()
{
	INIT_FUNC_PTR(glActiveTextureARB);
	INIT_FUNC_PTR(glClientActiveTextureARB);
	INIT_FUNC_PTR(glMultiTexCoord1dARB);
	INIT_FUNC_PTR(glMultiTexCoord1dvARB);
	INIT_FUNC_PTR(glMultiTexCoord1fARB);
	INIT_FUNC_PTR(glMultiTexCoord1fvARB);
	INIT_FUNC_PTR(glMultiTexCoord1iARB);
	INIT_FUNC_PTR(glMultiTexCoord1ivARB);
	INIT_FUNC_PTR(glMultiTexCoord1sARB);
	INIT_FUNC_PTR(glMultiTexCoord1svARB);
	INIT_FUNC_PTR(glMultiTexCoord2dARB);
	INIT_FUNC_PTR(glMultiTexCoord2dvARB);
	INIT_FUNC_PTR(glMultiTexCoord2fARB);
	INIT_FUNC_PTR(glMultiTexCoord2fvARB);
	INIT_FUNC_PTR(glMultiTexCoord2iARB);
	INIT_FUNC_PTR(glMultiTexCoord2ivARB);
	INIT_FUNC_PTR(glMultiTexCoord2sARB);
	INIT_FUNC_PTR(glMultiTexCoord2svARB);
	INIT_FUNC_PTR(glMultiTexCoord3dARB);
	INIT_FUNC_PTR(glMultiTexCoord3dvARB);
	INIT_FUNC_PTR(glMultiTexCoord3fARB);
	INIT_FUNC_PTR(glMultiTexCoord3fvARB);
	INIT_FUNC_PTR(glMultiTexCoord3iARB);
	INIT_FUNC_PTR(glMultiTexCoord3ivARB);
	INIT_FUNC_PTR(glMultiTexCoord3sARB);
	INIT_FUNC_PTR(glMultiTexCoord3svARB);
	INIT_FUNC_PTR(glMultiTexCoord4dARB);
	INIT_FUNC_PTR(glMultiTexCoord4dvARB);
	INIT_FUNC_PTR(glMultiTexCoord4fARB);
	INIT_FUNC_PTR(glMultiTexCoord4fvARB);
	INIT_FUNC_PTR(glMultiTexCoord4iARB);
	INIT_FUNC_PTR(glMultiTexCoord4ivARB);
	INIT_FUNC_PTR(glMultiTexCoord4sARB);
	INIT_FUNC_PTR(glMultiTexCoord4svARB);

	return true;
}

// GL_ARB_multisample

bool glextLoad_GL_ARB_multisample()
{
	INIT_FUNC_PTR(glSampleCoverageARB);

	return true;
}

// GL_ARB_texture_compression

bool glextLoad_GL_ARB_texture_compression()
{
	INIT_FUNC_PTR(glCompressedTexImage3DARB);
	INIT_FUNC_PTR(glCompressedTexImage2DARB);
	INIT_FUNC_PTR(glCompressedTexImage1DARB);
	INIT_FUNC_PTR(glCompressedTexSubImage3DARB);
	INIT_FUNC_PTR(glCompressedTexSubImage2DARB);
	INIT_FUNC_PTR(glCompressedTexSubImage1DARB);
	INIT_FUNC_PTR(glGetCompressedTexImageARB);

	return true;
}

// GL_ARB_vertex_program

bool glextLoad_GL_ARB_vertex_program()
{
	INIT_FUNC_PTR(glVertexAttrib1dARB);
	INIT_FUNC_PTR(glVertexAttrib1dvARB);
	INIT_FUNC_PTR(glVertexAttrib1fARB);
	INIT_FUNC_PTR(glVertexAttrib1fvARB);
	INIT_FUNC_PTR(glVertexAttrib1sARB);
	INIT_FUNC_PTR(glVertexAttrib1svARB);
	INIT_FUNC_PTR(glVertexAttrib2dARB);
	INIT_FUNC_PTR(glVertexAttrib2dvARB);
	INIT_FUNC_PTR(glVertexAttrib2fARB);
	INIT_FUNC_PTR(glVertexAttrib2fvARB);
	INIT_FUNC_PTR(glVertexAttrib2sARB);
	INIT_FUNC_PTR(glVertexAttrib2svARB);
	INIT_FUNC_PTR(glVertexAttrib3dARB);
	INIT_FUNC_PTR(glVertexAttrib3dvARB);
	INIT_FUNC_PTR(glVertexAttrib3fARB);
	INIT_FUNC_PTR(glVertexAttrib3fvARB);
	INIT_FUNC_PTR(glVertexAttrib3sARB);
	INIT_FUNC_PTR(glVertexAttrib3svARB);
	INIT_FUNC_PTR(glVertexAttrib4NbvARB);
	INIT_FUNC_PTR(glVertexAttrib4NivARB);
	INIT_FUNC_PTR(glVertexAttrib4NsvARB);
	INIT_FUNC_PTR(glVertexAttrib4NubARB);
	INIT_FUNC_PTR(glVertexAttrib4NubvARB);
	INIT_FUNC_PTR(glVertexAttrib4NuivARB);
	INIT_FUNC_PTR(glVertexAttrib4NusvARB);
	INIT_FUNC_PTR(glVertexAttrib4bvARB);
	INIT_FUNC_PTR(glVertexAttrib4dARB);
	INIT_FUNC_PTR(glVertexAttrib4dvARB);
	INIT_FUNC_PTR(glVertexAttrib4fARB);
	INIT_FUNC_PTR(glVertexAttrib4fvARB);
	INIT_FUNC_PTR(glVertexAttrib4ivARB);
	INIT_FUNC_PTR(glVertexAttrib4sARB);
	INIT_FUNC_PTR(glVertexAttrib4svARB);
	INIT_FUNC_PTR(glVertexAttrib4ubvARB);
	INIT_FUNC_PTR(glVertexAttrib4uivARB);
	INIT_FUNC_PTR(glVertexAttrib4usvARB);
	INIT_FUNC_PTR(glVertexAttribPointerARB);
	INIT_FUNC_PTR(glEnableVertexAttribArrayARB);
	INIT_FUNC_PTR(glDisableVertexAttribArrayARB);
	INIT_FUNC_PTR(glProgramStringARB);
	INIT_FUNC_PTR(glBindProgramARB);
	INIT_FUNC_PTR(glDeleteProgramsARB);
	INIT_FUNC_PTR(glGenProgramsARB);
	INIT_FUNC_PTR(glProgramEnvParameter4dARB);
	INIT_FUNC_PTR(glProgramEnvParameter4dvARB);
	INIT_FUNC_PTR(glProgramEnvParameter4fARB);
	INIT_FUNC_PTR(glProgramEnvParameter4fvARB);
	INIT_FUNC_PTR(glProgramLocalParameter4dARB);
	INIT_FUNC_PTR(glProgramLocalParameter4dvARB);
	INIT_FUNC_PTR(glProgramLocalParameter4fARB);
	INIT_FUNC_PTR(glProgramLocalParameter4fvARB);
	INIT_FUNC_PTR(glGetProgramEnvParameterdvARB);
	INIT_FUNC_PTR(glGetProgramEnvParameterfvARB);
	INIT_FUNC_PTR(glGetProgramLocalParameterdvARB);
	INIT_FUNC_PTR(glGetProgramLocalParameterfvARB);
	INIT_FUNC_PTR(glGetProgramivARB);
	INIT_FUNC_PTR(glGetProgramStringARB);
	INIT_FUNC_PTR(glGetVertexAttribdvARB);
	INIT_FUNC_PTR(glGetVertexAttribfvARB);
	INIT_FUNC_PTR(glGetVertexAttribivARB);
	INIT_FUNC_PTR(glGetVertexAttribPointervARB);
	INIT_FUNC_PTR(glIsProgramARB);

	return true;
}

// GL_ARB_vertex_buffer_object

bool glextLoad_GL_ARB_vertex_buffer_object()
{
	INIT_FUNC_PTR(glBindBufferARB);
	INIT_FUNC_PTR(glDeleteBuffersARB);
	INIT_FUNC_PTR(glGenBuffersARB);
	INIT_FUNC_PTR(glIsBufferARB);
	INIT_FUNC_PTR(glBufferDataARB);
	INIT_FUNC_PTR(glBufferSubDataARB);
	INIT_FUNC_PTR(glGetBufferSubDataARB);
	INIT_FUNC_PTR(glMapBufferARB);
	INIT_FUNC_PTR(glUnmapBufferARB);
	INIT_FUNC_PTR(glGetBufferParameterivARB);
	INIT_FUNC_PTR(glGetBufferPointervARB);

	return true;
}

// GL_ARB_occlusion_query

bool glextLoad_GL_ARB_occlusion_query()
{
	INIT_FUNC_PTR(glGenQueriesARB);
	INIT_FUNC_PTR(glDeleteQueriesARB);
	INIT_FUNC_PTR(glIsQueryARB);
	INIT_FUNC_PTR(glBeginQueryARB);
	INIT_FUNC_PTR(glEndQueryARB);
	INIT_FUNC_PTR(glGetQueryivARB);
	INIT_FUNC_PTR(glGetQueryObjectivARB);
	INIT_FUNC_PTR(glGetQueryObjectuivARB);

	return true;
}

// GL_ARB_draw_buffers

bool glextLoad_GL_ARB_draw_buffers()
{
	INIT_FUNC_PTR(glDrawBuffersARB);

	return true;
}

// GL_ARB_color_buffer_float

bool glextLoad_GL_ARB_color_buffer_float()
{
	INIT_FUNC_PTR(glClampColorARB);

	return true;
}

// GL_ARB_draw_instanced

bool glextLoad_GL_ARB_draw_instanced()
{
	INIT_FUNC_PTR(glDrawArraysInstancedARB);
	INIT_FUNC_PTR(glDrawElementsInstancedARB);

	return true;
}

// GL_ARB_geometry_shader4

bool glextLoad_GL_ARB_geometry_shader4()
{
	INIT_FUNC_PTR(glProgramParameteriARB);
	INIT_FUNC_PTR(glFramebufferTextureARB);
	INIT_FUNC_PTR(glFramebufferTextureLayerARB);
	INIT_FUNC_PTR(glFramebufferTextureFaceARB);

	return true;
}

// GL_ARB_map_buffer_range

bool glextLoad_GL_ARB_map_buffer_range()
{
	INIT_FUNC_PTR(glMapBufferRange);
	INIT_FUNC_PTR(glFlushMappedBufferRange);

	return true;
}

// GL_ARB_texture_buffer_object

bool glextLoad_GL_ARB_texture_buffer_object()
{
	INIT_FUNC_PTR(glTexBufferARB);

	return true;
}

// GL_ARB_uniform_buffer_object

bool glextLoad_GL_ARB_uniform_buffer_object()
{
	INIT_FUNC_PTR(glGetUniformIndices);
	INIT_FUNC_PTR(glGetActiveUniformsiv);
	INIT_FUNC_PTR(glGetActiveUniformName);
	INIT_FUNC_PTR(glGetUniformBlockIndex);
	INIT_FUNC_PTR(glGetActiveUniformBlockiv);
	INIT_FUNC_PTR(glGetActiveUniformBlockName);
	INIT_FUNC_PTR(glUniformBlockBinding);

	return true;
}

// GL_ARB_copy_buffer

bool glextLoad_GL_ARB_copy_buffer()
{
	INIT_FUNC_PTR(glCopyBufferSubData);

	return true;
}

// GL_EXT_blend_color

bool glextLoad_GL_EXT_blend_color()
{
	INIT_FUNC_PTR(glBlendColorEXT);

	return true;
}

// GL_EXT_texture3D

bool glextLoad_GL_EXT_texture3D()
{
	INIT_FUNC_PTR(glTexImage3DEXT);
	INIT_FUNC_PTR(glTexSubImage3DEXT);

	return true;
}

// GL_EXT_blend_minmax

bool glextLoad_GL_EXT_blend_minmax()
{
	INIT_FUNC_PTR(glBlendEquationEXT);

	return true;
}

// GL_EXT_draw_range_elements

bool glextLoad_GL_EXT_draw_range_elements()
{
	INIT_FUNC_PTR(glDrawRangeElementsEXT);

	return true;
}

// GL_EXT_multi_draw_arrays

bool glextLoad_GL_EXT_multi_draw_arrays()
{
	INIT_FUNC_PTR(glMultiDrawArraysEXT);
	INIT_FUNC_PTR(glMultiDrawElementsEXT);

	return true;
}

// GL_EXT_blend_func_separate

bool glextLoad_GL_EXT_blend_func_separate()
{
	INIT_FUNC_PTR(glBlendFuncSeparateEXT);

	return true;
}

// GL_NV_primitive_restart

bool glextLoad_GL_NV_primitive_restart()
{
	INIT_FUNC_PTR(glPrimitiveRestartNV);
	INIT_FUNC_PTR(glPrimitiveRestartIndexNV);

	return true;
}

// GL_EXT_blend_equation_separate

bool glextLoad_GL_EXT_blend_equation_separate()
{
	INIT_FUNC_PTR(glBlendEquationSeparateEXT);

	return true;
}

// GL_EXT_framebuffer_object

bool glextLoad_GL_EXT_framebuffer_object()
{
	INIT_FUNC_PTR(glIsRenderbufferEXT);
	INIT_FUNC_PTR(glBindRenderbufferEXT);
	INIT_FUNC_PTR(glDeleteRenderbuffersEXT);
	INIT_FUNC_PTR(glGenRenderbuffersEXT);
	INIT_FUNC_PTR(glRenderbufferStorageEXT);
	INIT_FUNC_PTR(glGetRenderbufferParameterivEXT);
	INIT_FUNC_PTR(glIsFramebufferEXT);
	INIT_FUNC_PTR(glBindFramebufferEXT);
	INIT_FUNC_PTR(glDeleteFramebuffersEXT);
	INIT_FUNC_PTR(glGenFramebuffersEXT);
	INIT_FUNC_PTR(glCheckFramebufferStatusEXT);
	INIT_FUNC_PTR(glFramebufferTexture1DEXT);
	INIT_FUNC_PTR(glFramebufferTexture2DEXT);
	INIT_FUNC_PTR(glFramebufferTexture3DEXT);
	INIT_FUNC_PTR(glFramebufferRenderbufferEXT);
	INIT_FUNC_PTR(glGetFramebufferAttachmentParameterivEXT);
	INIT_FUNC_PTR(glGenerateMipmapEXT);

	return true;
}

// GL_EXT_framebuffer_blit

bool glextLoad_GL_EXT_framebuffer_blit()
{
	INIT_FUNC_PTR(glBlitFramebufferEXT);

	return true;
}

// GL_EXT_framebuffer_multisample

bool glextLoad_GL_EXT_framebuffer_multisample()
{
	INIT_FUNC_PTR(glRenderbufferStorageMultisampleEXT);

	return true;
}

// GL_EXT_gpu_program_parameters

bool glextLoad_GL_EXT_gpu_program_parameters()
{
	INIT_FUNC_PTR(glProgramEnvParameters4fvEXT);
	INIT_FUNC_PTR(glProgramLocalParameters4fvEXT);

	return true;
}

// GL_NV_gpu_program4

bool glextLoad_GL_NV_gpu_program4()
{
	INIT_FUNC_PTR(glProgramLocalParameterI4iNV);
	INIT_FUNC_PTR(glProgramLocalParameterI4ivNV);
	INIT_FUNC_PTR(glProgramLocalParametersI4ivNV);
	INIT_FUNC_PTR(glProgramLocalParameterI4uiNV);
	INIT_FUNC_PTR(glProgramLocalParameterI4uivNV);
	INIT_FUNC_PTR(glProgramLocalParametersI4uivNV);
	INIT_FUNC_PTR(glProgramEnvParameterI4iNV);
	INIT_FUNC_PTR(glProgramEnvParameterI4ivNV);
	INIT_FUNC_PTR(glProgramEnvParametersI4ivNV);
	INIT_FUNC_PTR(glProgramEnvParameterI4uiNV);
	INIT_FUNC_PTR(glProgramEnvParameterI4uivNV);
	INIT_FUNC_PTR(glProgramEnvParametersI4uivNV);
	INIT_FUNC_PTR(glGetProgramLocalParameterIivNV);
	INIT_FUNC_PTR(glGetProgramLocalParameterIuivNV);
	INIT_FUNC_PTR(glGetProgramEnvParameterIivNV);
	INIT_FUNC_PTR(glGetProgramEnvParameterIuivNV);

	return true;
}

// GL_NV_geometry_program4

bool glextLoad_GL_NV_geometry_program4()
{
	INIT_FUNC_PTR(glProgramVertexLimitNV);
	INIT_FUNC_PTR(glFramebufferTextureEXT);
	INIT_FUNC_PTR(glFramebufferTextureLayerEXT);
	INIT_FUNC_PTR(glFramebufferTextureFaceEXT);

	return true;
}

// GL_NV_vertex_program4

bool glextLoad_GL_NV_vertex_program4()
{
	INIT_FUNC_PTR(glVertexAttribI1iEXT);
	INIT_FUNC_PTR(glVertexAttribI2iEXT);
	INIT_FUNC_PTR(glVertexAttribI3iEXT);
	INIT_FUNC_PTR(glVertexAttribI4iEXT);
	INIT_FUNC_PTR(glVertexAttribI1uiEXT);
	INIT_FUNC_PTR(glVertexAttribI2uiEXT);
	INIT_FUNC_PTR(glVertexAttribI3uiEXT);
	INIT_FUNC_PTR(glVertexAttribI4uiEXT);
	INIT_FUNC_PTR(glVertexAttribI1ivEXT);
	INIT_FUNC_PTR(glVertexAttribI2ivEXT);
	INIT_FUNC_PTR(glVertexAttribI3ivEXT);
	INIT_FUNC_PTR(glVertexAttribI4ivEXT);
	INIT_FUNC_PTR(glVertexAttribI1uivEXT);
	INIT_FUNC_PTR(glVertexAttribI2uivEXT);
	INIT_FUNC_PTR(glVertexAttribI3uivEXT);
	INIT_FUNC_PTR(glVertexAttribI4uivEXT);
	INIT_FUNC_PTR(glVertexAttribI4bvEXT);
	INIT_FUNC_PTR(glVertexAttribI4svEXT);
	INIT_FUNC_PTR(glVertexAttribI4ubvEXT);
	INIT_FUNC_PTR(glVertexAttribI4usvEXT);
	INIT_FUNC_PTR(glVertexAttribIPointerEXT);
	INIT_FUNC_PTR(glGetVertexAttribIivEXT);
	INIT_FUNC_PTR(glGetVertexAttribIuivEXT);

	return true;
}

// GL_EXT_gpu_shader4

bool glextLoad_GL_EXT_gpu_shader4()
{
	INIT_FUNC_PTR(glGetUniformuivEXT);
	INIT_FUNC_PTR(glBindFragDataLocationEXT);
	INIT_FUNC_PTR(glGetFragDataLocationEXT);
	INIT_FUNC_PTR(glUniform1uiEXT);
	INIT_FUNC_PTR(glUniform2uiEXT);
	INIT_FUNC_PTR(glUniform3uiEXT);
	INIT_FUNC_PTR(glUniform4uiEXT);
	INIT_FUNC_PTR(glUniform1uivEXT);
	INIT_FUNC_PTR(glUniform2uivEXT);
	INIT_FUNC_PTR(glUniform3uivEXT);
	INIT_FUNC_PTR(glUniform4uivEXT);

	return true;
}

// GL_EXT_draw_buffers2

bool glextLoad_GL_EXT_draw_buffers2()
{
	INIT_FUNC_PTR(glColorMaskIndexedEXT);
	INIT_FUNC_PTR(glGetBooleanIndexedvEXT);
	INIT_FUNC_PTR(glGetIntegerIndexedvEXT);
	INIT_FUNC_PTR(glEnableIndexedEXT);
	INIT_FUNC_PTR(glDisableIndexedEXT);
	INIT_FUNC_PTR(glIsEnabledIndexedEXT);

	return true;
}

// GL_NV_conditional_render

bool glextLoad_GL_NV_conditional_render()
{
	INIT_FUNC_PTR(glBeginConditionalRenderNV);
	INIT_FUNC_PTR(glEndConditionalRenderNV);

	return true;
}

// GL_EXT_transform_feedback

bool glextLoad_GL_EXT_transform_feedback()
{
	INIT_FUNC_PTR(glBeginTransformFeedbackEXT);
	INIT_FUNC_PTR(glEndTransformFeedbackEXT);
	INIT_FUNC_PTR(glBindBufferRangeEXT);
	INIT_FUNC_PTR(glBindBufferOffsetEXT);
	INIT_FUNC_PTR(glBindBufferBaseEXT);
	INIT_FUNC_PTR(glTransformFeedbackVaryingsEXT);
	INIT_FUNC_PTR(glGetTransformFeedbackVaryingEXT);

	return true;
}
