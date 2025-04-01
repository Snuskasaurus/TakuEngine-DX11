#pragma once

#include "../../Engine/Scene.h"

class CPlanetScene final : public CGameScene
{
protected:
    virtual void OnCreate() override;
    void OnKeyPressed(EKeyCode _key) override;
    virtual void OnUpdate(const float& _dt) override;
    virtual void OnDestroy() override;
};
