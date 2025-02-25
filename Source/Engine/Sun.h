#pragma once
#include "Color.h"
#include "Math.h"

struct SLightInfo
{
    TVector3f Direction = TVector3f::Zero;
    TColorf Diffuse = TColorf::White;
    float Ambient = 0.0f;
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
private:
    void Initialize();
    void Uninitialize();
    void UpdateOnChanges();
private:
    float CurrentYaw = 0.0f;
    float CurrentPitch = 0.0f;
    class CDrawable_InstancedMesh* DebugArrowMesh = nullptr;
    TColorf DiffuseColor = { 1.0f, 0.992156f, 0.815686f, 1.0f };
    float Ambient = 0.26f;
};
