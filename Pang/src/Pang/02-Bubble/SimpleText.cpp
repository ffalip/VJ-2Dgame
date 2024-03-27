#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "SimpleText.h"

using namespace std;

void SimpleText::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& pos)
{
	font.loadFromFile("images/textFont.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f/26.f, 0.5f), &font, &shaderProgram);
	textSprite->setNumberAnimations(36);

	//letters 0 - 25
	for (int i = 0; i < 26; ++i)
	{
		textSprite->setAnimationSpeed(i, 1);
		textSprite->addKeyframe(i, glm::vec2(i / 26.f, 0.5f));
	}
	//numbers 26 - 35
	for (int i = 26; i < 36; ++i) 
	{
		textSprite->setAnimationSpeed(i, 1);
		textSprite->addKeyframe(i, glm::vec2(i / 26.f, 0.f));
	}
	
	textSprite->changeAnimation(0);
	posText = pos;
	tileMapDispl = tileMapPos;
	textSprite->setPosition(glm::vec2(float(tileMapDispl.x + posText.x), float(tileMapDispl.y + posText.y)));
}

void SimpleText::update(int deltaTime, int c) 
{
	if (textSprite->animation() != c)
		textSprite->changeAnimation(c);
}

void SimpleText::render() 
{
	textSprite->render();
}

