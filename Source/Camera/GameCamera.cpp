#include "GameCamera.h"

#include <format>

#include "Engine/Inputmanager.h"
#include "Engine/Debug/DebugGUIManager.h"

///---------------------------------------------------------------------------------------------------------------------
inline float GetCameraInputForward()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_W))
        return 1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_S))
        return -1.0f;

    return 0.0f;
}
///---------------------------------------------------------------------------------------------------------------------
inline float GetCameraInputRight()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_D))
        return -1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_A))
        return 1.0f;

    return 0.0f;
}
///---------------------------------------------------------------------------------------------------------------------
inline float GetCameraInputYaw()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_Q))
        return -1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_E))
        return 1.0f;

    return 0.0f;
}
///---------------------------------------------------------------------------------------------------------------------
void AGameCamera::InitializeCameraGame()
{
    MDebugGUI::AddWindow("GameCamera", [this]() { this->DrawGameCameraWindow(); });
}
///---------------------------------------------------------------------------------------------------------------------
void AGameCamera::UpdateCamera(const float _dt)
{
    // Update Current and Target Zoom Values ---------------------------------------------------------------------------
    
    TargetZoomValue += MInput::GetMouseWheelDelta() * ZoomSpeed * _dt;
    TargetZoomValue = MMath::Clamp(TargetZoomValue, 0.0f, 1.0f);
    CurrentZoomValue = MMath::MoveTowards(CurrentZoomValue, TargetZoomValue, _dt);
    
    // Inputs ----------------------------------------------------------------------------------------------------------
    
    float MovementSpeed = MMath::Lerp(MinMoveSpeed, MaxMoveSpeed, CurrentZoomValue);
    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_SHIFT))
        MovementSpeed += MoveSpeedBonusFromShift;
    
    const float changeForward = GetCameraInputForward() * MovementSpeed * _dt;
    const float changeRight = GetCameraInputRight() * MovementSpeed * _dt;
    const float changeYaw = GetCameraInputYaw() * YawSpeed * _dt;
    
    // Update Transform ------------------------------------------------------------------------------------
    
    Transform.Rotator.Pitch = 0.0f; // Pitch needs to be 0 so it does not impact forward, and will be overriding later when doing zoom
    Transform.Rotator.Yaw += changeYaw;
    
    const TVector3f cameraForward = Transform.Forward();
    const TVector3f cameraRight = Transform.Right();
    const TVector3f positionChange = (cameraRight * changeRight) + (cameraForward * changeForward);
    Transform.Position += positionChange;
    
    // Update Transform from zoom --------------------------------------------------------------------------
    
    Transform.Rotator.Pitch = MMath::Lerp(MinPitch, MaxPitch, CurrentZoomValue);
    Transform.Position.y = MMath::Lerp(MinHeight, MaxHeight, CurrentZoomValue);
}
///---------------------------------------------------------------------------------------------------------------------
void AGameCamera::DrawGameCameraWindow()
{
    const std::string textPosition = std::format("World Position: x:{:.2f} y:{:.2f} z:{:.2f}", Transform.Position.x, Transform.Position.y, Transform.Position.z);
    ImGui::Text(textPosition.c_str());

    const std::string textRotation = std::format("World Rotation: pitch:{:.2f} yaw:{:.2f} roll:{:.2f}", Transform.Rotator.Pitch, Transform.Rotator.Yaw, Transform.Rotator.Roll);
    ImGui::Text(textRotation.c_str());
    
    const TVector3f cameraRight = Transform.Right();
    const std::string textRight = std::format("Right: x:{:.2f} y:{:.2f} z:{:.2f}", cameraRight.x, cameraRight.y, cameraRight.z);
    ImGui::Text(textRight.c_str());

    const TVector3f cameraForward = Transform.Forward();
    const std::string textForward = std::format("Forward: x:{:.2f} y:{:.2f} z:{:.2f}", cameraForward.x, cameraForward.y, cameraForward.z);
    ImGui::Text(textForward.c_str());
    
    const TVector3f cameraUp = Transform.Up();
    const std::string textUp = std::format("Up: x:{:.2f} y:{:.2f} z:{:.2f}", cameraUp.x, cameraUp.y, cameraUp.z);
    ImGui::Text(textUp.c_str());

    ImGui::Text("Zoom Target:  %.2f", TargetZoomValue);
    ImGui::Text("Zoom Current: %.2f", CurrentZoomValue);
}
///---------------------------------------------------------------------------------------------------------------------