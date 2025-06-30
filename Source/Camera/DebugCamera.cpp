#include "DebugCamera.h"
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
inline float GetCameraInputUp()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_SHIFT))
        return 1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_CTRL))
        return -1.0f;

    return 0.0f;
}
///---------------------------------------------------------------------------------------------------------------------
void ADebugCamera::InitializeCameraDebug()
{
    MDebugGUI::AddWindow("DebugGamera", [this]() { this->DrawCameraDebugWindow(); });
}
///---------------------------------------------------------------------------------------------------------------------
void ADebugCamera::UpdateCamera(const float _dt)
{
    // Inputs ----------------------------------------------------------------------------------------------------------

    // TODO Julien Rogel (25/06/2025): Rework this when adding mouse wheel events
    MovementSensibility = MovementSensibility + (0.0f * AddedSpeedMovementByCameraSpeedModifier * _dt);
    if (MovementSensibility <= 0.0f)
        MovementSensibility = 0.0f;

    const float changeForward = GetCameraInputForward() * MovementSensibility * _dt;
    const float changeRight = GetCameraInputRight() * MovementSensibility * _dt;
    const float changeUp = GetCameraInputUp() * MovementSensibility * _dt;

    float changeYaw = 0.0f;
    float changePitch = 0.0f;
    if (MInput::IsCursorLocked())
    {
        changeYaw = -MInput::GetMouseMovement().x * SpeedRotation * _dt;
        changePitch = MInput::GetMouseMovement().y * SpeedRotation * _dt;
    }

    ChangesFromInputs.x = changeRight;
    ChangesFromInputs.y = changeUp;
    ChangesFromInputs.z = changeForward;
    
    // Update Transform ------------------------------------------------------------------------------------

    Transform.Rotator.Yaw += changeYaw;
    Transform.Rotator.Pitch = MMath::Clamp(Transform.Rotator.Pitch + changePitch, -1.55334f, 1.55334f);
    
    const TVector3f cameraForward = Transform.Forward();
    const TVector3f cameraRight = Transform.Right();
    const TVector3f positionChange = (cameraRight * changeRight) + (cameraForward * changeForward) + (TVector3f::Up * changeUp);

    Transform.Position += positionChange;
}
///---------------------------------------------------------------------------------------------------------------------
void ADebugCamera::DrawCameraDebugWindow()
{
    const std::string textPosition = std::format("World Position: x:{:.2f} y:{:.2f} z:{:.2f}", Transform.Position.x, Transform.Position.y, Transform.Position.z);
    ImGui::Text(textPosition.c_str());

    const std::string textRotation = std::format("World Rotation: pitch:{:.2f} yaw:{:.2f} roll:{:.2f}", Transform.Rotator.Pitch, Transform.Rotator.Yaw, Transform.Rotator.Roll);
    ImGui::Text(textRotation.c_str());
    
    const std::string textInputs = std::format("Inputs: x:{:.2f} y:{:.2f} z:{:.2f}", ChangesFromInputs.x, ChangesFromInputs.y, ChangesFromInputs.z);
    ImGui::Text(textInputs.c_str());
    
    const TVector3f cameraRight = Transform.Right();
    const std::string textRight = std::format("Right: x:{:.2f} y:{:.2f} z:{:.2f}", cameraRight.x, cameraRight.y, cameraRight.z);
    ImGui::Text(textRight.c_str());

    const TVector3f cameraForward = Transform.Forward();
    const std::string textForward = std::format("Forward: x:{:.2f} y:{:.2f} z:{:.2f}", cameraForward.x, cameraForward.y, cameraForward.z);
    ImGui::Text(textForward.c_str());
    
    const TVector3f cameraUp = Transform.Up();
    const std::string textUp = std::format("Up: x:{:.2f} y:{:.2f} z:{:.2f}", cameraUp.x, cameraUp.y, cameraUp.z);
    ImGui::Text(textUp.c_str());
    
    const std::string textCameraSpeed = std::format("Movement Sensibility: x:{:.2f}", MovementSensibility);
    ImGui::Text(textCameraSpeed.c_str());
}
///---------------------------------------------------------------------------------------------------------------------