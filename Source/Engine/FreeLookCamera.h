#pragma once

#include "Math.h"

class TFreeLookCamera
{

public:
    void UpdateCamera(const float dt);
    TMatrix4f GetCameraWorldMatrix() const { return CamMatrix; }
    TMatrix4f GetCameraWorldInverseMatrix() const { return TMatrix4f::Inverse(CamMatrix); }
    void SetPosition(TVector3f _newPosition) { Position = _newPosition; }
    void SetRotation(TRotator _newRotation) { CamYaw = _newRotation.Yaw; CamPitch = _newRotation.Pitch; }
    
private:
    TMatrix4f CamMatrix = TMatrix4f::Identity;
    
private: // Transform
    TVector3f CamForward = TVector3f::Forward;
    TVector3f CamRight = TVector3f::Right;
    TVector3f CamUp = TVector3f::Up;
    TVector3f Position = { 0.0f, 0.0f, 0.0f };
    float CamYaw = 0.0f;
    float CamPitch = 0.0f;
private:
    float SpeedRotation = 0.04f;
    float SpeedMovement = 15.0f;
    float AddedSpeedMovementByCameraSpeedModifier = 3.0f;
};