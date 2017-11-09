#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <iostream>
#include <GL/GL.h>

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool InitializeExtensions(HWND hWnd);
bool InitOpenGL(HWND hWnd);
bool gQuit = false;
HDC gDeviceContext;
void render();
int main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);



	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = _T("XRender");
	wc.lpfnWndProc = WindowProc;

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowExW(0,
		L"XRender",      // name of the window class
		L"XRender-Window",             // title of the window
		WS_OVERLAPPEDWINDOW,              // window style
		CW_USEDEFAULT,                    // x-position of the window
		CW_USEDEFAULT,                    // y-position of the window
		800,             // width of the window
		600,            // height of the window
		NULL,                             // we have no parent window, NULL
		NULL,                             // we aren't using menus, NULL
		hInstance,                        // application handle
		NULL);                            // used with multiple windows, NULL

	
	ShowWindow(hWnd, SW_HIDE);
	if (InitializeExtensions(hWnd))
	{
		std::cout << "Initialize extensions complate.\n";
	}
	else {
		std::cout << "Initialize extensions failed.\n";
		return false;
	}
	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(hWnd);
	hWnd = nullptr;

	hWnd = CreateWindowExW(0,
		L"XRender",      // name of the window class
		L"XRender-Window",             // title of the window
		WS_OVERLAPPEDWINDOW,              // window style
		CW_USEDEFAULT,                    // x-position of the window
		CW_USEDEFAULT,                    // y-position of the window
		800,             // width of the window
		600,            // height of the window
		NULL,                             // we have no parent window, NULL
		NULL,                             // we aren't using menus, NULL
		hInstance,                        // application handle
		NULL);

	
	if (InitOpenGL(hWnd)) {
		std::cout << "Init OpenGL Sucessed!";
	}
	else {
		std::cout << "Init OpenGL Failed!";
	}
	gQuit = false;

	ShowWindow(hWnd, SW_SHOW);
	// Message Loop
	MSG msg;
	while (!gQuit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}

		render();
	}

	return 0;
}

/////////////
// DEFINES //
/////////////
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_TEXTURE0                       0x84C0
#define GL_BGRA                           0x80E1
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

//////////////
// TYPEDEFS //
//////////////
typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats,
	int *piFormats, UINT *nNumFormats);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);


PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;


bool LoadExtensionList()
{

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)return false;

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	if (!wglCreateContextAttribsARB) return false;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (!wglSwapIntervalEXT)
	{
		return false;
	}

	return true;
}

bool InitializeExtensions(HWND hWnd)
{
	HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;
	bool result;

	// 获得窗口的设备环境句柄
	deviceContext = GetDC(hWnd);
	if (!deviceContext)return false;

	// 设置一个临时的默认像素格式
	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if (error != 1)return false;

	// 创建一个临时渲染上下文
	renderContext = wglCreateContext(deviceContext);
	if (!renderContext)return false;

	// 将刚才创建的临时渲染上下文,设置为这个窗口的当前渲染上下文.
	error = wglMakeCurrent(deviceContext, renderContext);
	if (error != 1) return false;

	// 以扩展的方式加载最新版OpenGL
	result = LoadExtensionList();
	if (!result) return false;

	// 加载完扩展后,就可以释放掉临时渲染上下文了
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	renderContext = nullptr;

	// 释放设备环境
	ReleaseDC(hWnd, deviceContext);
	deviceContext = nullptr;
	return true;
}





bool InitOpenGL(HWND hWnd)
{



	HDC deviceContext = GetDC(hWnd);

	if (!deviceContext) {
		std::cout << "GetDC failed.\n";
		return false;
	}

	int attrib[19];
	int pixelFormat[1];
	unsigned int formatCount;

	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDesc;

	int attributeList[5];

	

	attrib[0] = WGL_SUPPORT_OPENGL_ARB;
	attrib[1] = TRUE;

	attrib[2] = WGL_DRAW_TO_WINDOW_ARB;
	attrib[3] = TRUE;

	attrib[4] = WGL_ACCELERATION_ARB;
	attrib[5] = WGL_FULL_ACCELERATION_ARB;

	// Support for 24bit color.
	attrib[6] = WGL_COLOR_BITS_ARB;
	attrib[7] = 24;

	// Support for 24 bit depth buffer.
	attrib[8] = WGL_DEPTH_BITS_ARB;
	attrib[9] = 24;

	// Support for double buffer.
	attrib[10] = WGL_DOUBLE_BUFFER_ARB;
	attrib[11] = TRUE;

	// Support for swapping front and back buffer.
	attrib[12] = WGL_SWAP_METHOD_ARB;
	attrib[13] = WGL_SWAP_EXCHANGE_ARB;

	// Support for the RGBA pixel type.
	attrib[14] = WGL_PIXEL_TYPE_ARB;
	attrib[15] = WGL_TYPE_RGBA_ARB;

	// Support for a 8 bit stencil buffer.
	attrib[16] = WGL_STENCIL_BITS_ARB;
	attrib[17] = 8;

	// Null terminate the attribute list.
	attrib[18] = 0;

	// Query for a pixel format that fits the attributes we want.
	result = wglChoosePixelFormatARB(deviceContext, attrib, NULL, 1, pixelFormat, &formatCount);
	if (result!=1)
	{
		std::cout << "Choose pixel format failed." << std::endl;
		return false;
	}
	result = SetPixelFormat(deviceContext, pixelFormat[0], &pixelFormatDesc);
	if (result != 1)return false;

	// Set the 4.0 version of OpenGL in the attribute list.
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;
	// Null terminate the attribute list.
	attributeList[4] = 0;

	// Create a OpenGL 4.0 rendering context.
	HGLRC renderingContext = wglCreateContextAttribsARB(deviceContext, 0, attributeList);
	if (!renderingContext) {
		return false;
	}
	result = wglMakeCurrent(deviceContext, renderingContext);
	if (result != 1)
	{
		return false;
	}

	std::string vendorString = (char*)glGetString(GL_VENDOR);
	std::string rendererString = (char*)glGetString(GL_RENDERER);

	std::cout<<vendorString.c_str()<<"\n"<<rendererString.c_str()<< std::endl;

	gDeviceContext = deviceContext;
	return true;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		gQuit = true;
		return 0;
	}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void render()
{
	
}