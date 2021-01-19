#pragma once
#include "TileManager.h"
#include "GameStateManager.h"
#include "MemoryManager.h"

namespace StarBangBang
{
	class MapEditor : public State
	{
	public:
		MapEditor(GameStateManager* manager, int id);
		void Load();
		void Init();
		void Update();
		void Draw();
		void Free();
		void Unload();
	private:
		TileManager tileManager { &objectManager };
		GameObject* tileMap;

		GameStateManager* gameStateManager;
	};
}