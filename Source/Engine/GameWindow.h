#pragma once

#include <Windows.h>

namespace JuProject
{
    struct SExitResult
    {
        bool Exiting = false;
        int Reason = -1;
    };

    HWND GetGameWindow();
    
    void CreateGameWindow(const HINSTANCE hInstance);
    void DestroyGameWindow();
    SExitResult HandleGameWindowMessage();

    void DoFrame(const float dt);
}

