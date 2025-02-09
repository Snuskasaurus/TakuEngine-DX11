#pragma once

// Created with the help of https://www.braynzarsoft.net/viewtutorial/q16390-18-direct-input

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

struct TInputHolder
{
    float CameraForward = 0.0f;
    float CameraRight = 0.0f;
    float CameraUp = 0.0f;
    float CameraPitch = 0.0f;
    float CameraYaw = 0.0f;
    float CameraSpeed = 0.0f;
};

class MInput
{
private:
    static MInput* Instance;

public:
    static void InitializeInput(HINSTANCE);
    static void DetectInputs();
    static void ClearInputs();
    static TInputHolder* GetInputHolder();

private:
    static void DetectInputs_Keyboard(const BYTE*);
    static void DetectInputs_Mouse(DIMOUSESTATE);
    
private:
    TInputHolder InputHolder;
};

#define GET_INPUT(InputName) MInput::GetInputHolder()->InputName
