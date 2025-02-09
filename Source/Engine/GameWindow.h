#pragma once

#include <Windows.h>
#include "Math.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

struct SExitResult
{
    bool Exiting = false;
    int Reason = -1;
};

void CreateGameWindow(const HINSTANCE hInstance);
void DestroyGameWindow();
SExitResult HandleGameWindowMessage();

void DrawGameWindow();

class GameWindow
{
public:
    static UINT GetGameWindowHeight();
    static UINT GetGameWindowWidth();
    static HWND GetWindowHandle();
    static bool HasFocus();
    static TMatrix4f GetPerspectiveMatrix();
};

