#include "TiledObject.h"

TiledObject::TiledObject(Texture& texture, float _x, float _y, IntRect _tileRect)
{
	sprite.setTexture(texture);

	tileRect = _tileRect;

	sprite.setTextureRect(tileRect);

	sprite.setOrigin(tileRect.width / 2, tileRect.height / 2);

	x = _x;
	y = _y;
	sprite.setPosition(x, y);
}

Sprite TiledObject::getSprite()
{
	return sprite;
}

void TiledObject::setPosition(float _x, float _y)
{
	sprite.setPosition(_x, _y);
}

IntRect TiledObject::getTileRect()
{
	return tileRect;
}

Rotations TiledObject::getRotation()
{
	std::cout << (int)sprite.getRotation() << "\n";
	switch ((int)sprite.getRotation())
	{
	case 90:
		return Rotations::LEFT;
	case 180:
		return Rotations::UP;
	case 270:
		return Rotations::RIGHT;
	case 0:
		return Rotations::DOWN;
	default:
		break;
	}
}