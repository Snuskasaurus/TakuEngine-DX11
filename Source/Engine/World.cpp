#include "World.h"

#include <assert.h>

#include "AssetList.h"
#include "Graphic.h"

CMesh* TakumiMesh = nullptr;
    
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
    SunDirection = { 0.37f, -0.63f, 0.0 };
    TakumiMesh = MGraphic::AddMeshToDraw({}, JU_ASSET_TAKUMI);
}
///---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUpdate(const float& _dt)
{
    FreeLookCamera.UpdateCamera(_dt);

    TakumiMesh->Transform.Rotator.Yaw += 1.0f * _dt;
}
///---------------------------------------------------------------------------------------------------------------------
