#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <iostream>

// for dx9
#include <d3d9.h>

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT InitDX9(HWND hWnd);


bool gQuit = false;
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

	ShowWindow(hWnd, SW_SHOW);

	InitDX9(hWnd);

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
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 创建DX9环境
HRESULT InitDX9(HWND hWnd)
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;

	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp,&pDevice);

	if (hr == S_OK)
	{
		std::cout << "Init DX9 Device Sucessed!" << std::endl;
	}
	else {
		std::cout << "Init DX9 Device Failed!" << std::endl;
	}

	return hr;
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