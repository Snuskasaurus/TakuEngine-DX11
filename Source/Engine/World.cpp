#include "World.h"

#include <assert.h>

#include "AssetList.h"
#include "Graphic.h"

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
    
    InstancedMesh = MGraphic::AddInstancedMeshToDraw({}, JU_ASSET_SUZANNE);
    const TVector3f MinPosition = TVector3f(-10.0f, -10.00f, -10.0f);
    const TVector3f MaxPosition = TVector3f(10.0f, 10.00f, 10.0f);
    for (int i = 1; i < 100; ++i)
    {
        TTransform NewTransform = { MMath::RandomVectorIntegerInRange(MinPosition, MaxPosition), 0.0f, 0.0f, 0.0f };
        InstancedMesh->Transforms.push_back(NewTransform);
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUpdate(const float& _dt)
{
    FreeLookCamera.UpdateCamera(_dt);

    Mesh->Transform.Rotator.Yaw += 1.0f * _dt;
}
///---------------------------------------------------------------------------------------------------------------------
