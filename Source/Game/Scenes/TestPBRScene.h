#pragma once

#include "../../Engine/Scene.h"

class CTestPBRScene final : public CGameScene
{
protected:
    virtual void OnCreate() override;
    virtual void OnUpdate(const float& _dt) override;
    virtual void OnDestroy() override;
    virtual void OnKeyPressed(EKeyCode _key) override;
};
