#include "GameWindow.h"

#include <Windows.h>
#include <Windowsx.h>
#include <wincodec.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgiformat.h>

#include "Math.h"
#include "HResultHandler.h"
#include "GraphicPipeline.h"
#include "MeshManager.h"

#define GAME_DATA_PATH "Game/Data/"
#define GAME_DATA_SHADER_PATH "Game/Data/Shaders/"

#define MESH_TO_IMPORT_SQUARE "Square"
#define MESH_TO_IMPORT_CUBE "Cube"
#define MESH_TO_IMPORT_SPHERE "Sphere"
#define MESH_TO_IMPORT_SUZANNE "Suzanne"
#define MESH_TO_IMPORT_CRATE "Crate"
#define MESH_TO_IMPORT_MONSTER "Monster"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Window
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindowHandle;
const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr UINT DefaultWindowSizeX = (UINT)(1920 * 0.85);
constexpr UINT DefaultWindowSizeY = (UINT)(1080 * 0.85);
constexpr UINT DefaultWindowPositionX = (UINT)(DefaultWindowSizeX * 0.1);
constexpr UINT DefaultWindowPositionY = (UINT)(DefaultWindowSizeY * 0.1);
UINT WindowSizeX = DefaultWindowSizeX;
UINT WindowSizeY = DefaultWindowSizeY;
bool HasWindowFocus = false;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Actors
//---------------------------------------------------------------------------------------------------------------------------------------------------------
CStaticMesh MeshTest1;
CStaticMesh MeshTest2;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// GameWindow Class
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindow::GetWindowHandle() { return GameWindowHandle; }
bool GameWindow::HasFocus() { return HasWindowFocus; }
TMatrix4f GameWindow::GetPerspectiveMatrix()
{
    // TODO Julien Rogel (02/02/2025): no need to compute it each time we call it but only when ScreenRatio change
    const float ScreenRatio = (float)WindowSizeY / (float)WindowSizeX;
    const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixPerspectiveFovRH(0.4f * 3.14f, ScreenRatio, 0.0001f, 1000.0f);
    return PerspectiveMatrix;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void InitializeGraphic()
{
    MGraphic::InitializeGraphic();
    MGraphic::AddMeshToDraw({TVector3f{0.0f, 0.0f, -1.0f}}, MESH_TO_IMPORT_CRATE);
    MGraphic::AddMeshToDraw({}, MESH_TO_IMPORT_MONSTER);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void UninitializeDirectX11()
{
    MGraphic::UninitializeGraphic();
}
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
void CreateGameWindow(const HINSTANCE hInstance)
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
	
    GameWindowHandle = CreateWindowEx(0, GameClassName, WindowName, DefaultDword,
        DefaultWindowPositionX, DefaultWindowPositionY, DefaultWindowSizeX, DefaultWindowSizeY,
        nullptr, nullptr, hInstance, nullptr);
    
    ShowWindow(GameWindowHandle, SW_SHOW);
    
    InitializeGraphic();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void DestroyGameWindow()
{
    assert(GameWindowHandle != nullptr);
    
    UninitializeDirectX11();
    
    DestroyWindow(GameWindowHandle);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
SExitResult HandleGameWindowMessage()
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
void DrawGameWindow()
{
    if (HasWindowFocus == false)
        return;

    MGraphic::DrawGraphic();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
UINT GameWindow::GetGameWindowHeight() { return WindowSizeY; }
UINT GameWindow::GetGameWindowWidth() {  return WindowSizeX; }
//---------------------------------------------------------------------------------------------------------------------------------------------------------
