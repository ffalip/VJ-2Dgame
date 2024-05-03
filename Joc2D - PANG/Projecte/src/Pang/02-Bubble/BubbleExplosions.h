#pragma once
#ifndef _BUBBLEEXPLOSIONS_INCLUDE
#define _BUBBLEEXPLOSIONS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class BubbleExplosions
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();
	void BubbleExplosions::setAnimation(int anim);

private:
	glm::ivec2 tileMapDispl, posBubbleExplosion;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif // _BUBBLEEXPLOSIONS_INCLUDE