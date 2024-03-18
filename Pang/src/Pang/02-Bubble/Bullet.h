#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Bullet
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* p);
private:
	glm::ivec2 tileMapDispl, posBullet;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap* map;
};

#endif // _BULLET_INCLUDE
