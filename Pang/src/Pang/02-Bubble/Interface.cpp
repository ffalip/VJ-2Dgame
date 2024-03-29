#include <iostream>
#include "Interface.h"

using namespace std;

#define POS_TIME_X 260
#define POS_TIME_Y 14

#define POS_LIFE_X 16 
#define POS_LIFE_Y 220

#define POS_TEXT_X 16
#define POS_TEXT_Y 210

#define POS_SCORE_X 104
#define POS_SCORE_Y 210

#define POS_ICOIN_X 300
#define POS_ICOIN_Y 224

#define POS_STAGE_X 176
#define POS_STAGE_Y 224

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
	vector<int> chars1 = textReader("PLAYER-1");
	for (int i = 0; i < 8; ++i)
	{
		playerTextDisplay[i] = new SimpleText;
		playerTextDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_TEXT_X + i * 7, POS_TEXT_Y));
		playerTextDisplay[i]->update(16, chars1[i]);
	}
	vector<int> chars2 = textReader("000000");
	for (int i = 0; i < 6; ++i)
	{
		scoreDisplay[i] = new SimpleText;
		scoreDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_SCORE_X + i * 7, POS_SCORE_Y));
		scoreDisplay[i]->update(16, chars2[i]);
	}
	updateScore(score);
	vector<int> chars3 = textReader("INSERT COIN");
	for (int i = 0; i < 11; ++i)
	{
		insertCoinDisplay[i] = new SimpleText;
		insertCoinDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_ICOIN_X + i * 7, POS_ICOIN_Y));
		insertCoinDisplay[i]->update(16, chars3[i]);
	}
	vector<int> chars4 = textReader("STAGE-" + to_string(stage));
	for (int i = 0; i < 7; ++i)
	{
		stageDisplay[i] = new SimpleText;
		stageDisplay[i]->init(tileMapPos, shaderProgram, glm::ivec2(POS_STAGE_X + i * 7, POS_STAGE_Y));
		stageDisplay[i]->update(16, chars4[i]);
	}
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

void Interface::updateStage(int newStage) 
{
	stage = newStage;
	vector<int> chars4 = textReader("STAGE-" + to_string(stage));
	for (int i = 0; i < 7; ++i)
	{
		stageDisplay[i]->update(16, chars4[i]);
	}
}

int Interface::getLife() {
	return life;
}

void Interface::updateScore(int addScore)
{
	score += addScore;
	string int2str = to_string(score);
	string aux = "";

	if (int2str.length() - 1 < 6)
	{
		cout << (6 - int2str.length()) << " ";
		for (int i = 0; i < (6 - int2str.length()); ++i) {
			aux.append("0");
		}
	}
	else int2str = "999999";

	vector<int> chars = textReader(aux + int2str);
	for (int i = 0; i < 6; ++i)
	{
		scoreDisplay[i]->update(16, chars[i]);
	}

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
	for (int i = 0; i < 8; ++i) {
		playerTextDisplay[i]->render();
	}
	for (int i = 0; i < 6; ++i) {
		scoreDisplay[i]->render();
	}
	for (int i = 0; i < 7; ++i) {
		stageDisplay[i]->render();
	}
	if (time % 2 == 0) {
		for (int i = 0; i < 11; ++i) {
			insertCoinDisplay[i]->render();
		}
	}
}

int Interface::getScore() {
	return score;
}

void Interface::reset(int lvl) {
	stage = lvl;
	time = 100;
	frames = 0;
}

vector<int> Interface::textReader(const string& txt)
{
	vector<int> chars;
	for (int i = 0; i < txt.length(); ++i) {
		if (txt[i] == ' ')
			chars.push_back(62);
		else if (txt[i] == '-')
			chars.push_back(64);
		else if (txt[i] == ':')
			chars.push_back(63);
		else if (txt[i] >= 'a')
			chars.push_back(int(txt[i] - 'a') + 26);
		else if (txt[i] >= 'A')
			chars.push_back(int(txt[i] - 'A'));
		else
			chars.push_back(int(txt[i] - '0') + 52);
	}
	return chars;
}