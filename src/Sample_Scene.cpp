#include "Sample_Scene.h"
#include "CollisionManager.h"
#include "MovementManager.h"
#include "Click.h"
#include "CameraComponent.h"
#include "constants.h"

void Testt()
{
	PRINT("TESTES\n");
}

StarBangBang::Sample_Scene::Sample_Scene(int id, GameStateManager& manager) : Scene(id, manager), gameObject{nullptr}
{
	
}

void StarBangBang::Sample_Scene::Load()
{
	image = graphicsManager.CreateSprite("./Resources/PlanetTexture.png");
	image2 = graphicsManager.CreateSprite("./Resources/boi.png");
}

void StarBangBang::Sample_Scene::Init()
{
	tilemap.Init();
	tilemap.SetCollidableTypes({ TileType::BRICK_BLACK });
	tilemap.Load(RESOURCES::LEVELS::COLLISION_TEST);

	GRAPHICS::SetBackgroundColor(SkyBlue);
	gameObject = objectManager.NewGameObject();

	//objectManager.AddImage(gameObject, image2);
	objectManager.AddComponent<ImageComponent>(gameObject, image2);
	objectManager.AddComponent<CameraComponent>(gameObject);
	objectManager.AddComponent<RigidBody>(gameObject);
	objectManager.AddComponent<PrimaryMovementController>(gameObject);
	objectManager.AddCollider(gameObject, false);

	//int nObjects = 8;
	//for (int i = 0; i < nObjects; i++)
	//{
	//	GameObject* obj = objectManager.NewGameObject();

	//	double rads = (2 * (double)PI / nObjects) * i;
	//	AEVec2& pos = obj->transform.position;

	//	pos.x = static_cast<float>(cos(rads) * 300);
	//	pos.y = static_cast<float>(sin(rads) * 300);

	//	objectManager.AddComponent<RigidBody>(obj).SetMass(0);
	//	objectManager.AddImage(obj, image);
	//	objectManager.AddCollider(obj, false);
	//	//objectManager.AddComponent<PrimaryMovementController>(obj);

	//}

	objectManager.Init();
}

void StarBangBang::Sample_Scene::Update()
{
	if (AEInputCheckTriggered(AEVK_SPACE))
	{
		gameStateManager.SetNextGameState(SceneID::DEMO);
	}
	Scene::Update();
}

void StarBangBang::Sample_Scene::Draw()
{
	Scene::Draw();
}

void StarBangBang::Sample_Scene::Free()
{
	Scene::Free();
}

void StarBangBang::Sample_Scene::Unload()
{
	tilemap.Unload();
	Scene::Unload();
	
}


