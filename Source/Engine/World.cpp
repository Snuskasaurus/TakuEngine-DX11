#include "World.h"

#include "IncludesExternal.h"

#include "Graphics/Graphic.h"

///---------------------------------------------------------------------------------------------------------------------
MWorld* MWorld::Instance = nullptr;
///---------------------------------------------------------------------------------------------------------------------
void MWorld::InitializeWorld()
{
    assert(Instance == nullptr);
    Instance = new MWorld;
    Instance->OnInitialize();
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::UninitializeWorld()
{
    assert(Instance != nullptr);
    Instance->OnUninitialize();
    delete Instance;
    Instance = nullptr;
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::OnInitialize()
{
    FreeLookCamera.SetPosition(TVector3f(0.0f, -80.0f, 30.0f));
    FreeLookCamera.SetRotation(TRotator(0.0f, -0.5f, 0.0f));
    
    CGameScene::ChangeGameScene(EGameSceneType::GridScene);
}
//---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUninitialize()
{
    if (Instance->CurrentGameScene != nullptr)
    {
        CurrentGameScene->Destroy();
        delete CurrentGameScene;
        CurrentGameScene = nullptr;
    }
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
