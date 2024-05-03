#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "SimpleText.h"

using namespace std;

void SimpleText::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::ivec2& pos)
{
	font.loadFromFile("images/textFont.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textSprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f/26.f, 1/3.f), &font, &shaderProgram);
	textSprite->setNumberAnimations(65);

	//letters 0 - 25 MAJ
	for (int i = 0; i < 26; ++i)
	{
		textSprite->setAnimationSpeed(i, 1);
		textSprite->addKeyframe(i, glm::vec2(i / 26.f, 1.f / 3.f));
	}
	//letters 26 - 51 min
	for (int i = 26; i < 52; ++i)
	{
		textSprite->setAnimationSpeed(i, 1);
		textSprite->addKeyframe(i, glm::vec2(i / 26.f, 2.f/3.f));
	}
	
	//numbers 53 - 62
	for (int i = 52; i < 62; ++i) 
	{
		textSprite->setAnimationSpeed(i, 1);
		textSprite->addKeyframe(i, glm::vec2(i / 26.f, 0.f));
	}

	//space
	textSprite->setAnimationSpeed(62, 1);
	textSprite->addKeyframe(62, glm::vec2(25.f / 26.f, 0.f));
	//colon
	textSprite->setAnimationSpeed(63, 1);
	textSprite->addKeyframe(63, glm::vec2(10.f / 26.f, 0.f));
	//guion
	textSprite->setAnimationSpeed(64, 1);
	textSprite->addKeyframe(64, glm::vec2(11.f / 26.f, 0.f));

	textSprite->changeAnimation(62);
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

