#pragma once

#include "../../Engine/Scene.h"
#include "Game/WorldMap.h"

class CGameTerrainScene final : public CGameScene
{
protected:
    virtual void OnEvent_Create() override;
    void OnEvent_KeyReleased(EKeyCode _key) override;
    virtual void OnEvent_Update(const float& _dt) override;
    virtual void OnEvent_Destroy() override;

private:
    void HandleCursorTerrainCollision();

private:
    FWorldMap WorldMap;

private:
    TVector3f CursorPosition;
    
private:
    CDrawable_InstancedMesh* WaterMesh = nullptr;
    CDrawable_InstancedMesh* TerrainMesh = nullptr;
    CDrawable_InstancedMesh* SelectorMesh = nullptr;
};
