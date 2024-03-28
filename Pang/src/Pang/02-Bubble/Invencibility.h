#pragma once
#ifndef _INVENCIBILITY_INCLUDE
#define _INVENCIBILITY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Invencibility
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
	void setApliedTrue();
	void setApliedFalse();
	void setGetInvTrue();
	void setGetInvFalse();
	bool getAplied();
	bool getGetInv();

private:
	glm::ivec2 tileMapDispl, posInv;
	int fall;
	Texture spritesheetIdle;
	Texture spritesheetAura;
	Sprite* spriteIdle;
	Sprite* spriteAura;
	Player* player;
	TileMap* map;
	bool aplied, getInv;
};

#endif // _INVENCIBILITY_INCLUDE
