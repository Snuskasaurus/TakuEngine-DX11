#include "World.h"

#include <assert.h>

#include "GraphicPipeline.h"

#define MESH_TO_IMPORT_SQUARE   "Square"
#define MESH_TO_IMPORT_CUBE     "Cube"
#define MESH_TO_IMPORT_SPHERE   "Sphere"
#define MESH_TO_IMPORT_SUZANNE  "Suzanne"
#define MESH_TO_IMPORT_CRATE    "Crate"
#define MESH_TO_IMPORT_MONSTER  "Monster"

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
    
    MGraphic::AddMeshToDraw({TVector3f{0.0f, 0.0f, -1.0f}}, MESH_TO_IMPORT_CRATE);
    MGraphic::AddMeshToDraw({}, MESH_TO_IMPORT_MONSTER);
}
///---------------------------------------------------------------------------------------------------------------------
void MWorld::OnUpdate(const float& _dt)
{
    FreeLookCamera.UpdateCamera(_dt);
}
///---------------------------------------------------------------------------------------------------------------------
