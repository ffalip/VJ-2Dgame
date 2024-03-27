#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "PararTemps.h"
#include "Game.h"

enum BubbleAnimsExplosions
{
	IDLE
};

void PararTemps::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spritesheet.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.875f, 0.5f));

	fall = 1;
	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDin.x), float(tileMapDispl.y + posDin.y)));
}

void PararTemps::update(int deltaTime)
{
	posDin.y += fall;
	if (map->collisionMoveDown(posDin, glm::ivec2(16, 16), &posDin.y))
	{
		fall = 0;
	}
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDin.x), float(tileMapDispl.y + posDin.y)));
}

void PararTemps::render()
{
	sprite->render();
}

void PararTemps::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void PararTemps::setPosition(const glm::vec2& pos)
{
	posDin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDin.x), float(tileMapDispl.y + posDin.y)));
}

void PararTemps::setAnimation(int anim)
{
	sprite->changeAnimation(anim);
}

glm::ivec2 PararTemps::getPosition()
{
	return posDin;
}