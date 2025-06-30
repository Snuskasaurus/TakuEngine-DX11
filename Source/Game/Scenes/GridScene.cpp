#include "GridScene.h"

#include "../MGridSystem.h"
#include "../../Engine/World.h"
#include "../../Engine/Debug/DebugDraw.h"

//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnEvent_Create()
{
    MGridSystem::CreateGrid();
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnEvent_KeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_G)
    {
        MGridSystem::ToggleDisplayingGrid();
    }
    else if (_key == EKeyCode::KEY_KEYPAD_8)
        SceneLight.AddPitch(5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_2)
        SceneLight.AddPitch(-5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_4)
        SceneLight.AddYaw(-5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_6)
        SceneLight.AddYaw(5.0f);
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnEvent_Update(const float& _dt)
{
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnEvent_Destroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
