#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see lvl1.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render();
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveLeftStairs(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRightStairs(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionBullet(const glm::ivec2& pos, const glm::ivec2& size, int b2c);
	bool collisionMoveRightCircle(const glm::ivec2& pos, float radius) const;
	bool collisionMoveLeftCircle(const glm::ivec2& pos, float radius) const;
	bool collisionMoveDownCircle(const glm::ivec2& pos, float radius, int* posY) const;
	bool collisionMoveTopCircle(const glm::ivec2& pos, float radius, int* posY) const;
	bool collisionMoveDownStairs(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUpStairs(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	int circleCollisionWithMap(float cx, float cy, float radius);
	int circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh);
	void updateArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao, vaoS;
	GLuint vbo, vboS;
	GLint posLocation, texCoordLocation, posLocationStairs, texCoordLocationStairs;
	ShaderProgram texProgram;
	int nTiles, nTilesSt, layers;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map, *mapStairs;
	bool needToUpdate, visibleDt;

	Texture spritesheet;
	Sprite* sprite;
	int anim;
};


#endif // _TILE_MAP_INCLUDE


