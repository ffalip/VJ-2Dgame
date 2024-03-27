#pragma once
#ifndef _PARARTEMPS_INCLUDE
#define _PARARTEMPS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class PararTemps
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();
	void setAnimation(int anim);

private:
	glm::ivec2 tileMapDispl, posDin;
	int fall;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif // _PARARTEMPS_INCLUDE
