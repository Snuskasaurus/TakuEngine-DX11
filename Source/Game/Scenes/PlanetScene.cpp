#include "PlanetScene.h"

#include "../../Engine/World.h"

void CreateIcospherePoints()
{
    // https://danielsieger.com/blog/2021/03/27/generating-spheres.html
    // https://blog.lslabs.dev/posts/generating_icosphere_with_code
    
}

//---------------------------------------------------------------------------------------------------------------------
void CPlanetScene::OnCreate()
{
    SceneLight.SetYaw(120.0f);
    SceneLight.SetPitch(130.0f);

    CreateIcospherePoints();
}
//---------------------------------------------------------------------------------------------------------------------
void CPlanetScene::OnKeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
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
void CPlanetScene::OnUpdate(const float& _dt)
{
    
}
//---------------------------------------------------------------------------------------------------------------------
void CPlanetScene::OnDestroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
