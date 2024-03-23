#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 0
#define JUMP_HEIGHT 0
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, FIRE_RIGHT, FIRE_LEFT, CLIMB_UP, CLIMB_DOWN, TOP_STAIRS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/PlayerInGame.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1.f/6.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.4f, 1.f / 6.f));

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 5.f / 6.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 4.f/6.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f, 4.f / 6.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 4.f / 6.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3f, 4.f / 6.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 4.f / 6.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(FIRE_RIGHT, 1);
	sprite->addKeyframe(FIRE_RIGHT, glm::vec2(0.0f, 2.f / 6.f));

	sprite->setAnimationSpeed(FIRE_LEFT, 1);
	sprite->addKeyframe(FIRE_LEFT, glm::vec2(0.1f, 5.f / 6.f));
	/*
	sprite->setAnimationSpeed(CLIMB_UP, 8);
	sprite->setAnimationSpeed(CLIMB_UP, glm::vec2(0.1f, 4.f / 6.f));

	sprite->setAnimationSpeed(CLIMB_DOWN, 8);
	sprite->setAnimationSpeed(CLIMB_DOWN, glm::vec2(0.1f, 3.f / 6.f));*/

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	fire_cooldown = 0;
	shooting = 0;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_S) && fire_cooldown == 0)
	{
		if (sprite->animation() != FIRE_RIGHT && sprite->animation() != FIRE_LEFT)
		{
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
				sprite->changeAnimation(FIRE_RIGHT);
			}
			else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
				sprite->changeAnimation(FIRE_LEFT);
			}
			fire_cooldown = 30; //30 = 0.5s
			shooting = 6;
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_LEFT) && (sprite->animation() != FIRE_LEFT) && (sprite->animation() != FIRE_RIGHT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT) && (sprite->animation() != FIRE_LEFT) && (sprite->animation() != FIRE_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == FIRE_RIGHT && shooting != 0)
			sprite->changeAnimation(FIRE_RIGHT);
		else if (sprite->animation() == FIRE_LEFT && shooting != 0)
			sprite->changeAnimation(FIRE_LEFT);
		else if ((sprite->animation() == FIRE_RIGHT && shooting == 0) || sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
		else if ((sprite->animation() == FIRE_LEFT && shooting == 0) || sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}

	}
	
	if (fire_cooldown > 0) --fire_cooldown;
	if (shooting > 0) --shooting;
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPos() 
{
	return posPlayer;
}




