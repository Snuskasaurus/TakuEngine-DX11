#pragma once

#include <Windows.h>

struct TExitReason
{
    bool Exist = false;
    int Reason = -1;
};

namespace JuProject
{
    void CreateGameWindow(const HINSTANCE hInstance);
    void DestroyGameWindow();
    TExitReason HandleGameWindowMessage();
}

