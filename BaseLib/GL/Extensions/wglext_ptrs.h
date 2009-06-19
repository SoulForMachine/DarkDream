
// WGL_ARB_extensions_string

EXTPTR bool glextLoad_WGL_ARB_extensions_string();

EXTPTR PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

// WGL_ARB_pixel_format

EXTPTR bool glextLoad_WGL_ARB_pixel_format();

EXTPTR PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
EXTPTR PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
EXTPTR PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

// WGL_ARB_create_context

EXTPTR bool glextLoad_WGL_ARB_create_context();

EXTPTR PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

// WGL_EXT_swap_control

EXTPTR bool glextLoad_WGL_EXT_swap_control();

EXTPTR PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
EXTPTR PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
