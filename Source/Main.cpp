#include "Engine/GameWindow.h"
#include "Engine/Inputmanager.h"

#include "Test.h"
#include "Engine/TimeManager.h"
#include "Engine/World.h"

namespace TakuEngine
{
	void Initialization(HINSTANCE hInstance, HINSTANCE hPrevInstance)
	{
		MTime::InitializeTime();
		CreateGameWindow(hInstance);
		InitializeInput(hInstance);
		MWorld::InitializeWorld();
	}

	void Update()
	{
		const float dt = MTime::GetDeltaTime();
		MWorld::UpdateWorld(dt);
	}

	void Draw()
	{
		DoFrame();
	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	TakuEngine::Initialization(hInstance, hPrevInstance);
	while (true)
	{
		DetectInputs();
		TakuEngine::Update();
		ClearInputs();
		TakuEngine::Draw();
		
		const SExitResult ExitResult = HandleGameWindowMessage();
		if (ExitResult.Exiting == true) return ExitResult.Reason; // Exit application
	}
}
