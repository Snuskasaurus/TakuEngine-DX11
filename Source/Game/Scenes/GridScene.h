#pragma once

#include "../../Engine/Scene.h"

class CGridScene final : public CGameScene
{
protected:
    virtual void OnEvent_Create() override;
    void OnEvent_KeyPressed(EKeyCode _key) override;
    virtual void OnEvent_Update(const float& _dt) override;
    virtual void OnEvent_Destroy() override;
};
