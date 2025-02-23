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
    KEY_BACKSPACE,

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
    KEY_ENTER,
    KEY_LEFT_CTRL,

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
    KEY_LEFT_SHIFT,
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
    KEY_RIGHT_SHIFT,
    KEY_KEYPAD_MULTIPLY,
    KEY_LEFT_ALT,
    KEY_SPACE,

    KEY_CAPS_LOCK,
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

    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_KEYPAD_7,
    KEY_KEYPAD_8,
    KEY_KEYPAD_9,
    KEY_KEYPAD_MINUS,
    KEY_KEYPAD_4,
    KEY_KEYPAD_5,
    KEY_KEYPAD_6,
    KEY_KEYPAD_PLUS,
    KEY_KEYPAD_1,
    KEY_KEYPAD_2,
    KEY_KEYPAD_3,
    KEY_KEYPAD_0,
    KEY_KEYPAD_PERIOD,

    KEY_OEM_102,
    KEY_F11,
    KEY_F12,
    
    KEY_INVALID
};

struct TInputHolder
{
    float CameraForward = 0.0f;
    float CameraRight = 0.0f;
    float CameraUp = 0.0f;
    float CameraPitch = 0.0f;
    float CameraYaw = 0.0f;
    float CameraSpeedModifier = 0.0f;
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
    static void FillInputHolder_Keyboard(const BYTE*);
    static void FillInputHolder_Mouse(DIMOUSESTATE);
    
private:
    TInputHolder InputHolder;
};

#define GET_INPUT(InputName) MInput::GetInputHolder()->InputName
