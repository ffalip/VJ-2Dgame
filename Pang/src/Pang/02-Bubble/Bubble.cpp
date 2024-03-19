#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 2
#define JUMP_HEIGHT 150
#define FALL_STEP 2

enum BubbleAnims
{
	GRAN48, MITJA32, PETITA16, ENANA8
};

void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Bubble.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(GRAN48, 1);
	sprite->addKeyframe(GRAN48, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(MITJA32, 1);
	sprite->addKeyframe(MITJA32, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(PETITA16, 1);
	sprite->addKeyframe(PETITA16, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(ENANA8, 1);
	sprite->addKeyframe(ENANA8, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
	velocity = 1;
	canJump = false;
}

void Bubble::update(int deltaTime)
{
	if (Game::instance().getKey(GLFW_KEY_0)) {
		sprite->changeAnimation(GRAN48);
	}
	if (Game::instance().getKey(GLFW_KEY_1)) {
		sprite->changeAnimation(MITJA32);
	}
	if (Game::instance().getKey(GLFW_KEY_2)) {
		sprite->changeAnimation(PETITA16);
	}
	if (Game::instance().getKey(GLFW_KEY_3)) {
		sprite->changeAnimation(ENANA8);
	}
	

	if (sprite->animation() == GRAN48 && canJump)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			
			posBubble.y = startY;
		}
		else
		{
			posBubble.y = int(startY - 130 * sin(3.14159f * jumpAngle / 180.f));
			posBubble.x += velocity;
			//Comprovar si choca amb paret
			if (map->collisionMoveRight(posBubble, glm::ivec2(50, 50))) {
				velocity = -(velocity);
			}
			if (map->collisionMoveLeft(posBubble, glm::ivec2(50, 50))) {
				velocity = -(velocity);
			}
			if (jumpAngle > 90)
				canJump = !map->collisionMoveDown(posBubble, glm::ivec2(48, 48), &posBubble.y);
		}
	}

	else
	{
		posBubble.y += FALL_STEP;
		if (map->collisionMoveDown(posBubble, glm::ivec2(48, 48), &posBubble.y))
		{
			canJump = true;
			jumpAngle = 0;
			startY = posBubble.y;
			
		}

	}

	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));

}

void Bubble::render()
{
	sprite->render();
}

void Bubble::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bubble::setPosition(const glm::vec2& pos)
{
	posBubble = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}