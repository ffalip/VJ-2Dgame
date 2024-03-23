#ifndef _BACKGROUND_INCLUDE
#define _BACKGROUND_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class Background
{
private:
	Background(const string& bgFile, const glm::vec2& minCoords, ShaderProgram& program);
public:
	static Background* createBackground(const string& bgFile, const glm::vec2& minCoords, ShaderProgram& program);

	~Background();

	void render() const;

private:
	void loadBg(const string& bgFile);
private:
	GLuint vao, vbo;
	GLint posLocation, texCoordLocation;
	Texture bgTexture;


};



#endif //_BACKGROUND_INCLUDE