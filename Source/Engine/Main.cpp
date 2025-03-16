#include <ctime>

#include "Resources/MeshResources.h"
#include "TimeManager.h"
#include "World.h"
#include "GameWindow.h"
#include "Inputmanager.h"
#include "Graphics/Graphic.h"
#include "Resources/AssetList.h"
#include "Debug/DebugDraw.h"

namespace TakuEngine
{
///--------------------------------------------------------------------------------------------------------------------------------------------------------
void Initialization(HINSTANCE hInstance, HINSTANCE hPrevInstance)
{
	MDebugDraw::Prepare();
	MGameWindow::InitializeGameWindow(hInstance);
	MGraphic::InitializeGraphic();
	MAsset::LoadAssets();
	MGraphic::InitializeShaders();
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
	MGraphic::PrepareUninitializeGraphic();
	MWorld::UninitializeWorld();
	MAsset::UnloadAssets();
	MGraphic::UninitializeGraphic();
	MGameWindow::UninitializeGameWindow();
}
///--------------------------------------------------------------------------------------------------------------------------------------------------------
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	
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
