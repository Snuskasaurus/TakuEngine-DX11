#include "World.h"

#include <format>

#include "GameWindow.h"
#include "IncludesExternal.h"
#include "Debug/DebugDraw.h"
#include "Debug/DebugGUIManager.h"

#include "Graphics/Graphic.h"

///---------------------------------------------------------------------------------------------------------------------
MWorld* MWorld::Instance = nullptr;
///---------------------------------------------------------------------------------------------------------------------
void MWorld::InitializeWorld()
{
    assert(Instance == nullptr);
    Instance = new MWorld;
    Instance->OnInitialize();

    Instance->BeginDraw();
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::UninitializeWorld()
{
    assert(Instance != nullptr);
    Instance->OnUninitialize();
    delete Instance;
    Instance = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::NotifyKeyPressedToGameScene(EKeyCode _keyCode)
{
    Instance->CurrentGameScene->OnKeyPressed_Internal(_keyCode);
    Instance->CurrentGameScene->OnEvent_KeyPressed(_keyCode);
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::NotifyKeyReleasedToGameScene(EKeyCode _keyCode)
{
    Instance->CurrentGameScene->OnKeyReleased_Internal(_keyCode);
    Instance->CurrentGameScene->OnEvent_KeyReleased(_keyCode);
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::UpdateWorld(const float& _dt)
{
    if (Instance->CurrentGameScene != nullptr)
    {
        Instance->CurrentGameScene->UpdateScene(_dt);
    }
}
//---------------------------------------------------------------------------------------------------------------------
TMatrix4f MWorld::GetCameraWorldMatrix()
{
    if (Instance->CurrentGameScene->DebugModeEnabled == false)
        return Instance->CurrentGameScene->GameCamera.GetCameraWorldMatrix();
    
    return Instance->CurrentGameScene->DebugCamera.GetCameraWorldMatrix();
}
//---------------------------------------------------------------------------------------------------------------------
TVector3f MWorld::GetCameraPosition()
{
    if (Instance->CurrentGameScene->DebugModeEnabled == false)
        return Instance->CurrentGameScene->GameCamera.GetCameraPosition();
    
    return Instance->CurrentGameScene->DebugCamera.GetCameraPosition();
}
//---------------------------------------------------------------------------------------------------------------------
TVector3f MWorld::GetCameraForward()
{
    if (Instance->CurrentGameScene->DebugModeEnabled == false)
        return Instance->CurrentGameScene->GameCamera.GetCameraWorldViewDir();
    
    return Instance->CurrentGameScene->DebugCamera.GetCameraWorldViewDir();
}
//---------------------------------------------------------------------------------------------------------------------
TMatrix4f MWorld::GetViewMatrix()
{
    if (Instance->CurrentGameScene->DebugModeEnabled == false)
        return Instance->CurrentGameScene->GameCamera.GetCameraViewMatrix();
    
    return Instance->CurrentGameScene->DebugCamera.GetCameraViewMatrix();
}
//---------------------------------------------------------------------------------------------------------------------
TMatrix4f MWorld::GetProjectionMatrix()
{
    // TODO Julien Rogel (02/02/2025): no need to compute it each time we call it but only when ScreenRatio change
    const float ScreenRatio = (float)MGameWindow::GetGameWindowWidth() / (float)MGameWindow::GetGameWindowHeight();
    constexpr float foAngleY = 0.4f * 3.14f;
    const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixPerspectiveFov(foAngleY, ScreenRatio, 0.1f, 300.0f);
    
    return PerspectiveMatrix;
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::BeginDraw()
{
    MDebugGUI::AddWindow("WorldDebug", [this]() { this->DrawDebugMatrix(); });
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::DrawDebugMatrix()
{
    auto debugs = MDebugDraw::GetDebugLines();

    ImGui::Text("Nb Debug Lines = %i", debugs.size());
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::OnInitialize()
{
    CGameScene::ChangeGameScene(GC_DEFAULT_SCENE);
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUninitialize()
{
    if (Instance->CurrentGameScene != nullptr)
    {
        CurrentGameScene->Destroy();
        delete CurrentGameScene;
        CurrentGameScene = nullptr;
    }
}
///---------------------------------------------------------------------------------------------------------------------
