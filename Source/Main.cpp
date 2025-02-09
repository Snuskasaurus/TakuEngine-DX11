#include "Engine/GameWindow.h"
#include "Engine/Inputmanager.h"

#include "Test.h"
#include "Engine/TimeManager.h"
#include "Engine/World.h"

namespace MainLoopEngine
{
///---------------------------------------------------------------------------------------------------------------------
void Initialization(HINSTANCE hInstance, HINSTANCE hPrevInstance)
{
	MTime::InitializeTime();
	CreateGameWindow(hInstance);
	MInput::InitializeInput(hInstance);
	MWorld::InitializeWorld();
}
///---------------------------------------------------------------------------------------------------------------------
void Update()
{
	MInput::DetectInputs();
	
	const float dt = MTime::GetDeltaTime();
	MWorld::UpdateWorld(dt);
	
	MInput::ClearInputs();
}
///---------------------------------------------------------------------------------------------------------------------
void Draw()
{
	DrawGameWindow();
}
///---------------------------------------------------------------------------------------------------------------------
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	MainLoopEngine::Initialization(hInstance, hPrevInstance);
	while (true)
	{
		MainLoopEngine::Update();
		MainLoopEngine::Draw();
		const SExitResult ExitResult = HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}
