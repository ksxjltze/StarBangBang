#pragma once
#include "State.h"
#include "ObjectManager.h"
#include "GraphicsManager.h"
#include "MemoryManager.h"
#include "GameStateManager.h"
#include "TileMap.h"

namespace StarBangBang
{
	class Level_Demo : public Scene
	{
	public:
		Level_Demo(int id, GameStateManager& manager);
		void Load();
		void Init();
		void Update();
		void Draw();
		void Free();
		void Unload();
	private:
		//player stuffs
		Sprite playerImage;

		Sprite playerImageL1;
		Sprite playerImageL2;
		Sprite playerImageL3;

		Sprite playerImageR1;
		Sprite playerImageR2;
		Sprite playerImageR3;

		//player 2 stuffs
		Sprite player2Image;

		Sprite playerImage2L1;
		Sprite playerImage2L2;
		Sprite playerImage2L3;

		Sprite playerImage2R1;
		Sprite playerImage2R2;
		Sprite playerImage2R3;

		//Indicator
		Sprite indicator;

		Sprite guardImage;
		Sprite securityCamImage;
		Sprite exitImage;

		GameObject* movementController;
		GameObject* player;
		GameObject* player2;
		GameObject* guardManager;
		GameObject* indicatorObj;

		TileMap tilemap;
	};
}