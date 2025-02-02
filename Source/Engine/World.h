#pragma once

#include "Math.h"
#include "FreeLookCamera.h"

__declspec(align(16)) struct SWorldLight
{
    TVector3f Direction = { 0.37f, 0.93f, 0.0 };
    float Ambient = 0.0f;
};

class MWorld
{
private:
    static MWorld* Instance;
    
public:
    static void InitializeWorld();
    static MWorld* GetWorld() { return Instance; }
    static void DoWorldUpdate(const float& _dt) { Instance->OnUpdate(_dt); }
    
public:
    TVector3f GetSunDirection() const { return SunDirection; }
    TMatrix4f GetInverseCameraMatrix() const { return FreeLookCamera.GetCameraWorldInverseMatrix(); }
    
protected:
    void OnInit();
    void OnUpdate(const float& _dt);
    
private:
    TVector3f SunDirection = TVector3f::Zero;
    
private:
    TFreeLookCamera FreeLookCamera;
};
