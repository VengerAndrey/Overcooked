#include "Map.h"

Map::Map(Texture& texture) : width(10), height(10)
{
	sprite.setTexture(texture);

	tileMap = new MapObjects * [width];

	for (int i = 0; i < height; i++)
	{
		tileMap[i] = new MapObjects[width];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if(i == 0 || j == 0 || i == height - 1 || j == width - 1)
				tileMap[i][j] = MapObjects::TABLE;
			//if (i == 1 && j == 1)
				//tileMap[i][j] = MapObjects::PLAYER;
		}
	}
}

void Map::repaint(RenderWindow& window)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (tileMap[i][j] == MapObjects::TABLE)
			{
				sprite.setTextureRect(IntRect(64, 0, 32, 32));

				sprite.setPosition(j * 32, i * 32);

				window.draw(sprite);
			}
			
		}
	}
}
