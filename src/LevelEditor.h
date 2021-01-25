#pragma once
#include "State.h"
#include "GameStateManager.h"
#include "Sprite.h"

namespace StarBangBang
{
	class LevelEditor : public State
	{
	public:
		LevelEditor(GameStateManager* manager, int id);
		void Load();
		void Init();
		void Update();
		void Draw();
		void Free();
		void Unload();
	private:
		float tileWidth, tileHeight;
		std::map<std::string, Sprite> palette;
		std::vector<std::vector<AEVec2>> grid;
		GameStateManager* gameStateManager;
	};
}