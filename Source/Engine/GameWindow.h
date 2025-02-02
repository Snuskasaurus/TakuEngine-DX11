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

void DoFrame(const float dt);

class GameWindow
{
public:
    static HWND GetWindowHandle();
    static bool HasFocus();
    static ID3D11Device* GetDevice();
    static ID3D11DeviceContext* GetImmediateContext();
    static TMatrix4f GetPerspectiveMatrix();
};

