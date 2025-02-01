#include "Engine/GameWindow.h"
#include "Engine/Inputmanager.h"

#include "Test.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	TestValidation::RunTest_Matrix();
	
	JuProject::CreateGameWindow(hInstance);
	InitializeInput(hInstance);
	
	while (true)
	{
		//Sleep(16);
		const float dt = 0.01f;
		
		DetectInputs(dt);
		JuProject::DoFrame(dt);
		ClearInputs();
		
		const JuProject::SExitResult ExitResult = JuProject::HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}