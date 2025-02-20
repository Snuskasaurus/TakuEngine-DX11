#include <ctime>

#include "Resources/MeshResources.h"
#include "TimeManager.h"
#include "World.h"
#include "GameWindow.h"
#include "Inputmanager.h"
#include "AssetList.h"

namespace TakuEngine
{
///--------------------------------------------------------------------------------------------------------------------------------------------------------
void LoadAssets()
{
	MAsset::LoadMeshes();
	MAsset::LoadShaders();
}
///--------------------------------------------------------------------------------------------------------------------------------------------------------
void Initialization(HINSTANCE hInstance, HINSTANCE hPrevInstance)
{
	MMeshResources::InitializeMeshResources();
	MGameWindow::InitializeGameWindow(hInstance);
	MTime::InitializeTime();
	MInput::InitializeInput(hInstance);
	MWorld::InitializeWorld();
}
///--------------------------------------------------------------------------------------------------------------------------------------------------------
void Update()
{
	MInput::DetectInputs();
	
	const float dt = MTime::GetDeltaTime();
	MWorld::UpdateWorld(dt);
	
	MInput::ClearInputs();
}
///--------------------------------------------------------------------------------------------------------------------------------------------------------
void Draw()
{
	MGameWindow::DrawGameWindow();
}
///--------------------------------------------------------------------------------------------------------------------------------------------------------
void Uninitialization()
{
	MGameWindow::UninitializeGameWindow();
	MMeshResources::UninitializeMeshResources();
}
///--------------------------------------------------------------------------------------------------------------------------------------------------------
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	
	TakuEngine::LoadAssets();
	TakuEngine::Initialization(hInstance, hPrevInstance);
	while (true)
	{
		TakuEngine::Update();
		TakuEngine::Draw();
		const SExitResult exitResult = MGameWindow::HandleGameWindowMessage();
		if (exitResult.Exiting == true)
		{
			TakuEngine::Uninitialization();
			return exitResult.Reason;
		}
	}
}
