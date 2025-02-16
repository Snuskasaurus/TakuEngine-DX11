#pragma once

// Created with the help of https://www.braynzarsoft.net/viewtutorial/q16390-18-direct-input
//                          https://raptor.developpez.com/tutorial/direct_input/base/

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum EKeyCode
{
    KEY_ESCAPE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_MINUS,
    KEY_EQUALS,
    KEY_BACK,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_LBRACKET,
    KEY_RBRACKET,
    KEY_RETURN,
    KEY_LCONTROL,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_GRAVE,
    KEY_LSHIFT,
    KEY_BACKSLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_RSHIFT,
    KEY_MULTIPLY,
    KEY_LMENU,
    KEY_SPACE,
    KEY_CAPITAL,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUMLOCK,
    KEY_SCROLL,
    KEY_NUMPAD7,
    KEY_NUMPAD8,
    KEY_NUMPAD9,
    KEY_SUBTRACT,
    KEY_NUMPAD4,
    KEY_NUMPAD5,
    KEY_NUMPAD6,
    KEY_ADD,
    KEY_NUMPAD1,
    KEY_NUMPAD2,
    KEY_NUMPAD3,
    KEY_NUMPAD0,
    KEY_DECIMAL,
    KEY_OEM_102,
    KEY_F11,
    KEY_F12,
    KEY_HOME,
    KEY_UP,
    KEY_PRIOR,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_END,
    KEY_DOWN,
    KEY_NEXT,
    KEY_INSERT,
    KEY_DELETE
};



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
