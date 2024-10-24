#include "GameWindow.h"
#include <Windows.h>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------------------------------------------------
const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int DefaultPositionX = 320;
constexpr int DefaultPositionY = 150;
constexpr int DefaultSizeX = 1280;
constexpr int DefaultSizeY = 720;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND InternalHwnd;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK GameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        {
            PostQuitMessage(1);
        }
        break;
    case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(1);
            }
        }
        break;
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);	
        }
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::CreateGameWindow(const HINSTANCE hInstance)
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
    wc.lpszClassName = GameClassName;
    RegisterClassEx(&wc);
	
    InternalHwnd = CreateWindowEx(0, GameClassName, WindowName, DefaultDword,
        DefaultPositionX, DefaultPositionY, DefaultSizeX, DefaultSizeY,
        nullptr, nullptr, hInstance, nullptr);
    
    ShowWindow(InternalHwnd, SW_SHOW);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::DestroyGameWindow()
{
    assert(InternalHwnd != nullptr);
    DestroyWindow(InternalHwnd);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
TExitReason JuProject::HandleGameWindowMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return { true, (int)msg.wParam };
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {false, -1 };
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
