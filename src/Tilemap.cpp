#include "Tilemap.h"

void TempTeam::Tilemap::Init()
{
	tileWidth = 100;
	tileHeight = 100;
	tileImage.Init("../Resources/grass.png", tileWidth, tileHeight, 255);
	transform.position.x -= AEGetWindowWidth() / 2;
	transform.position.y -= AEGetWindowHeight() / 2 - tileHeight / 4;

	for (int i = 0; i < 1; i++)
	{
		std::vector<Tile> row;
		for (int j = 0; j < 20; j++)
		{
			Tile tile;
			AEVec2 pos;
			pos.x = j;
			pos.y = i;

			tile.Init(&tileImage, pos);
			row.push_back(tile);
		}
		tiles.push_back(row);
	}
}

void TempTeam::Tilemap::Update()
{

}

void TempTeam::Tilemap::Draw()
{
	for (std::vector<Tile> row : tiles)
	{
		for (Tile tile : row)
		{
			tile.Draw(transform.position, tileWidth, tileHeight);
		}
	}
}

void TempTeam::Tilemap::Exit()
{
}
