#pragma once
#ifndef _FOOD_INCLUDE
#define _FOOD_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Food
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();
	void setAnimation(int anim);
	void setPlayer(Player* play);
	void setAplied();
	bool getAplied();

private:
	glm::ivec2 tileMapDispl, posFd;
	int fall;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif // _FOOD_INCLUDE
#pragma once
