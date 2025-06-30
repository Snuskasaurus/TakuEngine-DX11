#pragma once

#include "Engine/Math.h"

class AGameCamera
{
public:
    void InitializeCameraGame();
    void UpdateCamera(const float _dt);
    void DrawGameCameraWindow();

public:
    TVector3f GetCameraWorldViewDir() const { return Transform.Forward(); }
    TMatrix4f GetCameraWorldMatrix() const { return Transform.WorldMatrix(); }
    TMatrix4f GetCameraViewMatrix() const { return TMatrix4f::MatrixLookTo(Transform.Position, Transform.Forward(), Transform.Up()); }
    TVector3f GetCameraPosition() const { return  Transform.Position; }
    TTransform GetCameraTransform() const { return  Transform; }
    void SetCameraPosition(TVector3f _newPosition) {  Transform.Position = _newPosition; }
    void SetCameraRotation(TRotator newRotator) {  Transform.Rotator = newRotator; }

private:
    void SetupPitchAndHeightFromZoomValue();
    
private:
    TTransform Transform;

private: // Settings
    float ZoomSpeed = 0.1f;
    float MovementSpeed = 55.0f;
    float YawSpeed = 1.2f;

    float MinPitch = -0.80f; 
    float MinHeight = 62.0f; 
    float MaxPitch = -1.21f;
    float MaxHeight = 200.0f;
    
private: // Internals
    float TargetZoomValue = 0.5f;
    float CurrentZoomValue = 0.5f;
};
