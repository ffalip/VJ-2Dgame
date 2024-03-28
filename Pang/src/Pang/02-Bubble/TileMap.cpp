#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;

#define OFFSET 4

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	needToUpdate = false;
}

void TileMap::updateArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
	if (mapStairs != NULL)
		delete mapStairs;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);

	tilesheet.use();

	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	if (layers > 1) {
		glBindVertexArray(vaoS);
		glEnableVertexAttribArray(posLocationStairs);
		glEnableVertexAttribArray(texCoordLocationStairs);
		glDrawArrays(GL_TRIANGLES, 0, 6 * nTilesSt);
	}

	glDisable(GL_TEXTURE_2D);
	
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vboS);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	string tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y >> layers;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	mapStairs = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i < mapSize.x; ++i) {
			getline(sstream, tile, ',');
			map[j * mapSize.x + i] = stoi(tile);
			
		}
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	cout << layers;
	if (layers > 1) {
		
		for (int j = 0; j < mapSize.y; j++)
		{
			getline(fin, line);
			sstream.str(line);
			for (int i = 0; i < mapSize.x; ++i) {
				getline(sstream, tile, ',');
				mapStairs[j * mapSize.x + i] = stoi(tile);
			}
#ifndef _WIN32
			fin.get(tile);
#endif
		}
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, tileStairs;
	glm::vec2 posTile, texCoordTile[2], halfTexel, posTileSt, texCoordTileSt[2];
	vector<float> vertices;
	vector<float> verticesSt;
	
	nTiles = 0;
	nTilesSt = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			tileStairs = mapStairs[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
			if (tileStairs != 0)
			{
				++nTilesSt;
				// Same but for stairs
				posTileSt = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTileSt[0] = glm::vec2(float((tileStairs - 1) % tilesheetSize.x) / tilesheetSize.x, float((tileStairs - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTileSt[1] = texCoordTileSt[0] + tileTexSize;
				texCoordTileSt[1] -= halfTexel;
				// First triangle
				verticesSt.push_back(posTileSt.x); verticesSt.push_back(posTileSt.y);
				verticesSt.push_back(texCoordTileSt[0].x); verticesSt.push_back(texCoordTileSt[0].y);
				verticesSt.push_back(posTileSt.x + blockSize); verticesSt.push_back(posTileSt.y);
				verticesSt.push_back(texCoordTileSt[1].x); verticesSt.push_back(texCoordTileSt[0].y);
				verticesSt.push_back(posTileSt.x + blockSize); verticesSt.push_back(posTileSt.y + blockSize);
				verticesSt.push_back(texCoordTileSt[1].x); verticesSt.push_back(texCoordTileSt[1].y);
				// Second triangle
				verticesSt.push_back(posTileSt.x); verticesSt.push_back(posTileSt.y);
				verticesSt.push_back(texCoordTileSt[0].x); verticesSt.push_back(texCoordTileSt[0].y);
				verticesSt.push_back(posTileSt.x + blockSize); verticesSt.push_back(posTileSt.y + blockSize);
				verticesSt.push_back(texCoordTileSt[1].x); verticesSt.push_back(texCoordTileSt[1].y);
				verticesSt.push_back(posTileSt.x); verticesSt.push_back(posTileSt.y + blockSize);
				verticesSt.push_back(texCoordTileSt[0].x); verticesSt.push_back(texCoordTileSt[1].y);
				
			}
		}
	}
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenVertexArrays(1, &vaoS);
	glBindVertexArray(vaoS);
	glGenBuffers(1, &vboS);
	glBindBuffer(GL_ARRAY_BUFFER, vboS);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTilesSt * sizeof(float), &verticesSt[0], GL_STATIC_DRAW);
	posLocationStairs = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocationStairs = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + OFFSET) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveLeftStairs(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + OFFSET) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (mapStairs[y * mapSize.x + x] != 0) {
			return true;
		}
			
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - OFFSET - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRightStairs(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = ((pos.x + size.x - OFFSET - 1) / tileSize) - 1;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (mapStairs[y * mapSize.x + x] != 0) {
			return true;
		}
			
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = (pos.x + OFFSET) / tileSize;
	x1 = (pos.x + size.x - OFFSET - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveDownStairs(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = (pos.x + OFFSET) / tileSize;
	x1 = (pos.x + size.x - OFFSET - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (mapStairs[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUpStairs(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = (pos.x + OFFSET) / tileSize;
	x1 = (pos.x + size.x - OFFSET - 1) / tileSize;
	y = pos.y / tileSize; // Cambio aquí para detectar colisiones desde arriba

	for (int x = x0; x <= x1; x++)
	{
		if (mapStairs[y * mapSize.x + x] != 0)
		{
			if (*posY + size.y - tileSize * y <= 4) // Cambio aquí para detectar colisiones desde arriba
			{
				*posY = tileSize * (y + 1); // Ajustar la posición para evitar la colisión
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionBullet(const glm::ivec2& pos, const glm::ivec2& size, int b2c) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize + 1;
	x1 = (pos.x + size.x - 1) / tileSize + 1;
	y = (pos.y/tileSize) - b2c-1;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			int tile = map[y * mapSize.x + x];
			if (tile >= 15 && tile <= 18) {
				int left = tile - 15;
				int right = 18 - tile;
				for (int i = 0; i <= left + right; ++i)
				{
					map[y * mapSize.x + (x - left + i)] = 0;
				}
				map[y * mapSize.x + x] = 0;
			}
			if (tile >= 10 && tile <= 12) {
				int left = tile - 10;
				int right = 12 - tile;
				for (int i = 0; i <= left + right; ++i)
				{
					map[y * mapSize.x + (x - left + i)] = 0;
				}
				map[y * mapSize.x + x] = 0;
			}
			return true;
		}
	}
	return false;
}

int TileMap::circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {

	// temporary variables to set edges for testing
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)         testX = rx;      // test left edge
	else if (cx > rx + rw) testX = rx + rw;   // right edge
	if (cy < ry)         testY = ry;      // bottom edge
	else if (cy > ry + rh) testY = ry + rh;   // top edge

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		// Determine the collision direction
		if (distX <= 0 && distY <= 0) {
			if (distX <= distY) return 0;
			else return 2;
		}

		else if (distX >= 0 && distY >= 0) {
			if (distX >= distY) return 1;
			else return 3;
		}


		else if (distX >= 0 && distY <= 0) {
			if (distX >= -(distY)) return 1;
			else return 2;
		}

		else if (distX <= 0 && distY >= 0) {
			if (distY <= -(distX)) return 0;
			else return 3;
		}
	}
	return -1; // No collision
}

int TileMap::circleCollisionWithMap(float cx, float cy, float radius)
{
	int tile;
	// Iterate through each tile in the map
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			// Calculate the coordinates of the rectangle corresponding to this tile
			if (tile != 0) {
				float rx = i * tileSize;
				float ry = j * tileSize;
				float rw = tileSize;
				float rh = tileSize;

				// Check for collision between the circle and the rectangle
				int collisionDirection = circleRect(cx, cy, radius, rx, ry, rw, rh);
				if (collisionDirection != -1)
				{
					return collisionDirection; // Collision detected
				}
			}
		}
	}
	return -1; // No collision detected
}
