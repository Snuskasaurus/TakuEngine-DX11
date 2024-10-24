#include "Engine/GameWindow.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	JuProject::CreateGameWindow(hInstance);

	while (true)
	{
		JuProject::DoFrame();
		const JuProject::SExitResult ExitResult = JuProject::HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}