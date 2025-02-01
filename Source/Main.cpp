#include "Engine/GameWindow.h"
#include "Engine/Inputmanager.h"

#include "Test.h"
#include "Engine/TimeManager.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	TestValidation::RunTest_Matrix();

	//JuProject::MeshManager::InitializeMeshManager();
	JuProject::InitializeTime();
	JuProject::CreateGameWindow(hInstance);
	InitializeInput(hInstance);
	
	while (true)
	{
		//Sleep(12);
		const float dt = JuProject::GetDeltaTime();
		DetectInputs(dt);
		JuProject::DoFrame(dt);
		ClearInputs();
		
		const JuProject::SExitResult ExitResult = JuProject::HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}
