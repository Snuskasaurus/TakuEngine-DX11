#include "GameWindow.h"

#include <Windows.h>
#include <Windowsx.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include "Math.h"
#include "Graphics/Graphic.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindowHandle;
const CHAR* GameClassName = "TakuEngine";
const CHAR* WindowName = "TakuEngine";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr UINT DefaultWindowSizeX = (UINT)(1920 * 0.65);
constexpr UINT DefaultWindowSizeY = (UINT)(1080 * 0.65);
constexpr UINT DefaultWindowPositionX = (UINT)(DefaultWindowSizeX * 0.1);
constexpr UINT DefaultWindowPositionY = (UINT)(DefaultWindowSizeY * 0.1);
UINT WindowSizeX = DefaultWindowSizeX;
UINT WindowSizeY = DefaultWindowSizeY;
bool HasWindowFocus = false;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK GameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        {
            WindowSizeX = HIWORD(lParam);
            WindowSizeY = LOWORD(lParam);
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
    
    MGraphic::InitializeGraphic();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void MGameWindow::UninitializeGameWindow()
{
    assert(GameWindowHandle != nullptr);
    MGraphic::UninitializeGraphic();
    DestroyWindow(GameWindowHandle);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void MGameWindow::DrawGameWindow()
{
    if (MGameWindow::HasFocus() == false)
        return;

    MGraphic::Draw();
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
TMatrix4f MGameWindow::GetPerspectiveMatrix()
{
    // TODO Julien Rogel (02/02/2025): no need to compute it each time we call it but only when ScreenRatio change
    const float ScreenRatio = (float)WindowSizeY / (float)WindowSizeX;
    const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixPerspectiveFovRH(0.4f * 3.14f, ScreenRatio, 0.0001f, 5000.0f);
    return PerspectiveMatrix;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
