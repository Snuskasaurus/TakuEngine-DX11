#include "World.h"

#include "IncludesExternal.h"

#include "Graphic.h"

///---------------------------------------------------------------------------------------------------------------------
MWorld* MWorld::Instance = nullptr;
///---------------------------------------------------------------------------------------------------------------------
void MWorld::InitializeWorld()
{
    assert(Instance == nullptr);
    Instance = new MWorld;
    Instance->OnInit();
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::OnInit()
{
    FreeLookCamera.SetPosition(TVector3f(0.0f, -10.0f, 6.0f));
    FreeLookCamera.SetRotation(TRotator(0.0f, -0.5f, 0.0f));
    SunDirection = { 0.05f, -0.1f, 0.25f };
    
    CGameScene::ChangeGameScene(EGameSceneType::GridScene);
}
///---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUpdate(const float& _dt)
{
    FreeLookCamera.UpdateCamera(_dt);
    if (CurrentGameScene != nullptr)
    {
        CurrentGameScene->Update(_dt);
    }
}
///---------------------------------------------------------------------------------------------------------------------
