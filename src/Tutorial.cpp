#include "Tutorial.h"
#include "Text.h"
#include "globals.h"
#include "CameraComponent.h"
#include "Click.h"

using namespace StarBangBang;

void Tutorial::NewTextObject(AEVec2 position, const std::string& s, float scale)
{
	objectManager.AddComponent<Text>(objectManager.NewGameObject(), s, fontId2, scale).gameObject->transform.position = position;
}

Tutorial::Tutorial(int id, GameStateManager& gsm) : Scene(id, gsm), tilemap{ objectManager, graphicsManager }
{
}

void Tutorial::Load()
{
	tilemap.Load(RESOURCES::LEVELS::LEVEL_TUTORIAL);
	movementSprite = graphicsManager.CreateSprite(RESOURCES::VENDING_LEFT_PATH);
	distractSprite = graphicsManager.CreateSprite(RESOURCES::VENDING_LEFT_PATH);
	backSprite     = graphicsManager.CreateSprite(RESOURCES::DOOR_PATH);
}

void Tutorial::Init()
{
	GameObject* obj = objectManager.NewGameObject();
	obj->transform.scale = { 0.00001f, 0.00001f };
	objectManager.AddImage(obj, graphicsManager.CreateSprite(RESOURCES::BIN_PATH));

	float spacing = 100.f;
	float offset = 100.f;

	if (GRAPHICS::IsFullscreen())
		offset = spacing;

	AEVec2 pos = { 0.0f, 120.0f };

	GRAPHICS::SetBackgroundColor(Black);

	NewTextObject({0, 120}, "Tutorial", 1.0f);

	ImageComponent* movementImg = objectManager.AddImage(objectManager.NewGameObject(), movementSprite);
	movementImg->gameObject->SetPos({-140 - offset, 50});
	NewTextObject({ -140 - offset, 0 }, "Use arrow keys to move", 0.3f);

	ImageComponent* tabImg = objectManager.AddImage(objectManager.NewGameObject(), movementSprite);
	tabImg->gameObject->SetPos({ 0, 50 });
	NewTextObject({ 0, 0 }, "Use [tab] to change characters", 0.3f);

	ImageComponent* distractImg = objectManager.AddImage(objectManager.NewGameObject(), distractSprite);
	distractImg->gameObject->SetPos({ 140 + offset, 50 });
	NewTextObject({ 140 + offset, 0 }, "Touch objects to distract guards", 0.3f);

	NewTextObject({ 0, -60 }, "Objective:", 0.5f);
	NewTextObject({ 0, -80 }, "Break out of the prison while avoiding guards and security cameras!", 0.3f);

	GameObject* backButton = objectManager.NewGameObject();
	objectManager.AddImage(backButton, backSprite);
	backButton->transform.position = { 0, -150 };
	backButton->transform.scale = { 1, 1 };
	objectManager.AddComponent<Click<Tutorial>>(backButton).setCallback(*this, &Tutorial::MainMenu);
}

void Tutorial::Update()
{
	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		gameStateManager.SetNextGameState(MAIN_MENU);
		return;
	}

	Scene::Update();
}

void Tutorial::Draw()
{
	Scene::Draw();
}

void Tutorial::Free()
{
	Scene::Free();
}

void Tutorial::Unload()
{
	Scene::Unload();
}

void Tutorial::MainMenu()
{
	gameStateManager.SetNextGameState(MAIN_MENU);
}
