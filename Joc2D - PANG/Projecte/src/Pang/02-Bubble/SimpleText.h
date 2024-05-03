#ifndef _SIMPLETEXT_INCLUDE
#define _SIMPLETEXT_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"

class SimpleText
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& pos);
	void update(int deltaTime, int anim);
	void render();

private:
	Texture font;
	Sprite* textSprite;
	glm::ivec2 tileMapDispl, posText;

};

#endif //_SIMPLETEXT_INCLUDE