#pragma once
#include "Color.h"
#include "Math.h"

struct SLightInfo
{
    TVector3f Direction = TVector3f::Zero;
    
    TColorf LightColor = { 1.0f, 0.969f, 0.839f };
    float LightIntensity = 1.25f;
    
    TColorf ShadowColor = { 0.169f, 0.176f, 0.29f };
    float ShadowIntensity = 0.25f;
};

class CSceneLight
{
    friend class MGraphic;
    friend class CGameScene;
    
public:
    SLightInfo GetSceneLightInfo() const { return LightInfo; }
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
    SLightInfo LightInfo;
    TMatrix4f LightMatrix;
    TVector3f Forward;
};
