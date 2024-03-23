#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Bubble
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, posBubble;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int jumpAngle, startY, velocity, direccio;
	bool canJump;
};

#endif // _BUBBLE_INCLUDE