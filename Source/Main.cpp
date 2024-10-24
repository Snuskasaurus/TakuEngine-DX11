// https://learn.microsoft.com/fr-fr/windows/win32/api/winbase/nf-winbase-winmain
// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles

#include "Engine/GameWindow.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	JuProject::CreateGameWindow(hInstance);
	return JuProject::HandleGameWindowMessage();
}