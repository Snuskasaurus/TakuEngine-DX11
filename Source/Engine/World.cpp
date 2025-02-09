#include "World.h"

#include <assert.h>

#include "AssetList.h"
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
    SunDirection = { 0.37f, 0.93f, 0.0 };
    
    MGraphic::AddMeshToDraw({TVector3f{0.0f, 0.0f, -1.0f}}, JU_ASSET_CRATE);
    MGraphic::AddMeshToDraw({}, JU_ASSET_MONSTER);
}
///---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUpdate(const float& _dt)
{
    FreeLookCamera.UpdateCamera(_dt);
}
///---------------------------------------------------------------------------------------------------------------------
