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
	void updateScore(int addScore);
	void reset(int lvl);
	int getScore();
	int getTime() { return time; }

private:
	vector<int> textReader(const string& txt); //retuns a vector with the sprite characters idx

private:
	
	TimeText* timeText[4];
	TimeText* timeDisplay[3];
	TimeText* lifeDisplay[4];
	SimpleText* playerTextDisplay[8];
	SimpleText* scoreDisplay[6];
	SimpleText* insertCoinDisplay[11];
	SimpleText* stageDisplay[7];
	int time   = 100; 
	int frames = 0; 
	int life   = 4; 
	int stage  = 1; 
	int score  = 0;

};


#endif // _INTERFACE_INCLUDE

