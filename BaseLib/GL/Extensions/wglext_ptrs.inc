
// WGL_ARB_extensions_string

bool glextLoad_WGL_ARB_extensions_string()
{
	INIT_FUNC_PTR(wglGetExtensionsStringARB);

	return true;
}

// WGL_ARB_pixel_format

bool glextLoad_WGL_ARB_pixel_format()
{
	INIT_FUNC_PTR(wglGetPixelFormatAttribivARB);
	INIT_FUNC_PTR(wglGetPixelFormatAttribfvARB);
	INIT_FUNC_PTR(wglChoosePixelFormatARB);

	return true;
}

// WGL_ARB_create_context

bool glextLoad_WGL_ARB_create_context()
{
	INIT_FUNC_PTR(wglCreateContextAttribsARB);

	return true;
}

// WGL_EXT_swap_control

bool glextLoad_WGL_EXT_swap_control()
{
	INIT_FUNC_PTR(wglSwapIntervalEXT);
	INIT_FUNC_PTR(wglGetSwapIntervalEXT);

	return true;
}
