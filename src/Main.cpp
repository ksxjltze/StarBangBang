/******************************************************************************/
/*!
\title		Captain Stealth
\file		Main.cpp
\author 	Lee Jia Keat
\par    	email: l.jiakeat@digipen.edu
\date   	April 09, 2021
\brief		Main game loop

Copyright (C) 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "AEEngine.h"
#include "AudioEngine.h"

#include "GameStateManager.h"
#include "constants.h"
#include "SceneList.h"

#include "BasicMeshShape.h"
#include "globals.h"
#include "MessageBus.h"

namespace StarBangBang
{
	float g_dt = 0;
	s8 fontId = -1;
	s8 fontId2 = -1;
	static bool debug = false;
}

// ---------------------------------------------------------------------------
// main

namespace StarBangBang
{
	/*!*************************************************************************
	 * \brief 
	 * Checks if the game is in debug mode.
	 * \return 
	 * True if in debug mode, false otherwise.
	***************************************************************************/
	bool IsDebug()
	{
		return debug;
	}

	/*!*************************************************************************
	 * \brief 
	 * Checks if the game window is in focus (not iconic).
	 * \return 
	 * True if focused, false otherwise
	***************************************************************************/
	bool LostFocus()
	{
		return IsIconic(AESysGetWindowHandle());
	}

	/*!*************************************************************************
	 * \brief 
	 * Displays the current FPS.
	 * \return
	 * void
	***************************************************************************/
	void DisplayFps()
	{
		char strBuffer[100];
		memset(strBuffer, 0, 100 * sizeof(char));
		sprintf_s(strBuffer, "FPS:  %.6f", AEFrameRateControllerGetFrameRate());

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		f32 TextWidth, TextHeight;
		AEGfxGetPrintSize(fontId, strBuffer, 0.5f, TextWidth, TextHeight);
		AEGfxPrint(fontId, strBuffer, 0.99f - TextWidth, 0.99f - TextHeight, 0.5f, 1.f, 1.f, 1.f);
	}

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	StarBangBang::debug = true;
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	///////////////////////
	// Variable declaration
	using namespace StarBangBang;
	int gGameRunning = 1;

	//Audio Initialization
	AudioEngine audioEngine;
	FMOD::Sound* sound = nullptr;
	FMOD::Sound* btnSound = nullptr;
	FMOD::Sound* keyPickupSound = nullptr;
	FMOD::Sound* guardAlert = nullptr;
	FMOD::Sound* doorSound = nullptr;
	FMOD::Sound* ventOpenSound = nullptr;
	FMOD::Sound* ventCloseSound = nullptr;
	FMOD::Sound* walkSound = nullptr;
	FMOD::Sound* menuBgm = nullptr;
	FMOD::Sound* gameBgm = nullptr;
	FMOD::Sound* ggBgm = nullptr;

	//SFX
	audioEngine.CreateSound(&btnSound, RESOURCES::SFX::SFX_BUTTON_CLICK_PATH);
	audioEngine.CreateSound(&keyPickupSound, RESOURCES::SFX::SFX_KEY_PICKUP_PATH);
	audioEngine.CreateSound(&guardAlert, RESOURCES::SFX::SFX_DETECTED_PATH);
	audioEngine.CreateSound(&doorSound, RESOURCES::SFX::SFX_DOOR_OPEN_PATH);
	audioEngine.CreateSound(&ventOpenSound, RESOURCES::SFX::SFX_VENT_OPEN_PATH);
	audioEngine.CreateSound(&ventCloseSound, RESOURCES::SFX::SFX_VENT_CLOSE_PATH);
	audioEngine.CreateSound(&walkSound, RESOURCES::SFX::SFX_WALK_FOOTSTEPS_PATH);

	//BGM
	audioEngine.CreateSound(&menuBgm, RESOURCES::BGM::BGM_MENU_PATH);
	audioEngine.CreateSound(&gameBgm, RESOURCES::BGM::BGM_GAME_PATH);
	audioEngine.CreateSound(&ggBgm, RESOURCES::BGM::BGM_GAMEOVER_PATH);

	//Register sound names
	audioEngine.AddSound(SFX::BUTTON_CLICK, btnSound);
	audioEngine.AddSound(SFX::KEY_PICKUP, keyPickupSound);
	audioEngine.AddSound(SFX::DETECTED, guardAlert);
	audioEngine.AddSound(SFX::DOOR_OPEN, doorSound);
	audioEngine.AddSound(SFX::VENT_CLOSE, ventCloseSound);
	audioEngine.AddSound(SFX::VENT_OPEN, ventOpenSound);
	audioEngine.AddSound(SFX::FOOTSTEPS, walkSound);
	audioEngine.AddSound(BGM::MENU, menuBgm);
	audioEngine.AddSound(BGM::GAME, gameBgm);
	audioEngine.AddSound(BGM::GAMEOVER, ggBgm);

	MessageBus::RegisterGlobalListener(&audioEngine);
	//audioEngine.playSound("Test", false);

	GameStateManager gameStateManager;

	//

	// Variable declaration end
	///////////////////////////


	/////////////////
	// Initialization

	// Add Game States/Scenes
	std::vector<Scene*> sceneList;

	Scene* mainMenuScene	= gameStateManager.AddGameState<Main_Menu>(SceneID::MAIN_MENU);
	Scene* sceneDemo		= gameStateManager.AddGameState<Level_Demo>(SceneID::GAME);
	Scene* sceneEditor		= gameStateManager.AddGameState<LevelEditor>(SceneID::EDITOR);
	Scene* tutorialScene	= gameStateManager.AddGameState<Tutorial>(TUTORIAL);
	Scene* ggScene			= gameStateManager.AddGameState<Scene_GameOver>(GAME_OVER);
	Scene* credits			= gameStateManager.AddGameState<Credits>(CREDITS);
	Scene* logoScene 		= gameStateManager.AddGameState<LogoSplash>();
	
	// Hack to remove unreferenced local variable warning
	sceneList.push_back(sceneDemo);
	sceneList.push_back(sceneEditor);
	sceneList.push_back(tutorialScene);
	sceneList.push_back(mainMenuScene);
	sceneList.push_back(ggScene);
	sceneList.push_back(credits);

	// Set Initial State
	gameStateManager.SetInitialState(logoScene);


	//// Using custom window procedure
	AESysInit(hInstance, nCmdShow, (s32)GRAPHICS::TARGET_WINDOW_WIDTH, (s32)GRAPHICS::TARGET_WINDOW_HEIGHT, debug, 60, true, NULL);
	StarBangBang::InitBasicMesh();
	StarBangBang::CollisionManager::CollisionManagerInit();
	if (!debug)
	{
		//Full screen
		GRAPHICS::ToggleFullscreen();
	}

	// Changing the window title
	AESysSetWindowTitle("Captain Stealth");

	// reset the system modules
	AESysReset();

	AEGfxSetBackgroundColor(0.3f, 0.6f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ALWAYS CREATE FONTS OUTSIDE LOOP
	fontId = AEGfxCreateFont("./Resources/Roboto-Regular.ttf", 32);
	fontId2 = AEGfxCreateFont("./Resources/Roboto-Regular.ttf", 48);
	if (fontId < 0)
	{
		fontId = 0;
		AE_ASSERT_MESG(fontId, "FAILED TO CREATE FONT");
	}

	// Initialization end
	/////////////////////

	//Game Loop
	while (gGameRunning)
	{
		HWND hWnd = AESysGetWindowHandle();

		if (GetAsyncKeyState(AEVK_LALT))
		{
			if (AEInputCheckTriggered(AEVK_RETURN))
				GRAPHICS::ToggleFullscreen();
			else if (GetAsyncKeyState(AEVK_TAB))
				ShowWindow(hWnd, SW_MINIMIZE);

			if (GetAsyncKeyState(AEVK_LCTRL) && GetAsyncKeyState(VK_DELETE))
			{
				ShowWindow(hWnd, SW_MINIMIZE);
			}

		}

		if (GetFocus() == NULL)
		{
			ShowWindow(hWnd, SW_MINIMIZE);
		}

		// Informing the system about the loop's start
		AESysFrameStart();
		g_dt = static_cast<float>(AEFrameRateControllerGetFrameTime());

		// Handling Input
		AEInputUpdate();

		// Events
		MessageBus::Update();

		// Update State
		gameStateManager.Update();

		CollisionManager::ResolverUpdate();
		
		if (debug)
			DisplayFps();
		audioEngine.Update();

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (!gameStateManager.GetStatus() || !AESysDoesWindowExist())
			gGameRunning = 0;
	}

	FreeUnitMeshes();
	audioEngine.ReleaseSound(sound);
	audioEngine.Exit();

	//free font
	AEGfxDestroyFont(fontId);

	//free the system
	AESysExit();
}