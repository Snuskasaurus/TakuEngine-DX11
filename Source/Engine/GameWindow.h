#pragma once

#include <Windows.h>

namespace JuProject
{
    void CreateGameWindow(const HINSTANCE hInstance);
    void DestroyGameWindow();
    int HandleGameWindowMessage();
}

