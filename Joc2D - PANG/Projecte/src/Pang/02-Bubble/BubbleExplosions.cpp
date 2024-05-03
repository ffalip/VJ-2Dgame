#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "BubbleExplosions.h"
#include "Game.h"

enum BubbleAnimsExplosions
{
	EXPLOTAR_GRAN, EXPLOTAR_MITJANA, EXPLOTAR_PETITA, EXPLOTAR_ENANA, IDLE
};

void BubbleExplosions::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	
	spritesheet.loadFromFile("images/Explosions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(EXPLOTAR_GRAN, 16);
	sprite->addKeyframe(EXPLOTAR_GRAN, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(EXPLOTAR_GRAN, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(EXPLOTAR_GRAN, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(EXPLOTAR_GRAN, glm::vec2(0.75f, 0.75f));

	sprite->setAnimationSpeed(EXPLOTAR_MITJANA, 16);
	sprite->addKeyframe(EXPLOTAR_MITJANA, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(EXPLOTAR_MITJANA, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(EXPLOTAR_MITJANA, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(EXPLOTAR_MITJANA, glm::vec2(0.75f, 0.5f));

	sprite->setAnimationSpeed(EXPLOTAR_PETITA, 16);
	sprite->addKeyframe(EXPLOTAR_PETITA, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(EXPLOTAR_PETITA, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(EXPLOTAR_PETITA, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(EXPLOTAR_PETITA, glm::vec2(0.75f, 0.25f));

	sprite->setAnimationSpeed(EXPLOTAR_ENANA, 16);
	sprite->addKeyframe(EXPLOTAR_ENANA, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(EXPLOTAR_ENANA, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(EXPLOTAR_ENANA, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(EXPLOTAR_ENANA, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(IDLE);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubbleExplosion.x), float(tileMapDispl.y + posBubbleExplosion.y)));
}

void BubbleExplosions::update(int deltaTime)
{
	if (sprite->animation() != IDLE) {
		if (sprite->getFrame() == 3) sprite->changeAnimation(IDLE);
	}
	sprite->update(deltaTime);
}

void BubbleExplosions::render()
{
	sprite->render();
}

void BubbleExplosions::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void BubbleExplosions::setPosition(const glm::vec2& pos)
{
	posBubbleExplosion = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubbleExplosion.x), float(tileMapDispl.y + posBubbleExplosion.y)));
}

void BubbleExplosions::setAnimation(int anim)
{
	sprite->changeAnimation(anim);
}

glm::ivec2 BubbleExplosions::getPosition()
{
	return posBubbleExplosion;
}