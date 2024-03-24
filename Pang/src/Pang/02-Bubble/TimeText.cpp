#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "TimeText.h"

using namespace std;

enum TimeTextFrames
{
	T = 10, I = 11, M = 12, E = 13
};

void TimeText::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& pos)
{
	font.loadFromFile("images/timeFont.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 0.5f), &font, &shaderProgram);
	textSprite->setNumberAnimations(14);
	
	//Number frames
	for (int i = 0; i < 10; ++i) {
		textSprite->setAnimationSpeed(i, 8);
		textSprite->addKeyframe(i, glm::vec2(i/10.f, 0.f));
	}
	//Time title
	textSprite->setAnimationSpeed(T, 8);
	textSprite->addKeyframe(T, glm::vec2( 0.f, 0.5f));

	textSprite->setAnimationSpeed(I, 8);
	textSprite->addKeyframe(I, glm::vec2(0.1f, 0.5f));

	textSprite->setAnimationSpeed(M, 8);
	textSprite->addKeyframe(M, glm::vec2(0.2f, 0.5f));

	textSprite->setAnimationSpeed(E, 8);
	textSprite->addKeyframe(E, glm::vec2(0.3f, 0.5f));

	posTime = pos;
	textSprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	textSprite->setPosition(glm::vec2(float(tileMapDispl.x + posTime.x), float(tileMapDispl.y + posTime.y)));
}

void TimeText::update(int deltaTime, int anim)
{
	textSprite->update(deltaTime);
	if (textSprite->animation() != anim)
		textSprite->changeAnimation(anim);
}

void TimeText::render() {
	textSprite->render();
}
