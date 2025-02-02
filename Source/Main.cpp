#include "Engine/GameWindow.h"
#include "Engine/Inputmanager.h"

#include "Test.h"
#include "Engine/TimeManager.h"
#include "Engine/World.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	TestValidation::RunTest_Matrix();

	//MeshManager::InitializeMeshManager();
	MTime::InitializeTime();
	CreateGameWindow(hInstance);
	InitializeInput(hInstance);
	MWorld::InitializeWorld();
	
	while (true)
	{
		const float dt = MTime::GetDeltaTime();
		{
			DetectInputs(dt);
			MWorld::DoWorldUpdate(dt);
			DoFrame(dt);
			ClearInputs();
		}
		const SExitResult ExitResult = HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}
