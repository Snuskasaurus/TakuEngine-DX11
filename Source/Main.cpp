#include "Engine/GameWindow.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	JuProject::CreateGameWindow(hInstance);

	while (true)
	{
		const TExitReason ExitReason = JuProject::HandleGameWindowMessage();
		if (ExitReason.Exist == true)
			return ExitReason.Reason;
	}
}