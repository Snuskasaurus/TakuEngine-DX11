#pragma once
#include <array>
#include <functional>
#include <vector>
#include <string>
#include <windows.h>
#include "../../imgui/imgui.h"

using ImGuiWindowFunc = std::function<void()>;
struct TWindowFunctionHolder
{
    std::string Name;
    ImGuiWindowFunc Function;
};

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

    static void AddWindow(const std::string& _name, const ImGuiWindowFunc& _windowFunction)
    {
        assert(Instance != nullptr);
        TWindowFunctionHolder NewWindowFunctionHolder;
        NewWindowFunctionHolder.Name = _name;
        NewWindowFunctionHolder.Function = _windowFunction;
        Instance->WindowDrawFunctions.push_back(NewWindowFunctionHolder);
    }

    static void RemoveWindow(const std::string& _name)
    {
        for (int i = 0; i < Instance->WindowDrawFunctions.size(); ++i)
        {
            if (Instance->WindowDrawFunctions[i].Name == _name) // TODO Julien Rogel (24/06/2025): Search with index instead of strings
            {
                Instance->WindowDrawFunctions.erase(Instance->WindowDrawFunctions.begin() + i);
                return;
            }
        }
    }
    
private:
    std::vector<TWindowFunctionHolder> WindowDrawFunctions;
};
