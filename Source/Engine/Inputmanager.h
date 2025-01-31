#pragma once

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

// Created with the help of https://www.braynzarsoft.net/viewtutorial/q16390-18-direct-input

struct TInputHolder
{
    float CameraForward = 0.0f;
    float CameraRight = 0.0f;
    float CameraUp = 0.0f;
    float CameraPitch = 0.0f;
    float CameraYaw = 0.0f;
};

#define GET_INPUT(InputName) GetInputHolder()->InputName

void InitializeInput(HINSTANCE _hInstance);
void DetectInputs(float _dt);
void ClearInputs();

TInputHolder* GetInputHolder();
