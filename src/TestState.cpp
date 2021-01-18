#include "TestState.h"
using namespace StarBangBang;

TestState::TestState(GameStateManager* gameStateManager)
{
	object2 = nullptr;
	object2Child = nullptr;
	objectImage = nullptr;
	tileImage = nullptr;
	this->gameStateManager = gameStateManager;
}

void StarBangBang::TestState::Load()
{
	objectImage = objectManager.ImageLoad(graphicsManager, 100, 100, "../Resources/boi.png", 255);
	tileImage = objectManager.ImageLoad(graphicsManager, 100, 100, "../Resources/grass.png", 255);

	object2 = objectManager.NewGameObject(100, 100);
	object2Child = objectManager.NewGameObject(50, 50);
	objectManager.AddImageComponent(object2, graphicsManager, "../Resources/PlanetTexture.png");
	objectManager.AddImageComponent(object2Child, graphicsManager, "../Resources/boi.png");
}

void TestState::Init()
{
	////////////////////////////////
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	object2->transform.position.x = 100;
	objectManager.AddDragComponent(object2);
	objectManager.AddDragComponent(object2Child);
	objectManager.AddChildGameObject(object2Child, object2);

	tileManager.Init(objectManager, graphicsManager);


	//////////////////////////////////
	// Creating Fonts	
	fontId = AEGfxCreateFont("../Resources/Roboto-Regular.ttf", 12);
	// Creating Fonts end
	//////////////////////////////////
}

void TestState::Update()
{
	objectManager.Update();
	//Move child
	//double dt = AEFrameRateControllerGetFrameTime();
	//object2Child->transform.position.x += 100 * dt;

	if (AEInputCheckTriggered(VK_SPACE))
	{
		State* nextState = gameStateManager->AddGameState<TestState2>();
		gameStateManager->SetNextGameState(nextState);
	}
}

void StarBangBang::TestState::Draw()
{
	objectManager.Draw();

	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "FPS:  %.6f", AEFrameRateControllerGetFrameRate());

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	f32 TextWidth, TextHeight;
	AEGfxGetPrintSize(fontId, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(fontId, strBuffer, 0.99f - TextWidth, 0.99f - TextHeight, 1.0f, 1.f, 1.f, 1.f);
}

void TestState::Free()
{
	//Free objects and textures
	AEGfxDestroyFont(fontId);
	objectManager.FreeObjects();
}

void StarBangBang::TestState::Unload()
{
	graphicsManager.UnloadTextures();
	graphicsManager.FreeMeshes();
}
