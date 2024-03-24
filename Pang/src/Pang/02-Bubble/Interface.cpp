#include "Interface.h"

using namespace std;

#define POS_X 264
#define POS_Y 16

void Interface::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	for (int i = 0; i < 4; ++i) 
	{
		timeText[i] = new TimeText;
		timeText[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_X + i * 16, POS_Y));
		timeText[i]->update(16, 10 + i);
	}
	for (int i = 0; i < 3; ++i)
	{
		timeDisplay[i] = new TimeText;
		timeDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_X + i * 16 + 64, POS_Y));
		timeDisplay[i]->update(16, 0);
	}
	time = 100;
	frames = 0;
}

void Interface::update(int deltaTime) 
{
	int t0 = time / 100;
	int t1 = (time / 10) % 10;
	int t2 = time % 10;
	
	timeDisplay[0]->update(16, t0);
	timeDisplay[1]->update(16, t1);
	timeDisplay[2]->update(16, t2);

	if (frames == 60 && time > 0) {
		frames = 0;
		time--;
	}
	else ++frames;

}

void Interface::render()
{
	for (int i = 0; i < 4; ++i) {
		timeText[i]->render();
	}
	for (int i = 0; i < 3; ++i) {
		timeDisplay[i]->render();
	}
}