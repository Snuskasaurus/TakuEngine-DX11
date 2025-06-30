#pragma once

#include "Engine/Math.h"

class AGameCamera
{
public:
    void InitializeCameraGame();
    void UpdateCamera(const float _dt);
public:
    TVector3f GetCameraWorldViewDir() const { return Transform.Forward(); }
    TMatrix4f GetCameraWorldMatrix() const { return Transform.WorldMatrix(); }
    TMatrix4f GetCameraViewMatrix() const { return TMatrix4f::MatrixLookTo(Transform.Position, Transform.Forward(), Transform.Up()); }
    TVector3f GetCameraPosition() const { return  Transform.Position; }
    TTransform GetCameraTransform() const { return  Transform; }
    void SetCameraPosition(TVector3f _newPosition) {  Transform.Position = _newPosition; }
    void SetCameraRotation(TRotator newRotator) {  Transform.Rotator = newRotator; }
private:
    TTransform Transform;

    float MovementSpeed = 30.0f;
};
