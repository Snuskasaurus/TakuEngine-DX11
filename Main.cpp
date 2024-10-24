// https://learn.microsoft.com/fr-fr/windows/win32/api/winbase/nf-winbase-winmain
// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles

#include <Windows.h>

const wchar_t* G_GameClassName = L"JuProject";
const wchar_t* G_WindowName = L"JuProject";
constexpr DWORD G_WindowDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int G_WindowPositionX = 320;
constexpr int G_WindowPositionY = 150;
constexpr int G_WindowSizeX = 1280;
constexpr int G_WindowSizeY = 720;

LRESULT CALLBACK GameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY: PostQuitMessage(1); break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = GameWindowProcedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hIconSm = nullptr;
	wc.lpszClassName = G_GameClassName;
	RegisterClassEx(&wc);
	
	const HWND hwnd = CreateWindowEx(0, G_GameClassName, G_WindowName,
		G_WindowDword,G_WindowPositionX, G_WindowPositionY, G_WindowSizeX,
		G_WindowSizeY, nullptr, nullptr, hInstance, nullptr);
	
	ShowWindow(hwnd, SW_SHOW);
	
	MSG msg;
	BOOL gResult;
	do
	{
		gResult = GetMessage(&msg, nullptr, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} while (gResult > 0);
	return gResult == -1 ? -1 : (int)msg.wParam;
}