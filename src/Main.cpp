// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "GameStateManager.h"
#include "Level_Demo.h"
#include "CollisionTest.h"
#include "TestScene.h"
#include "constants.h"
#include "LevelEditor.h"
#include "AudioEngine.h"

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	///////////////////////
	// Variable declaration
	using namespace StarBangBang;

	int gGameRunning = 1;
	s8 fontId = 0;

	AudioEngine audioEngine;
	FMOD::Sound* sound = nullptr;
	audioEngine.CreateSound(&sound, "../Resources/drumloop.wav");
	audioEngine.playSound(sound, false);

	GameStateManager gameStateManager;

	gameStateManager.AddGameState<Level_Demo>(Constants::SceneID::DEMO);
	State* testScene = gameStateManager.AddGameState<Level_Demo>(Constants::SceneID::TEST);
	gameStateManager.SetInitialState(testScene);

	State* sceneDemo = gameStateManager.AddGameState<Level_Demo>(Constants::SceneID::DEMO);
	State* sceneEditor = gameStateManager.AddGameState<LevelEditor>(Constants::SceneID::EDITOR);
	State* sceneTest = gameStateManager.AddGameState<TestScene>(Constants::SceneID::TEST);
	gameStateManager.SetInitialState(sceneDemo);

	// Variable declaration end
	///////////////////////////


	/////////////////
	// Initialization
	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);

	StarBangBang::InitBasicMesh();
	//Full screen
	//AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 60, true, NULL);
	//AEToogleFullScreen(true);

	// Changing the window title
	AESysSetWindowTitle("Captain Stealth");

	// reset the system modules
	AESysReset();

	AEGfxSetBackgroundColor(0.3f, 0.6f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	fontId = AEGfxCreateFont("../Resources/Roboto-Regular.ttf", 12);

	// Initialization end
	/////////////////////

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Update State
		gameStateManager.Update();
		audioEngine.Update();

		//FPS
		AEVec2 camPos;
		AEGfxGetCamPosition(&camPos.x, &camPos.y);


		StarBangBang::Test_BoxUpdate();
		//StarBangBang::Test_CircleUpdate();
		char strBuffer[100];
		memset(strBuffer, 0, 100 * sizeof(char));
		sprintf_s(strBuffer, "FPS:  %.6f", AEFrameRateControllerGetFrameRate());

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		f32 TextWidth, TextHeight;
		AEGfxGetPrintSize(fontId, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(fontId, strBuffer, 0.99f - TextWidth, 0.99f - TextHeight, 1.0f, 1.f, 1.f, 1.f);

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}
	gameStateManager.ExitGame();

	//Audio Engine (temp implementation)
	audioEngine.ReleaseSound(sound);
	audioEngine.Exit();
	
	// free the system
	AESysExit();

}