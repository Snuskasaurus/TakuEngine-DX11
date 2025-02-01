#include "Inputmanager.h"

#include <Windows.h>

#include "HResultHandler.h"
#include "GameWindow.h"

DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

TInputHolder Inputs;

IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;

///---------------------------------------------------------------------------------------------------------------------
void InitializeInput(HINSTANCE _hInstance)
{
    CHECK_HRESULT(DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, nullptr));
    CHECK_HRESULT(DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, nullptr));
    CHECK_HRESULT(DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, nullptr));

    HWND hwnd = JuProject::GetGameWindow();
    
    CHECK_HRESULT(DIKeyboard->SetDataFormat(&c_dfDIKeyboard));
    CHECK_HRESULT(DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

    CHECK_HRESULT(DIMouse->SetDataFormat(&c_dfDIMouse));
    CHECK_HRESULT(DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND));
}
///---------------------------------------------------------------------------------------------------------------------
void DetectKeyboardInputs(float _dt)
{
    BYTE keyboardState[256];
    DIKeyboard->Acquire();
    DIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

#define INPUT_KEYBOARD_KEY(Key, Input, Value) if(keyboardState[Key] & 0x80) { Inputs.Input = Value; }

    INPUT_KEYBOARD_KEY(DIK_D, CameraRight, 1.0f)
    INPUT_KEYBOARD_KEY(DIK_A, CameraRight, -1.0f)
    
    INPUT_KEYBOARD_KEY(DIK_W, CameraForward, 1.0f)
    INPUT_KEYBOARD_KEY(DIK_S, CameraForward, -1.0f)
    
    INPUT_KEYBOARD_KEY(DIK_LSHIFT, CameraUp, 1.0f)
    INPUT_KEYBOARD_KEY(DIK_LCONTROL, CameraUp, -1.0f)

#undef INPUT_KEYBOARD_KEY
}
///---------------------------------------------------------------------------------------------------------------------
void DetectMouseInputs(float _dt)
{
    DIMOUSESTATE mouseCurrState;
    DIMouse->Acquire();
    DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
    
    if(mouseCurrState.lX != mouseLastState.lX)
    {
        Inputs.CameraYaw = -(float)mouseCurrState.lX;
    }
    if (mouseCurrState.lY != mouseLastState.lY)
    {
        Inputs.CameraPitch = -(float)mouseCurrState.lY;
    }
}
///---------------------------------------------------------------------------------------------------------------------
void DetectInputs(float _dt)
{
    DetectKeyboardInputs(_dt);
    DetectMouseInputs(_dt);
}
///---------------------------------------------------------------------------------------------------------------------
void ClearInputs()
{
    Inputs = {};
}
///---------------------------------------------------------------------------------------------------------------------
TInputHolder* GetInputHolder()
{
    return &Inputs;
}
///---------------------------------------------------------------------------------------------------------------------
