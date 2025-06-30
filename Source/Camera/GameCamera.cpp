#include "GameCamera.h"
#include "Engine/Inputmanager.h"

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

inline float GetCameraInputUp()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_SHIFT))
        return 1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_CTRL))
        return -1.0f;

    return 0.0f;
}
///---------------------------------------------------------------------------------------------------------------------
void AGameCamera::InitializeCameraGame()
{
    
}
///---------------------------------------------------------------------------------------------------------------------
void AGameCamera::UpdateCamera(const float _dt)
{
    // Inputs ----------------------------------------------------------------------------------------------------------

    const float changeForward = GetCameraInputForward() * MovementSpeed * _dt;
    const float changeRight = GetCameraInputRight() * MovementSpeed * _dt;
    
    // Update Transform ------------------------------------------------------------------------------------
    
    const TVector3f cameraForward = TVector3f::Forward;
    const TVector3f cameraRight = TVector3f::Right;
    const TVector3f positionChange = (cameraRight * changeRight) + (cameraForward * changeForward);

    Transform.Position += positionChange;
}
///---------------------------------------------------------------------------------------------------------------------