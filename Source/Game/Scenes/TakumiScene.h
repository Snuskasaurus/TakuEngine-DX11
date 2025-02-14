#pragma once

#include "../../Engine/Scene.h"
#include "../../Engine/Graphic.h"

class CTakumiScene: public CGameScene
{
protected:
    virtual void OnCreate() override;
    virtual void OnUpdate(const float& _dt) override;
    virtual void OnDestroy() override;

private:
    CInstancedMesh* InstancedMesh = nullptr;
};
