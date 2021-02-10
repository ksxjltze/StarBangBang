#include "LevelEditor.h"
#include "BasicMeshShape.h"
#include "Utils.h"
#include <iostream>
#include <fstream>

#include "PrimaryMovementController.h"

namespace StarBangBang
{
	//typedef std::pair<std::string, Sprite> TileImage;
	//typedef std::pair<Node*, GameObject*> Tile;

	void HighLightGridNode(Grid& grid)
	{
		AEVec2 mousePos = GetMouseWorldPos();
		Node* n = grid.GetNodeFromPosition(mousePos);
		if (n)
			DrawCircle(grid.GetNodeSize() / 2, n->nodePos);
	}

	LevelEditor::LevelEditor(int id, GameStateManager& manager) : Scene(id, manager)
	{
		tileWidth = 50; tileHeight = 50;
	}

	void LevelEditor::Load()
	{
		Sprite grassSprite = graphicsManager.CreateSprite(Constants::PROTOTYPE_SPRITE_GRASS_PATH, tileWidth, tileHeight);
		Sprite stoneSprite = graphicsManager.CreateSprite(Constants::PROTOTYPE_SPRITE_STONE_PATH, tileWidth, tileHeight);

		auto grassTile = std::pair<std::string, TileSprite>("Grass", { 1, "Grass", grassSprite });
		auto stoneTile = std::pair<std::string, TileSprite>("Stone", { 2, "Stone", stoneSprite });

		palette.insert(grassTile);
		palette.insert(stoneTile);
	}

	void LevelEditor::Init()
	{
		GameObject* obj = objectManager.NewGameObject();
		GameObject* obj2 = objectManager.NewGameObject();

		objectManager.AddImage(obj, palette.at("Grass").sprite);
		objectManager.AddImage(obj2, palette.at("Stone").sprite);

		obj2->transform.position.x += tileWidth;
		grid.CreateGrid(tileWidth, 20, 20);

		selectedTile = palette.at("Grass");

		camera = objectManager.NewGameObject();
		objectManager.AddComponent<CameraComponent>(camera);
		objectManager.AddComponent<PrimaryMovementController>(camera);

	}

	void LevelEditor::Update()
	{
		Scene::Update();

		if (AEInputCheckTriggered(AEVK_1))
		{
			selectedTile = palette.at("Grass");
		}
		else if (AEInputCheckTriggered(AEVK_2))
		{
			selectedTile = palette.at("Stone");
		}

		if (AEInputCheckTriggered(AEVK_RETURN))
		{
			SaveLevel();
		}

		//Insert/Replace/Remove Tile.
		AEVec2 mousePos = GetMouseWorldPos();
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			Node* n = grid.GetNodeFromPosition(mousePos);
			if (n)
			{

				if (!n->occupied)
				{
					InsertTile(n);
				}
				else
				{
					RemoveTile(n);
					InsertTile(n);
				}
			}
		}

		if (AEInputCheckTriggered(AEVK_RBUTTON))
		{
			Node* n = grid.GetNodeFromPosition(mousePos);
			if (n->occupied)
			{
				RemoveTile(n);
			}
		}
	}

	void LevelEditor::InsertTile(Node* node)
	{
		GameObject* obj = objectManager.NewGameObject();
		obj->SetPos(node->nodePos);

		Tile tile{ selectedTile , obj };

		objectManager.AddImage(obj, selectedTile.sprite);
		tileObjects.insert(std::pair<Node*, Tile>(node, tile));
		node->occupied = true;
		std::cout << "LevelEditor: TILE INSERTED" << std::endl;
	}

	void LevelEditor::RemoveTile(Node* n)
	{
		std::cout << "LevelEditor: NODE OCCUPIED" << std::endl;
		objectManager.DestroyGameObject(tileObjects.at(n).gameObject);
		tileObjects.erase(n);
		n->occupied = false;
	}

	void LevelEditor::SaveLevel()
	{
		std::fstream outputStream;
		outputStream.open("../Resources/Levels/test.txt", std::fstream::out);

		outputStream << "TEST";

		//for (auto tile : tileObjects)
		//{
		//	
		//}

		outputStream.close();
	}

	void LevelEditor::Draw()
	{
		Scene::Draw();
		HighLightGridNode(grid);
		grid.DrawGrid();
	}

	void LevelEditor::Free()
	{
		Scene::Free();
		grid.FreeGrid();
	}

	void LevelEditor::Unload()
	{
		Scene::Unload();
	}
}

