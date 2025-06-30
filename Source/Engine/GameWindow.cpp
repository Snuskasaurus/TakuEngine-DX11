#include "GameWindow.h"

#include <windows.h>
#include <Windowsx.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include "Inputmanager.h"
#include "Math.h"
#include "Debug/DebugGUIManager.h"
#include "Graphics/Graphic.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindowHandle;
const CHAR* GameClassName = "TakuEngine";
const CHAR* WindowName = "TakuEngine";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr UINT DefaultWindowSizeX = (UINT)(1440);
constexpr UINT DefaultWindowSizeY = (UINT)(810);
constexpr UINT DefaultWindowPositionX = (UINT)(DefaultWindowSizeX * 0.1);
constexpr UINT DefaultWindowPositionY = (UINT)(DefaultWindowSizeY * 0.1);
UINT WindowSizeX = DefaultWindowSizeX;
UINT WindowSizeY = DefaultWindowSizeY;
bool HasWindowFocus = false;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK GameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MDebugGUI::DispatchWinProcDebugGUI(hWnd, msg, wParam, lParam);
    
    switch (msg)
    {
    case WM_KEYDOWN:
        {
            MInput::HandleKeyDown(wParam);
        } break;
    case WM_KEYUP:
        {
            MInput::HandleKeyUp(wParam);
        } break;
    case WM_MOUSEWHEEL:
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            MInput::UpdateMouseWheelInput((float)zDelta);
        } break;
    case WM_SIZE:
        {
            WindowSizeX = LOWORD(lParam);
            WindowSizeY = HIWORD(lParam);
        } break;
    case WM_DESTROY:
        {
            PostQuitMessage(1);
        } break;
    case WM_SETFOCUS:
        {
            HasWindowFocus = true;
        } break ;
    case WM_KILLFOCUS:
        {
            HasWindowFocus = false;
        } break ;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void MGameWindow::InitializeGameWindow(const HINSTANCE hInstance)
{
    WNDCLASSEX windowInfos = { 0 };
    {
        windowInfos.cbSize = sizeof(windowInfos);
        windowInfos.style = CS_OWNDC;
        windowInfos.lpfnWndProc = GameWindowProcedure;
        windowInfos.cbClsExtra = 0;
        windowInfos.cbWndExtra = 0;
        windowInfos.hInstance = hInstance;
        windowInfos.hCursor = nullptr;
        windowInfos.hIcon = nullptr;
        windowInfos.hbrBackground = nullptr;
        windowInfos.lpszMenuName = nullptr;
        windowInfos.hIconSm = nullptr;
        windowInfos.lpszClassName = GameClassName;
    }
    
    RegisterClassEx(&windowInfos);
	
    GameWindowHandle = CreateWindowEx(0, GameClassName, WindowName, DefaultDword,
        DefaultWindowPositionX, DefaultWindowPositionY, DefaultWindowSizeX, DefaultWindowSizeY,
        nullptr, nullptr, hInstance, nullptr);
    
    ShowWindow(GameWindowHandle, SW_SHOW);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void MGameWindow::UninitializeGameWindow()
{
    assert(GameWindowHandle != nullptr);
    DestroyWindow(GameWindowHandle);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void MGameWindow::DrawGameWindow()
{
    if (MGameWindow::HasFocus() == false)
        return;

    MGraphic::RenderFrame();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
SExitResult MGameWindow::HandleGameWindowMessage()
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
UINT MGameWindow::GetGameWindowHeight() { return WindowSizeY; }
UINT MGameWindow::GetGameWindowWidth() {  return WindowSizeX; }
HWND MGameWindow::GetWindowHandle() { return GameWindowHandle; }
bool MGameWindow::HasFocus() { return HasWindowFocus; }
//---------------------------------------------------------------------------------------------------------------------------------------------------------
