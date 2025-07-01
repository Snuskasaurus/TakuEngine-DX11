#include "DebugGUIManager.h"

#include "../GameWindow.h"
#include "../Graphics/Graphic.h"

#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx11.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
///---------------------------------------------------------------------------------------------------------------------
MDebugGUI* MDebugGUI::Instance = nullptr;
///---------------------------------------------------------------------------------------------------------------------
void MDebugGUI::InitializeDebugGUI()
{
    assert(Instance == nullptr);
    Instance = new MDebugGUI;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(MGameWindow::GetWindowHandle());
    ImGui_ImplDX11_Init(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext());
}
///---------------------------------------------------------------------------------------------------------------------
void MDebugGUI::StartFrameDebugGUI()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();

    for (int i = 0; i < Instance->WindowDrawFunctions.size(); ++i)
    {
        ImGui::Begin(Instance->WindowDrawFunctions[i].Name.c_str());
        Instance->WindowDrawFunctions[i].Function();
        ImGui::End();
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MDebugGUI::EndFrameDebugGUI()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
///---------------------------------------------------------------------------------------------------------------------
void MDebugGUI::UninitializeDebugGUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    assert(Instance != nullptr);
    delete Instance;
    Instance = nullptr;
}
///---------------------------------------------------------------------------------------------------------------------
bool MDebugGUI::DispatchWinProcDebugGUI(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}
///---------------------------------------------------------------------------------------------------------------------
