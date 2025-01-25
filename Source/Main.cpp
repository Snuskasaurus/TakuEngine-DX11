#include "Engine/GameWindow.h"

#include "Test.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	TestValidation::RunTest_Matrix();
	
	JuProject::CreateGameWindow(hInstance);
	while (true)
	{
		const float dt = 0.01f;
		JuProject::DoFrame(dt);
		const JuProject::SExitResult ExitResult = JuProject::HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}