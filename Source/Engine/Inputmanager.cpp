#include "Inputmanager.h"

#include <Windows.h>

#include "CoreIncludesExternal.h"

#include "HResultHandler.h"
#include "GameWindow.h"

///---------------------------------------------------------------------------------------------------------------------
LPDIRECTINPUT8 G_DIRECT_INPUT;
IDirectInputDevice8* G_INPUT_DEVICE_KEYBOARD;
IDirectInputDevice8* G_INPUT_DEVICE_MOUSE;
///---------------------------------------------------------------------------------------------------------------------
MInput* MInput::Instance = nullptr;
///---------------------------------------------------------------------------------------------------------------------
void MInput::InitializeInput(HINSTANCE _hInstance)
{
    assert(Instance == nullptr);
    Instance = new MInput;

    CHECK_HRESULT(DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&G_DIRECT_INPUT, nullptr));
    CHECK_HRESULT(G_DIRECT_INPUT->CreateDevice(GUID_SysKeyboard, &G_INPUT_DEVICE_KEYBOARD, nullptr));
    CHECK_HRESULT(G_DIRECT_INPUT->CreateDevice(GUID_SysMouse, &G_INPUT_DEVICE_MOUSE, nullptr));

    const HWND hwnd = MGameWindow::GetWindowHandle();
    
    CHECK_HRESULT(G_INPUT_DEVICE_KEYBOARD->SetDataFormat(&c_dfDIKeyboard));
    CHECK_HRESULT(G_INPUT_DEVICE_KEYBOARD->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

    CHECK_HRESULT(G_INPUT_DEVICE_MOUSE->SetDataFormat(&c_dfDIMouse));
    CHECK_HRESULT(G_INPUT_DEVICE_MOUSE->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND));
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::DetectInputs()
{
    if (MGameWindow::HasFocus() == false)
        return;
    
    BYTE keyboardState[256];
    G_INPUT_DEVICE_KEYBOARD->Acquire();
    G_INPUT_DEVICE_KEYBOARD->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);
    DetectInputs_Keyboard(keyboardState);

    DIMOUSESTATE mouseCurrState;
    G_INPUT_DEVICE_MOUSE->Acquire();
    G_INPUT_DEVICE_MOUSE->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
    DetectInputs_Mouse(mouseCurrState);
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::ClearInputs()
{
    Instance->InputHolder = {};
}
///---------------------------------------------------------------------------------------------------------------------
TInputHolder* MInput::GetInputHolder()
{
    return &Instance->InputHolder;
}
///---------------------------------------------------------------------------------------------------------------------
#define KEYBOARD_CALL_FUNCTION(Key, CallFunc) if(_keyboardState[Key] & 0x80) { CallFunc; }
#define KEYBOARD_CHANGE_INPUT_HOLDER(Key, Input, Value) if(_keyboardState[Key] & 0x80) { Instance->InputHolder.Input = Value; }
///---------------------------------------------------------------------------------------------------------------------
void MInput::DetectInputs_Keyboard(const BYTE* _keyboardState)
{
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_D, CameraRight, 1.0f)
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_A, CameraRight, -1.0f)
    
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_W, CameraForward, 1.0f)
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_S, CameraForward, -1.0f)
    
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_LSHIFT, CameraUp, 1.0f)
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_LCONTROL, CameraUp, -1.0f)

#ifdef _DEBUG
    KEYBOARD_CALL_FUNCTION(DIK_ESCAPE, PostQuitMessage(1))
#endif
}
///---------------------------------------------------------------------------------------------------------------------
#undef KEYBOARD_CALL_FUNCTION
#undef KEYBOARD_CHANGE_INPUT_HOLDER
///---------------------------------------------------------------------------------------------------------------------
#define MOUSE_AXE_CHANGE_INPUT_HOLDER_POSITIVE(Axis, Input) if(_mouseState.Axis != 0) { Instance->InputHolder.Input = (float)_mouseState.Axis; }
#define MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE(Axis, Input) if(_mouseState.Axis != 0) { Instance->InputHolder.Input = (float)-_mouseState.Axis; }
///---------------------------------------------------------------------------------------------------------------------
void MInput::DetectInputs_Mouse(DIMOUSESTATE _mouseState)
{
    MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE(lX, CameraYaw)
    MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE(lY, CameraPitch)
    MOUSE_AXE_CHANGE_INPUT_HOLDER_POSITIVE(lZ, CameraSpeed)
}
///---------------------------------------------------------------------------------------------------------------------
#undef MOUSE_AXE_CHANGE_INPUT_HOLDER_POSITIVE
#undef MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE
///---------------------------------------------------------------------------------------------------------------------