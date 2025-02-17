#pragma once

#include "../../Engine/Scene.h"

class CGridScene final : public CGameScene
{
protected:
    virtual void OnCreate() override;
    virtual void OnUpdate(const float& _dt) override;
    virtual void OnDestroy() override;

private:
    CDrawable_InstancedMesh* InstancedMesh = nullptr;
};
