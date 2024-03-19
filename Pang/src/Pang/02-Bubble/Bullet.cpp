#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bullet.h"
#include "Game.h"

//constants i enums
enum BulletAnims
{
	IDLE, SHOOTING 
};

void Bullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 192), glm::vec2(1.f / 71.f,1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(SHOOTING, 64);
	for (int i = 0; i < 70; ++i) {
		float x = (1.f / 71.f) * i;
		sprite->addKeyframe(SHOOTING, glm::vec2(0.f+x, 0.f));
	}
	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(70.f/71.f, 0.f));
	
	sprite->changeAnimation(IDLE);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x + 1), 24.f));

}

void Bullet::update(int deltaTime) 
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_S))
	{
		if (sprite->animation() != SHOOTING)
		{
			sprite->changeAnimation(SHOOTING);
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x + 28), 24.f));
		}
			
	}
	else if (Game::instance().getKey(GLFW_KEY_A))
	{
		if (sprite->animation() != IDLE)
			sprite->changeAnimation(IDLE);
		
	}
	//else if (sprite->animation() != IDLE) 
		//sprite->changeAnimation(IDLE);
	int x = sprite->getFrame();
	if (sprite->animation() != IDLE && map->collisionBullet(posBullet, glm::ivec2(16, 32 + 2 * x))) {
		cout << x << " ";
		sprite->changeAnimation(IDLE);
	}

	if (sprite->animation() == IDLE) posBullet = player->getPos();
	
}

void Bullet::render()
{
	sprite->render();
}

void Bullet::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bullet::setPlayer(Player* p)
{
	player = p;
}