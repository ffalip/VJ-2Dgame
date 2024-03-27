#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Invencibility.h"
#include "Game.h"

enum anim
{
	IDLE, AURA
};

void Invencibility::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spritesheetIdle.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteIdle = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125f, 0.5f), &spritesheetIdle, &shaderProgram);
	spriteIdle->setNumberAnimations(1);

	spriteIdle->setAnimationSpeed(IDLE, 8);
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.125f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.375f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.625f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.75f, 0.f));
	spriteIdle->addKeyframe(IDLE, glm::vec2(0.875f, 0.f));

	fall = 1;
	spriteIdle->changeAnimation(IDLE);
	tileMapDispl = tileMapPos;
	spriteIdle->setPosition(glm::vec2(float(tileMapDispl.x + posInv.x), float(tileMapDispl.y + posInv.y)));


	spritesheetAura.loadFromFile("images/Invencibility.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteAura = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.5f, 1.0f), &spritesheetAura, &shaderProgram);
	spriteAura->setNumberAnimations(1);
	
	spriteAura->setAnimationSpeed(AURA, 2);
	spriteAura->addKeyframe(AURA, glm::vec2(0.f, 0.f));
	spriteAura->addKeyframe(AURA, glm::vec2(0.5f, 0.f));

	spriteAura->changeAnimation(AURA);
	aplied = true;
}

void Invencibility::update(int deltaTime)
{
	posInv.y += fall;
	if (map->collisionMoveDown(posInv, glm::ivec2(16, 16), &posInv.y))
	{
		fall = 0;
	}
	
	if (aplied) {
		spriteAura->setPosition(glm::vec2((player->getPos().x), (player->getPos().y)));
		spriteAura->update(deltaTime);
	}
	
	spriteIdle->update(deltaTime);
	spriteIdle->setPosition(glm::vec2(float(tileMapDispl.x + posInv.x), float(tileMapDispl.y + posInv.y)));
	
}

void Invencibility::render()
{
	if(aplied) spriteAura->render();
	spriteIdle->render();
}

void Invencibility::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Invencibility::setPosition(const glm::vec2& pos)
{
	posInv = pos;
	spriteIdle->setPosition(glm::vec2(float(tileMapDispl.x + posInv.x), float(tileMapDispl.y + posInv.y)));
	
}

void Invencibility::setAnimation(int anim)
{
	spriteIdle->changeAnimation(anim);
}

void Invencibility::setPlayer(Player* play)
{
	player = play;
	spriteAura->setPosition(glm::vec2((player->getPos().x ), (player->getPos().y)));
}

void Invencibility::setAplied() {
	aplied = true;
}

bool Invencibility::getAplied() {
	return aplied;
}

glm::ivec2 Invencibility::getPosition()
{
	return posInv;
}