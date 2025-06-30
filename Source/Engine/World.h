#pragma once

#include "Math.h"
#include "Camera/DebugCamera.h"
#include "Inputmanager.h"
#include "Scene.h"
#include "Camera/GameCamera.h"

struct alignas(16) SWorldLight
{
    TVector3f Direction = { 0.37f, 0.93f, 0.0 };
    float Ambient = 0.0f;
};

class MWorld
{
    friend class CGameScene;
    friend class MGraphic;
    friend class MInput;
    friend struct SShaderBufferHolder;
    
private:
    static MWorld* Instance;
    
public:
    static MWorld* GetWorld() { return Instance; }
    static void InitializeWorld();
    static void UninitializeWorld();
    static void NotifyKeyPressedToGameScene(EKeyCode _keyCode);
    static void NotifyKeyReleasedToGameScene(EKeyCode _keyCode);
    static void UpdateWorld(const float& _dt);
public:
    CGameScene* GetCurrentScene() const { return CurrentGameScene; }

    static TVector3f GetCameraPosition();
    static TVector3f GetCameraForward();
    static TMatrix4f GetCameraWorldMatrix();
    static TMatrix4f GetViewMatrix();
    static TMatrix4f GetProjectionMatrix();

private:
    void BeginDraw();
    void DrawDebugMatrix();
    
private:
    void OnInitialize();
    void OnUninitialize();
    void OnUpdate(const float& _dt);
    
private:
    CGameScene* CurrentGameScene = nullptr;
};
