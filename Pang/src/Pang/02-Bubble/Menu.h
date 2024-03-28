#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "Sprite.h"
#include "ShaderProgram.h"

class Menu
{
private:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void changeMenu(int m) { actual = m; };


public:
	Texture menuTex;
	Texture controlsTex;
	Texture creditsTex;
	Sprite* menu;
	Sprite* controls;
	Sprite* credits;
	glm::ivec2 tileMapDispl, posPlayer;
	int actual;
};


#endif // _MENU_INCLUDE