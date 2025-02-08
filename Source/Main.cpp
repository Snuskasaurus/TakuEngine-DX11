#include "Engine/GameWindow.h"
#include "Engine/Inputmanager.h"

#include "Test.h"
#include "Engine/TimeManager.h"
#include "Engine/World.h"


void DoInitialization(HINSTANCE hInstance, HINSTANCE hPrevInstance)
{
	MTime::InitializeTime();
	CreateGameWindow(hInstance);
	InitializeInput(hInstance);
	MWorld::InitializeWorld();
}

void DoUpdate()
{
	const float dt = MTime::GetDeltaTime();
	MWorld::DoWorldUpdate(dt);
	DoFrame(dt);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	DoInitialization(hInstance, hPrevInstance);
	while (true)
	{
		DetectInputs();
		DoUpdate();
		ClearInputs();
		const SExitResult ExitResult = HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}
