#pragma once
#include <windows.h>

class MDebugGUI
{
private:
    static MDebugGUI* Instance;
public:
    static void InitializeDebugGUI();
    static void StartFrameDebugGUI();
    static void EndFrameDebugGUI();
    static void UninitializeDebugGUI();
    static bool DispatchWinProcDebugGUI(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
