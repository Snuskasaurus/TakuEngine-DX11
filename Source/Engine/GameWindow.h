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


class MGameWindow
{
public:
    static void InitializeGameWindow(const HINSTANCE hInstance);
    static void UninitializeGameWindow();
    static void DrawGameWindow();
    static SExitResult HandleGameWindowMessage();
public:
    static UINT GetGameWindowHeight();
    static UINT GetGameWindowWidth();
    static HWND GetWindowHandle();
    static bool HasFocus();
    static TMatrix4f GetPerspectiveMatrix();
};

