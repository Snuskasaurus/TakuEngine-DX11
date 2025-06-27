#include "Inputmanager.h"

#include <Windows.h>

#include "IncludesExternal.h"

#include "GameWindow.h"
#include "World.h"

bool G_KEY_THIS_FRAME[EKeyCode::KEY_INVALID];

struct FKeyEvent
{
    bool IsDown;
    EKeyCode Code;
};

std::vector<FKeyEvent> KeyEventsFromLastFrame;
bool AreKeyDown[EKeyCode::KEY_INVALID] = { false };
std::vector<FKeyEvent> KeyEventsThisFrame;
TVector2f LastMousePosition;
TVector2f MouseMovement;
bool CursorIsLocked = false;

///---------------------------------------------------------------------------------------------------------------------
MInput* MInput::Instance = nullptr;
///---------------------------------------------------------------------------------------------------------------------
void MInput::InitializeInput()
{
    assert(Instance == nullptr);
    Instance = new MInput;

    KeyEventsFromLastFrame.reserve(64);
    KeyEventsThisFrame.reserve(64);
    
    POINT cursorPos = {0, 0};
    assert(GetCursorPos(&cursorPos));
    {
        LastMousePosition = { (float)cursorPos.x, (float)cursorPos.y };
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::DetectInputs()
{
    POINT cursorPos = {0, 0};
    assert(GetCursorPos(&cursorPos));
    {
        TVector2f newMousePosition = { (float)cursorPos.x, (float)cursorPos.y };
        MouseMovement = LastMousePosition - newMousePosition;
        LastMousePosition = newMousePosition;
    }
    if (CursorIsLocked == true)
    {
        cursorPos.x = (int)MGameWindow::GetGameWindowWidth() / 2;
        cursorPos.y = (int)MGameWindow::GetGameWindowHeight() / 2;
        ClientToScreen(MGameWindow::GetWindowHandle(), &cursorPos);
        SetCursorPos(cursorPos.x, cursorPos.y);
        LastMousePosition = { (float)cursorPos.x, (float)cursorPos.y };
    }
    
    if (KeyEventsFromLastFrame.empty())
        return;
    
    for (const auto& KeyEvent : KeyEventsFromLastFrame)
    {
        if (KeyEvent.Code == EKeyCode::KEY_INVALID)
            return;
        AreKeyDown[KeyEvent.Code] = KeyEvent.IsDown;
    }
    
    KeyEventsThisFrame.resize(KeyEventsFromLastFrame.size());
    std::memcpy(KeyEventsThisFrame.data(), KeyEventsFromLastFrame.data(), KeyEventsFromLastFrame.size() * sizeof(FKeyEvent));
    KeyEventsFromLastFrame.clear();
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::ClearInputs()
{
    KeyEventsThisFrame.clear();
}
///---------------------------------------------------------------------------------------------------------------------
TInputHolder* MInput::GetInputHolder()
{
    return &Instance->InputHolder;
}
///---------------------------------------------------------------------------------------------------------------------
bool MInput::IsCursorLocked()
{
    return CursorIsLocked;
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::ToggleCursorLock()
{
    CursorIsLocked = !CursorIsLocked;
    ShowCursor(!CursorIsLocked);
}
///---------------------------------------------------------------------------------------------------------------------
TVector2f MInput::GetMousePosition()
{
    POINT cursorPos = {0, 0};
    if (GetCursorPos(&cursorPos) == false)
    {
        return { 0.0f, 0.0f };
    }
    
    if (ScreenToClient(MGameWindow::GetWindowHandle(), &cursorPos) == false)
    {
        return { 0.0f, 0.0f };
    }
    
    return { static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y) };
}

TVector2f MInput::GetMouseMovement()
{
    return MouseMovement;
}

///---------------------------------------------------------------------------------------------------------------------
void MInput::DispatchKeyEventsToScenes()
{
    for (const auto& KeyEvent : KeyEventsThisFrame)
    {
        if (KeyEvent.IsDown)
        {
            // TODO Julien Rogel (18/06/2025): Prevent double KeyPressEvent
            MWorld::NotifyKeyPressedToGameScene(KeyEvent.Code);
        }
        else
        {
            MWorld::NotifyKeyReleasedToGameScene(KeyEvent.Code);
        }
    }
}
///---------------------------------------------------------------------------------------------------------------------
bool MInput::IsKeyUp(EKeyCode KeyCode)
{
    return !AreKeyDown[KeyCode];
}
///---------------------------------------------------------------------------------------------------------------------
bool MInput::IsKeyDown(EKeyCode KeyCode)
{
    return AreKeyDown[KeyCode];
}
///---------------------------------------------------------------------------------------------------------------------
EKeyCode TranslateWin32KeyToKeyCode(const WPARAM wParam)
{
    switch (wParam)
    {
        case VK_ESCAPE:     return KEY_ESCAPE;
        case '1':           return KEY_1;
        case '2':           return KEY_2;
        case '3':           return KEY_3;
        case '4':           return KEY_4;
        case '5':           return KEY_5;
        case '6':           return KEY_6;
        case '7':           return KEY_7;
        case '8':           return KEY_8;
        case '9':           return KEY_9;
        case '0':           return KEY_0;
        case VK_OEM_MINUS:  return KEY_MINUS;
        case VK_OEM_PLUS:   return KEY_EQUALS;
        case VK_BACK:       return KEY_BACKSPACE;

        case VK_TAB:        return KEY_TAB;
        case 'Q':           return KEY_Q;
        case 'W':           return KEY_W;
        case 'E':           return KEY_E;
        case 'R':           return KEY_R;
        case 'T':           return KEY_T;
        case 'Y':           return KEY_Y;
        case 'U':           return KEY_U;
        case 'I':           return KEY_I;
        case 'O':           return KEY_O;
        case 'P':           return KEY_P;
        case VK_OEM_4:      return KEY_LBRACKET;
        case VK_OEM_6:      return KEY_RBRACKET;
        case VK_RETURN:     return KEY_ENTER;
        case VK_CONTROL:    return KEY_LEFT_CTRL;
        case VK_LCONTROL:   return KEY_LEFT_CTRL;

        case 'A':           return KEY_A;
        case 'S':           return KEY_S;
        case 'D':           return KEY_D;
        case 'F':           return KEY_F;
        case 'G':           return KEY_G;
        case 'H':           return KEY_H;
        case 'J':           return KEY_J;
        case 'K':           return KEY_K;
        case 'L':           return KEY_L;
        case VK_OEM_1:      return KEY_SEMICOLON;
        case VK_OEM_7:      return KEY_APOSTROPHE;
        case VK_OEM_3:      return KEY_GRAVE;
        case VK_SHIFT:      return KEY_LEFT_SHIFT;
        case VK_LSHIFT:     return KEY_LEFT_SHIFT;
        case VK_OEM_5:      return KEY_BACKSLASH;

        case 'Z':           return KEY_Z;
        case 'X':           return KEY_X;
        case 'C':           return KEY_C;
        case 'V':           return KEY_V;
        case 'B':           return KEY_B;
        case 'N':           return KEY_N;
        case 'M':           return KEY_M;
        case VK_OEM_COMMA:  return KEY_COMMA;
        case VK_OEM_PERIOD: return KEY_PERIOD;
        case VK_OEM_2:      return KEY_SLASH;
        case VK_RSHIFT:     return KEY_RIGHT_SHIFT;
        case VK_MULTIPLY:   return KEY_KEYPAD_MULTIPLY;
        case VK_LMENU:      return KEY_LEFT_ALT;
        case VK_SPACE:      return KEY_SPACE;

        case VK_CAPITAL:    return KEY_CAPS_LOCK;
        case VK_F1:         return KEY_F1;
        case VK_F2:         return KEY_F2;
        case VK_F3:         return KEY_F3;
        case VK_F4:         return KEY_F4;
        case VK_F5:         return KEY_F5;
        case VK_F6:         return KEY_F6;
        case VK_F7:         return KEY_F7;
        case VK_F8:         return KEY_F8;
        case VK_F9:         return KEY_F9;
        case VK_F10:        return KEY_F10;

        case VK_NUMLOCK:    return KEY_NUM_LOCK;
        case VK_SCROLL:     return KEY_SCROLL_LOCK;
        case VK_NUMPAD7:    return KEY_KEYPAD_7;
        case VK_NUMPAD8:    return KEY_KEYPAD_8;
        case VK_NUMPAD9:    return KEY_KEYPAD_9;
        case VK_SUBTRACT:   return KEY_KEYPAD_MINUS;
        case VK_NUMPAD4:    return KEY_KEYPAD_4;
        case VK_NUMPAD5:    return KEY_KEYPAD_5;
        case VK_NUMPAD6:    return KEY_KEYPAD_6;
        case VK_ADD:        return KEY_KEYPAD_PLUS;
        case VK_NUMPAD1:    return KEY_KEYPAD_1;
        case VK_NUMPAD2:    return KEY_KEYPAD_2;
        case VK_NUMPAD3:    return KEY_KEYPAD_3;
        case VK_NUMPAD0:    return KEY_KEYPAD_0;
        case VK_DECIMAL:    return KEY_KEYPAD_PERIOD;

        case VK_OEM_102:    return KEY_OEM_102; // Usually "<" on ISO keyboards
        case VK_F11:        return KEY_F11;
        case VK_F12:        return KEY_F12;

        default:            return KEY_INVALID;
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::HandleKeyUp(WPARAM wParam)
{
    EKeyCode KeyCode = TranslateWin32KeyToKeyCode(wParam);
    if (KeyCode == EKeyCode::KEY_INVALID) return;
    
    KeyEventsFromLastFrame.push_back({false, KeyCode});
}
///---------------------------------------------------------------------------------------------------------------------
void MInput::HandleKeyDown(WPARAM wParam)
{
    EKeyCode KeyCode = TranslateWin32KeyToKeyCode(wParam);
    if (KeyCode == EKeyCode::KEY_INVALID) return;
    
    KeyEventsFromLastFrame.push_back({true, KeyCode});
}
///---------------------------------------------------------------------------------------------------------------------
