#pragma once
#include "Color.h"
#include "Math.h"

struct SLightInfo
{
    TVector3f Direction = TVector3f::Zero;
    TColorf Color = TColorf::White;
    float AmbientIntensity = 0.0f;
    float ColorIntensity = 0.0f;
};

class CSceneLight
{
    friend class MGraphic;
    friend class CGameScene;
public:
    SLightInfo GetSceneLightInfo() const;
    void AddYaw(float _yaw) { SetYaw(CurrentYaw + _yaw); }
    void AddPitch(float _pitch) { SetPitch(CurrentPitch + _pitch); }
    void SetYaw(float _yaw);
    void SetPitch(float _pitch);
public:
    TVector3f GetWorldLightDir() const { return -Forward; }
    TMatrix4f GetViewMatrix() const { return TMatrix4f::Inverse(LightMatrix); }
    TMatrix4f GetProjectionMatrix() const;

private:
    void OnValueChanged();
private:
    float CurrentYaw = 0.0f;
    float CurrentPitch = 0.0f;
    class CDrawable_InstancedMesh* DebugArrowMesh = nullptr;
    TColorf DiffuseColor = { 0.99215686274f, 0.98431372549f, 0.82745098039f, 1.0f };
    float Ambient = 0.15f;
    float Intensity = 0.5f;
    TMatrix4f LightMatrix;
    TVector3f Forward;
};
