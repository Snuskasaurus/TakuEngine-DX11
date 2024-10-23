// https://learn.microsoft.com/fr-fr/windows/win32/api/winbase/nf-winbase-winmain
// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles


#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC; // Allocates a unique device context for each window in the class. Used to support multiples windows
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hIconSm = nullptr;

	const auto GameWindowName = L"Julien's Project";
	wc.lpszClassName = GameWindowName;

	RegisterClassEx(&wc);

	while (true)
	{

	}
	return 0;
}