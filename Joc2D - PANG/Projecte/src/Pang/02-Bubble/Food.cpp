#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Food.h"
#include "Game.h"

enum anim
{
	BANANA, WATERMELON, GRAPES, PICKLE
};

void Food::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int anim) {

	spritesheet.loadFromFile("images/food.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f/12.f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(BANANA, 1);
	sprite->addKeyframe(BANANA, glm::vec2(2.f/12.f, 0.f));

	sprite->setAnimationSpeed(WATERMELON, 1);
	sprite->addKeyframe(WATERMELON, glm::vec2(4.f / 12.f, 0.f));

	sprite->setAnimationSpeed(GRAPES, 1);
	sprite->addKeyframe(GRAPES, glm::vec2(6.f / 12.f, 0.f));

	sprite->setAnimationSpeed(PICKLE, 1);
	sprite->addKeyframe(PICKLE, glm::vec2(0.f / 12.f, 0.25f));

	fall = 1;
	sprite->changeAnimation(anim);
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
	else fall = 1;
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

int Food::getAnim() {
	if (sprite->animation() == BANANA) return 0;
	else if (sprite->animation() == WATERMELON) return 1;
	else if (sprite->animation() == GRAPES) return 2;
	else if (sprite->animation() == PICKLE) return 3;
}

glm::ivec2 Food::getPosition()
{
	return posFd;
}