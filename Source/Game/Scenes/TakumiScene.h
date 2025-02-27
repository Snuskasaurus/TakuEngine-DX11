﻿#pragma once

#include "../../Engine/Scene.h"

class CTakumiScene final : public CGameScene
{
protected:
    virtual void OnCreate() override;
    virtual void OnUpdate(const float& _dt) override;
    virtual void OnDestroy() override;

    void OnKeyPressed(EKeyCode _key) override;
    
private:
    CDrawable_InstancedMesh* InstancedMesh = nullptr;
};
