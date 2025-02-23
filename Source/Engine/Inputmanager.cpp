#include "Inputmanager.h"

#include <Windows.h>

#include "IncludesExternal.h"

#include "HResultHandler.h"
#include "GameWindow.h"
#include "World.h"

///---------------------------------------------------------------------------------------------------------------------
LPDIRECTINPUT8 G_DIRECT_INPUT;
IDirectInputDevice8* G_INPUT_DEVICE_KEYBOARD;
IDirectInputDevice8* G_INPUT_DEVICE_MOUSE;
BYTE G_LAST_KEYBOARD_STATE[256];
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

    DIPROPDWORD dipdw = {};
    {
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = 16;
    }
    CHECK_HRESULT(G_INPUT_DEVICE_KEYBOARD->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));
    
    CHECK_HRESULT(G_INPUT_DEVICE_MOUSE->SetDataFormat(&c_dfDIMouse));
    CHECK_HRESULT(G_INPUT_DEVICE_MOUSE->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND));
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::DetectInputs()
{
    if (MGameWindow::HasFocus() == false)
        return;
    
    BYTE newKeyboardState[256];
    G_INPUT_DEVICE_KEYBOARD->Acquire();
    G_INPUT_DEVICE_KEYBOARD->GetDeviceState(sizeof(newKeyboardState),(LPVOID)&newKeyboardState);
    FillInputHolder_Keyboard(newKeyboardState);
    
    DIDEVICEOBJECTDATA rgdod[16];
    DWORD dwItems = 16;
    CHECK_HRESULT(G_INPUT_DEVICE_KEYBOARD->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), rgdod, &dwItems, 0));

    // Process each event
    for (DWORD i = 0; i < dwItems; i++)
    {
        if (rgdod[i].dwData & 0x80) // Key pressed
        {
            const DWORD KeyValue = rgdod[i].dwOfs;
            const EKeyCode KeyCode = (KeyValue > 0 && KeyValue < EKeyCode::KEY_INVALID) ? (EKeyCode)(KeyValue - 1) : EKeyCode::KEY_INVALID;
            MWorld::GetWorld()->CurrentGameScene->NotifyKeyPressed(KeyCode);
        }
    }

    DIMOUSESTATE mouseCurrState;
    G_INPUT_DEVICE_MOUSE->Acquire();
    G_INPUT_DEVICE_MOUSE->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
    FillInputHolder_Mouse(mouseCurrState);
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
void MInput::FillInputHolder_Keyboard(const BYTE* _keyboardState)
{
#define KEYBOARD_CHANGE_INPUT_HOLDER(Key, Input, Value) if(_keyboardState[Key] & 0x80) { Instance->InputHolder.Input = Value; }
    
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_D, CameraRight, 1.0f)
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_A, CameraRight, -1.0f)
    
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_W, CameraForward, 1.0f)
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_S, CameraForward, -1.0f)
    
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_LSHIFT, CameraUp, 1.0f)
    KEYBOARD_CHANGE_INPUT_HOLDER(DIK_LCONTROL, CameraUp, -1.0f)

#undef KEYBOARD_CHANGE_INPUT_HOLDER
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::FillInputHolder_Mouse(DIMOUSESTATE _mouseState)
{
#define MOUSE_AXE_CHANGE_INPUT_HOLDER_POSITIVE(Axis, Input) if(_mouseState.Axis != 0) { Instance->InputHolder.Input = (float)_mouseState.Axis; }
#define MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE(Axis, Input) if(_mouseState.Axis != 0) { Instance->InputHolder.Input = (float)-_mouseState.Axis; }
    
    MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE(lX, CameraYaw)
    MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE(lY, CameraPitch)
    MOUSE_AXE_CHANGE_INPUT_HOLDER_POSITIVE(lZ, CameraSpeedModifier)
    
#undef MOUSE_AXE_CHANGE_INPUT_HOLDER_POSITIVE
#undef MOUSE_AXE_CHANGE_INPUT_HOLDER_NEGATIVE
}
///---------------------------------------------------------------------------------------------------------------------