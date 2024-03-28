#ifndef _INTERFACE_INCLUDE
#define _INTERFACE_INCLUDE


#include <glm/glm.hpp>
#include "TimeText.h"
#include "SimpleText.h"

class Interface
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	int getLife();
	void updateLife(int newLife);
	void updateStage(int newStage);
	int getScore();
	void setScore(int amount);
	vector<int> textReader(const string& txt); //retuns a vector with the sprite characters idx

private:
	
	TimeText* timeText[4];
	TimeText* timeDisplay[3];
	TimeText* lifeDisplay[4];
	SimpleText* playerTextDisplay[8];
	SimpleText* scoreDisplay[6];
	SimpleText* insertCoinDisplay[11];
	SimpleText* stageDisplay[7];
	int time, frames, life, stage, score;
	
	vector<SimpleText*> textDisplay;

};


#endif // _INTERFACE_INCLUDE

