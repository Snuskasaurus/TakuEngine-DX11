#include "World.h"

#include <assert.h>

#include "../Engine/AssetList.h"
#include "../Engine/Graphic.h"

CMesh* Mesh = nullptr;
CInstancedMesh* InstancedMesh = nullptr;
    
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
    
    Mesh = MGraphic::AddMeshToDraw({}, JU_ASSET_TAKUMI);
    
    InstancedMesh = MGraphic::AddInstancedMeshToDraw({}, JU_ASSET_TAKUMI);
    const TVector3f MinPosition = TVector3f(-100.0f, -100.00f, -100.0f);
    const TVector3f MaxPosition = TVector3f(100.0f, 100.00f, 100.0f);
    for (int i = 1; i < 1024; ++i)
    {
        const float Yaw = MMath::RandomNumberIntegerInRange(-10.0f, 10.0f);
        TTransform NewTransform = { MMath::RandomVectorIntegerInRange(MinPosition, MaxPosition), Yaw, 0.0f, 0.0f };
        InstancedMesh->Transforms.push_back(NewTransform);
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUpdate(const float& _dt)
{
    FreeLookCamera.UpdateCamera(_dt);

    for (int i = 0; i < 1024; ++i)
    {
        InstancedMesh->Transforms[i].Rotator.Yaw += (i % 8 + 1) * _dt;
        InstancedMesh->Transforms[i].Rotator.Pitch += (i % 8 + 1) * _dt;
        InstancedMesh->Transforms[i].Rotator.Roll += (i % 8 + 1) * _dt;
    }
    
}
///---------------------------------------------------------------------------------------------------------------------
