#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Dinamita.h"
#include "Game.h"

enum DinamitaAnim
{
	IDLE
};

void Dinamita::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spritesheet.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 4);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.5f));
	fall = 1;
	sprite->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDin.x), float(tileMapDispl.y + posDin.y)));
}

void Dinamita::update(int deltaTime)
{
	posDin.y += fall;
	if (map->collisionMoveDown(posDin, glm::ivec2(16, 16), &posDin.y))
	{
		fall = 0;
	}
	else fall = 1;
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDin.x), float(tileMapDispl.y + posDin.y)));
}

void Dinamita::render()
{
	sprite->render();
}

void Dinamita::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Dinamita::setPosition(const glm::vec2& pos)
{
	posDin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDin.x), float(tileMapDispl.y + posDin.y)));
}

void Dinamita::setAnimation(int anim)
{
	sprite->changeAnimation(anim);
}

glm::ivec2 Dinamita::getPosition()
{
	return posDin;
}