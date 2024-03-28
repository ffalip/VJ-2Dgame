#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Food.h"
#include "Game.h"

enum anim
{
	BANANA
};

void Food::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spritesheet.loadFromFile("images/food.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f/12.f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(BANANA, 8);
	sprite->addKeyframe(BANANA, glm::vec2(2.f/12.f, 0.f));

	fall = 1;
	sprite->changeAnimation(BANANA);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFd.x), float(tileMapDispl.y + posFd.y)));
}

void Food::update(int deltaTime)
{
	posFd.y += fall;
	if (map->collisionMoveDown(posFd, glm::ivec2(16, 16), &posFd.y))
	{
		fall = 0;
	}

	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFd.x), float(tileMapDispl.y + posFd.y)));

}

void Food::render()
{
	sprite->render();
}

void Food::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Food::setPosition(const glm::vec2& pos)
{
	posFd = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFd.x), float(tileMapDispl.y + posFd.y)));

}

void Food::setAnimation(int anim)
{
	sprite->changeAnimation(anim);
}


glm::ivec2 Food::getPosition()
{
	return posFd;
}