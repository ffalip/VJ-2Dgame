#ifndef _INTERFACE_INCLUDE
#define _INTERFACE_INCLUDE


#include <glm/glm.hpp>
#include "TimeText.h"

class Interface
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void updateLife(int newLife);

private:
	
	TimeText* timeText[4];
	TimeText* timeDisplay[3];
	TimeText* lifeDisplay[4];
	int time, frames, life;

};


#endif // _INTERFACE_INCLUDE

