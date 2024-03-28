#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Bubble
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int anim, float vel, int startYini);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition();
	int getSize();
	float getVelocity();
	int getStartY();
	bool collisionWithBullet(const glm::ivec2& posBullet, int heightBullet, int widthBullet);
	bool Bubble::collisionWithPlayer(const glm::ivec2& posBullet, int heightPlayer, int widthPlayer);
	bool freeze;
private:
	glm::ivec2 tileMapDispl, posBubble;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int  startY, direccio;
	float velocity, jumpAngle;
	bool canJump;
};

#endif // _BUBBLE_INCLUDE