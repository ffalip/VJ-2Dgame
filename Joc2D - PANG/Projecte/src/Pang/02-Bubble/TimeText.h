#ifndef _TIMETEXT_INCLUDE
#define _TIMETEXT_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"

class TimeText
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& pos);
	void update(int deltaTime, int anim);
	void render();

private:
	Texture font;
	Sprite* textSprite;
	glm::ivec2 tileMapDispl, posTime;

};

#endif //_SIMPLETEXT_INCLUDE