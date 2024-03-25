#include "Interface.h"

using namespace std;

#define POS_TIME_X 264
#define POS_TIME_Y 16

#define POS_LIFE_X 16 
#define POS_LIFE_Y 212

void Interface::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	for (int i = 0; i < 4; ++i) 
	{
		timeText[i] = new TimeText;
		timeText[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_TIME_X + i * 16, POS_TIME_Y));
		timeText[i]->update(16, 10 + i);
	}
	for (int i = 0; i < 3; ++i)
	{
		timeDisplay[i] = new TimeText;
		timeDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_TIME_X + i * 16 + 64, POS_TIME_Y));
		timeDisplay[i]->update(16, 0);
	}
	for (int i = 0; i < 4; ++i)
	{
		lifeDisplay[i] = new TimeText;
		lifeDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_LIFE_X + i * 20, POS_LIFE_Y));
		lifeDisplay[i]->update(16, 14);
	}
	time = 100;
	life = 4;
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

void Interface::updateLife(int newLife) {
	life = newLife;
}

void Interface::render()
{
	for (int i = 0; i < 4; ++i) {
		timeText[i]->render();
	}
	for (int i = 0; i < 3; ++i) {
		timeDisplay[i]->render();
	}
	for (int i = 0; i < life; ++i) {
		lifeDisplay[i]->render();
	}
}