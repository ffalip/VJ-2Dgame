#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Background.h"

using namespace std;

#define BG_SIZE_X 384
#define BG_SIZE_Y 208

Background::Background(const string& bgFile, const glm::vec2& minCoords, ShaderProgram& program) 
{
	loadBg(bgFile);
	//quad of the size of the map
	//glm::vec2 texCoord = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
	vector<float> vertices;
	vertices.push_back(minCoords.x); vertices.push_back(minCoords.y);
	vertices.push_back(0); vertices.push_back(0);

	vertices.push_back(minCoords.x + BG_SIZE_X); vertices.push_back(minCoords.y);
	vertices.push_back(1); vertices.push_back(0);

	vertices.push_back(minCoords.x); vertices.push_back(minCoords.y + BG_SIZE_Y);
	vertices.push_back(0); vertices.push_back(1);

	vertices.push_back(minCoords.x + BG_SIZE_X); vertices.push_back(minCoords.y);
	vertices.push_back(1); vertices.push_back(0);

	vertices.push_back(minCoords.x); vertices.push_back(minCoords.y + BG_SIZE_Y);
	vertices.push_back(0); vertices.push_back(1);

	vertices.push_back(minCoords.x + BG_SIZE_X); vertices.push_back(minCoords.y + BG_SIZE_Y);
	vertices.push_back(1); vertices.push_back(1);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

Background* Background::createBackground(const string& bgFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	Background* bg = new Background(bgFile, minCoords, program);
	return bg;
}


void Background::render() const
{
	glEnable(GL_TEXTURE_2D);
	bgTexture.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Background::loadBg(const string& bgFile) 
{
	bgTexture.loadFromFile(bgFile, TEXTURE_PIXEL_FORMAT_RGBA);
	bgTexture.setWrapS(GL_CLAMP_TO_EDGE);
	bgTexture.setWrapT(GL_CLAMP_TO_EDGE);
	bgTexture.setMinFilter(GL_NEAREST);
	bgTexture.setMagFilter(GL_NEAREST);
}