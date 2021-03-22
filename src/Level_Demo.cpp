#include "Level_Demo.h"
#include "CameraComponent.h"

#include "InteractableComponent.h"
#include "Guard.h"
#include "GuardManager.h"
#include "SecurityCamera.h"
#include "Detector.h"

#include "MovementManager.h"
#include "DetectionListener.h"
#include "CollisionListener.h"
#include "MessageBus.h"

#include <iostream>
#include "constants.h"

#include "ObserverTest.h"
#include "Text.h"
#include "globals.h"

StarBangBang::Level_Demo::Level_Demo(int id, GameStateManager& manager) : Scene(id, manager), tilemap{ objectManager, graphicsManager }
{
	player = nullptr;
	player2 = nullptr;
	movementController = nullptr;
	guardManager = nullptr;
}

void StarBangBang::Level_Demo::Load()
{
	playerImage = graphicsManager.CreateSprite(RESOURCES::CAPTAINSTEALTH_F1_PATH);
	player2Image = graphicsManager.CreateSprite(RESOURCES::PRISONER_F1_PATH);
	guardImage = graphicsManager.CreateSprite(RESOURCES::SECURITYGUARD_F1_PATH);
	securityCamImage = graphicsManager.CreateSprite(RESOURCES::CAMERA_PATH);
}

//Initialization of game objects, components and scripts.
void StarBangBang::Level_Demo::Init()
{
	PathFinder::ShowGrid(true);
	GRAPHICS::SetBackgroundColor(Black);

	//Load tilemap
	tilemap.SetCollidableTypes({ TileType::BRICK_BLACK });
	tilemap.Load(RESOURCES::LEVELS::LEVEL_TEST_PATH);

	tilemap.SetGrid(PathFinder::GetWorldGrid());
	//tilemap.SetVisible(false);

	//Movement controller
	movementController = objectManager.NewGameObject();
	MovementManager& moveMgr = objectManager.AddComponent<MovementManager>(movementController);

	//Player components and scripts
	player = objectManager.NewGameObject();
	player->SetPos({ 250, 800 });
	player->transform.scale = { 0.7f, 0.7f };

	DetectionListener* listener = &objectManager.AddComponent<DetectionListener>(player);
	MessageBus::RegisterListener(listener);

	objectManager.AddImage(player, playerImage);
	objectManager.AddComponent<Text>(player).fontID = StarBangBang::fontId;
	objectManager.AddComponent<CameraComponent>(player);
	objectManager.AddComponent<RigidBody>(player);
	objectManager.AddCollider(player, false);
	objectManager.AddComponent<PrimaryMovementController>(player);
	objectManager.AddComponent<CollisionListener>(player);

	//Player 2
	player2 = objectManager.NewGameObject();
	player2->SetPos({ 250, 950 });
	player2->transform.scale = { 0.7f, 0.7f };

	objectManager.AddImage(player2, player2Image);
	objectManager.AddComponent<RigidBody>(player2);
	objectManager.AddCollider(player2, false).isTrigger = true;
	objectManager.AddComponent<PrimaryMovementController>(player2);

	//Movement Manager
	moveMgr.AddController(player);
	moveMgr.AddController(player2);

	//Guards and security cameras
	guardManager = objectManager.NewGameObject();
	GuardManager& mgr = objectManager.AddComponent<GuardManager>(guardManager);
	mgr.Init(&objectManager, &guardImage, player, player2);
	guardManager->GetComponent<GuardManager>()->CreateSecurityCameras(&objectManager, &securityCamImage, player, player2);

	// Call Start on scripts
	objectManager.Init();
}

void StarBangBang::Level_Demo::Update()
{
	Scene::Update();
	if (AEInputCheckTriggered(VK_SPACE))
	{
		gameStateManager.SetNextGameState(SceneID::EDITOR);
	}



}

void StarBangBang::Level_Demo::Draw()
{
	Scene::Draw();
}

void StarBangBang::Level_Demo::Free()
{
	Scene::Free();
}

void StarBangBang::Level_Demo::Unload()
{
	tilemap.Unload();
	Scene::Unload();
}