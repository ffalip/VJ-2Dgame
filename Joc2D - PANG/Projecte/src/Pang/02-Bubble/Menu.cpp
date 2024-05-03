#include "Menu.h"


enum WindowMenu
{
	START_MENU, CONTROLS, CREDITS
};

void Menu::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	menuTex.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	controlsTex.loadFromFile("images/controlsMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsTex.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);

	menu = Sprite::createSprite(glm::ivec2(384, 208), glm::vec2(0.5f, 1.f), &menuTex, &shaderProgram);
	menu->setNumberAnimations(1);
	menu->setAnimationSpeed(0, 1);
	menu->addKeyframe(0, glm::vec2(0.f, 0.f));
	menu->addKeyframe(0, glm::vec2(0.5f, 0.f));
	menu->changeAnimation(0);
	menu->setPosition(glm::vec2(tileMapPos.x, tileMapPos.y));

	controls = Sprite::createSprite(glm::ivec2(384, 208), glm::vec2(1.f, 1.f / 3.f), &controlsTex, &shaderProgram);
	controls->setNumberAnimations(1);
	controls->setAnimationSpeed(0, 8);
	controls->addKeyframe(0, glm::vec2(0.f, 0.f));
	controls->addKeyframe(0, glm::vec2(0.0f, 1.f / 3.f));
	controls->addKeyframe(0, glm::vec2(0.0f, 2.f / 3.f));
	controls->changeAnimation(0);
	controls->setPosition(glm::vec2(tileMapPos.x, tileMapPos.y));

	credits = Sprite::createSprite(glm::ivec2(384, 208), glm::vec2(1.f, 1.f), &creditsTex, &shaderProgram);
	credits->setNumberAnimations(1);
	credits->setAnimationSpeed(0, 0);
	credits->addKeyframe(0, glm::vec2(0.f, 0.f));
	credits->changeAnimation(0);
	credits->setPosition(glm::vec2(tileMapPos.x, tileMapPos.y));

	actual = 0;
}

void Menu::update(int deltTime)
{
	menu->update(deltTime);
	controls->update(deltTime);
	credits->update(deltTime);
}


void Menu::render() 
{
	if (actual == START_MENU) 
	{
		menu->render();
	}
	else if (actual == CONTROLS) 
	{
		controls->render();
	}
	else if (actual == CREDITS) 
	{
		credits->render();
	}
}