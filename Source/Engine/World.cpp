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
    Instance->FreeLookCamera.UpdateCamera(_dt);
    
    if (Instance->CurrentGameScene != nullptr)
    {
        Instance->CurrentGameScene->UpdateScene(_dt);
    }
}
//---------------------------------------------------------------------------------------------------------------------
TMatrix4f MWorld::GetCameraWorldMatrix()
{
    return Instance->FreeLookCamera.GetCameraWorldMatrix();
}
//---------------------------------------------------------------------------------------------------------------------
TVector3f MWorld::GetCameraPosition()
{
    return Instance->FreeLookCamera.GetCameraPosition();
}
//---------------------------------------------------------------------------------------------------------------------
TVector3f MWorld::GetCameraForward()
{
    return -Instance->FreeLookCamera.GetCameraWorldViewDir();
}
//---------------------------------------------------------------------------------------------------------------------
TMatrix4f MWorld::GetViewMatrix()
{
    return Instance->FreeLookCamera.GetCameraViewMatrix();
}
//---------------------------------------------------------------------------------------------------------------------
TMatrix4f MWorld::GetProjectionMatrix()
{
    // TODO Julien Rogel (02/02/2025): no need to compute it each time we call it but only when ScreenRatio change
    const float ScreenRatio = (float)MGameWindow::GetGameWindowWidth() / (float)MGameWindow::GetGameWindowHeight();
    const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixPerspectiveFov(0.4f * 3.14f, ScreenRatio, 0.1f, 300.0f);
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
    FreeLookCamera.InitializeCamera();
    FreeLookCamera.SetCameraPosition(TVector3f(18.0f, 22.0f, 20.0f));
    FreeLookCamera.SetCameraRotation(TRotator(-0.52f, 2.44f, 0.0f));
    
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
