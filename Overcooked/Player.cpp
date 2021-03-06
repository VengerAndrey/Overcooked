#include "Player.h"

Player::Player(sf::Texture& texture, float _x, float _y, std::list<TiledObject*>& _objects) :
	TiledObject(texture, _x, _y, 0, 0), 
	objects(_objects)
{
	state = States::STAY;

	xSpeed = 0;
	ySpeed = 0;

	inHands = nullptr;

	type = ObjectTypes::PLAYER;
}

Player::~Player()
{
	delete inHands;
}

void Player::update(float time, Map* map)
{
	trackControls();

	switch (state)
	{
	case Player::States::LEFT:
		sprite.setRotation(90);
		if (map->at((getNextPosition().first + TILE_SIZE / 2) / TILE_SIZE, getNextPosition().second / TILE_SIZE) == MapObjects::FLOOR)
			xSpeed = -0.1;
		else
			xSpeed = 0;
		ySpeed = 0;
		break;
	case Player::States::UP:
		sprite.setRotation(180);
		if (map->at(getNextPosition().first / TILE_SIZE, (getNextPosition().second + TILE_SIZE / 2) / TILE_SIZE) == MapObjects::FLOOR)
			ySpeed = -0.1;
		else
			ySpeed = 0;
		xSpeed = 0;
		break;
	case Player::States::RIGHT:
		sprite.setRotation(270);
		if (map->at((getNextPosition().first - TILE_SIZE / 2) / TILE_SIZE, getNextPosition().second / TILE_SIZE) == MapObjects::FLOOR)
			xSpeed = 0.1;
		else
			xSpeed = 0;
		ySpeed = 0;
		break;
	case Player::States::DOWN:
		sprite.setRotation(0);
		if (map->at(getNextPosition().first / TILE_SIZE, (getNextPosition().second - TILE_SIZE / 2) / TILE_SIZE) == MapObjects::FLOOR)
			ySpeed = 0.1;
		else
			ySpeed = 0;
		xSpeed = 0;
		break;
	case Player::States::STAY:
	case Player::States::NEED_UPDATE:
		xSpeed = ySpeed = 0;
	default:
		break;
	}

	x += xSpeed * time;
	y += ySpeed * time;

	sprite.setPosition(x, y);

	if (isSomethingInHands())
	{
		switch (state)
		{
		case Player::States::LEFT:
			inHands->setPosition(x - inHands->getTileRect().width, y);
			break;
		case Player::States::UP:
			inHands->setPosition(x, y - inHands->getTileRect().height);
			break;
		case Player::States::RIGHT:
			inHands->setPosition(x + inHands->getTileRect().width, y);
			break;
		case Player::States::DOWN:
			inHands->setPosition(x, y + inHands->getTileRect().height);
			break;
		case Player::States::NEED_UPDATE:
		{
			switch (getRotation())
			{
			case Rotations::LEFT:
				inHands->setPosition(x - inHands->getTileRect().width, y);
				break;
			case Rotations::UP:
				inHands->setPosition(x, y - inHands->getTileRect().height);
				break;
			case Rotations::RIGHT:
				inHands->setPosition(x + inHands->getTileRect().width, y);
				break;
			case Rotations::DOWN:
				inHands->setPosition(x, y + inHands->getTileRect().height);
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	state = States::STAY;
}

void Player::trackControls()
{
	if (sf::Keyboard::isKeyPressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			state = States::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			state = States::UP;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			state = States::RIGHT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			state = States::DOWN;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			state = States::NEED_UPDATE;
		}
	}
}

sf::Sprite Player::getSprite()
{
	return sprite;
}

bool Player::isSomethingInHands()
{
	return (inHands) ? true : false;
}

sf::Sprite Player::getInHandsSprite()
{
	if (inHands)
		return inHands->getSprite();
	else
		return sf::Sprite();
}

void Player::take()
{
	int playerXTile;
	int playerYTile;
	int objXTile;
	int objYTile;
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		playerXTile = x / TILE_SIZE;
		playerYTile = y / TILE_SIZE;
		objXTile = (*it)->getSprite().getPosition().x / TILE_SIZE;
		objYTile = (*it)->getSprite().getPosition().y / TILE_SIZE;
		if ((getRotation() == Rotations::LEFT && playerXTile == objXTile + 1 && playerYTile == objYTile) ||
			(getRotation() == Rotations::UP && playerXTile == objXTile && playerYTile == objYTile + 1) ||
			(getRotation() == Rotations::RIGHT && playerXTile == objXTile - 1 && playerYTile == objYTile) ||
			(getRotation() == Rotations::DOWN && playerXTile == objXTile && playerYTile == objYTile - 1) ||
			playerXTile == objXTile && playerYTile == objYTile)
		{
			delete inHands;
			inHands = *it;
			objects.erase(it);
			break;
		}
	}
}

void Player::put()
{
	sf::Vector2f pos(convertToMapPosition(inHands->getSprite().getPosition()).x, convertToMapPosition(inHands->getSprite().getPosition()).y);
	pos.x *= TILE_SIZE;
	pos.x += TILE_SIZE / 2;
	pos.y *= TILE_SIZE;
	pos.y += TILE_SIZE / 2;
	for (auto obj : objects)
	{
		if (obj->getSprite().getPosition() == pos)
		{
			if (obj->getType() == ObjectTypes::UTENSIL)
			{
				Utensil* utensil = (Utensil*)obj;
				if (inHands->getType() == ObjectTypes::UTENSIL)
				{
					Utensil* inHandsUtensil = (Utensil*)inHands;
					inHandsUtensil->moveToUtensil(utensil);
					return;
				}
				else
				{
					if (utensil->add(inHands))
						inHands = nullptr;
				}
				return;
			}
			else
				return;
		}
	}
	inHands->setPosition(pos.x, pos.y);
	objects.push_back(inHands);
	inHands = nullptr;
}

std::pair<int, int> Player::getNextPosition()
{
	switch (getRotation())
	{
	case Rotations::LEFT:
		return std::make_pair(x - TILE_SIZE, y);
	case Rotations::UP:
		return std::make_pair(x, y - TILE_SIZE);
	case Rotations::RIGHT:
		return std::make_pair(x + TILE_SIZE, y);
	case Rotations::DOWN:
		return std::make_pair(x, (int)y + TILE_SIZE);
	default:
		break;
	}
}

void Player::draw(sf::RenderWindow& rw)
{
	TiledObject::draw(rw);
	if (isSomethingInHands())
	{
		inHands->draw(rw);
	}
}
