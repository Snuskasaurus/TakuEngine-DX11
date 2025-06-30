#pragma once

#include "Engine/Math.h"

class ADebugCamera
{
public:
    void InitializeCameraDebug();
    void UpdateCamera(const float dt);
public:
    TVector3f GetCameraWorldViewDir() const { return Transform.Forward(); }
    TMatrix4f GetCameraWorldMatrix() const { return Transform.WorldMatrix(); }
    TMatrix4f GetCameraViewMatrix() const { return TMatrix4f::MatrixLookTo(Transform.Position, Transform.Forward(), Transform.Up()); }
    TVector3f GetCameraPosition() const { return Transform.Position; }
    void SetCameraPosition(TVector3f _newPosition) {  Transform.Position = _newPosition; }
    void SetCameraRotation(TRotator newRotator) {  Transform.Rotator = newRotator; }
private:
    void DrawCameraDebugWindow();
private:
    TTransform Transform;
private:
    float SpeedRotation = 0.04f;
    float MovementSensibility = 30.0f;
    float AddedSpeedMovementByCameraSpeedModifier = 3.0f;
private: // Debugs
    TVector3f ChangesFromInputs;
};