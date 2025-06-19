#include "FreeLookCamera.h"

#include <Windows.h>

#include "IncludesExternal.h"
#include "Inputmanager.h"

float GetCameraInputForward()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_W))
       return 1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_S))
        return -1.0f;

    return 0.0f;
}

float GetCameraInputRight()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_D))
        return 1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_A))
        return -1.0f;

    return 0.0f;
}

float GetCameraInputUp()
{
    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_SHIFT))
        return 1.0f;

    if (MInput::IsKeyDown(EKeyCode::KEY_LEFT_CTRL))
        return -1.0f;

    return 0.0f;
}

void TFreeLookCamera::UpdateCamera(const float dt)
{
    // Inputs ----------------------------------------------------------------------------------------------------------

    SpeedMovement += 1.0f * AddedSpeedMovementByCameraSpeedModifier * dt;
    //SpeedMovement += GET_INPUT(CameraSpeedModifier) * AddedSpeedMovementByCameraSpeedModifier * dt;
    if (SpeedMovement <= 0.0f)
        SpeedMovement = 0.0f;

    const float SpeedForward = GetCameraInputForward() * SpeedMovement * dt;
    const float SpeedRight = GetCameraInputRight() * SpeedMovement * dt;
    const float SpeedUp = GetCameraInputUp() * SpeedMovement * dt;
    
    const float InputYaw = GET_INPUT(CameraYaw) * SpeedRotation * dt;
    const float InputPitch = GET_INPUT(CameraPitch) * SpeedRotation * dt;

    // Update Yaw and Pitch --------------------------------------------------------------------------------------------
    
    CamYaw += InputYaw;
    CamPitch = MMath::Clamp(CamPitch + InputPitch, -1.55334f, 1.55334f);
    
    // Position / Rotation / Matrix ------------------------------------------------------------------------------------
  
    const TMatrix4f CamRotationMatrix = TMatrix4f::MatrixRotationPitch(CamPitch) * TMatrix4f::MatrixRotationYaw(CamYaw) * TMatrix4f::MatrixRotationRoll(0.0f);
    
    CamForward = TVector3f::TransformCoord(TVector3f::Forward, CamRotationMatrix);
    CamForward = TVector3f::Normalize(CamForward);

    CamRight = TVector3f::TransformCoord(TVector3f::Right, CamRotationMatrix);
    CamRight = TVector3f::Normalize(CamRight);

    CamUp = TVector3f::Cross(CamForward, CamRight);
    CamUp = TVector3f::Normalize(CamUp);

    Position += CamRight * SpeedRight;
    Position += CamForward * SpeedForward;
    Position += TVector3f::Up * SpeedUp;
    
    const TMatrix4f CamPositionMatrix = TMatrix4f::MatrixTranslation(Position);
    CamMatrix = CamRotationMatrix * CamPositionMatrix;

    // Logs ------------------------------------------------------------------------------------------------------------
    
    //OutputDebugStringA(("dt=" + std::to_string(dt) + "\n").c_str());
    
#define PRINT_VEC(VecName, Vec) OutputDebugStringA((VecName " = [" + std::to_string(Vec.x) + "|" + std::to_string(Vec.y) + "|" + std::to_string(Vec.z) + "]\n").c_str());

    // OutputDebugStringA("---------------------------\n");
    // PRINT_VEC("Camera Position", Position)
    // PRINT_VEC("Camera FORWARD", CamForward)
    // PRINT_VEC("Camera RIGHT", CamRight)
    // PRINT_VEC("Camera UP", CamUp)
    
#undef PRINT_VEC
}
