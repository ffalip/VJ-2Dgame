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
	for (int i = 0; i <= 70; ++i) {
		float x = (1.f / 71.f) * i;
		sprite->addKeyframe(SHOOTING, glm::vec2(0.f+x, 0.f));
	}
	sprite->setAnimationSpeed(IDLE, 1);
	sprite->addKeyframe(IDLE, glm::vec2(70.f/71.f, 0.f));
	
	sprite->changeAnimation(IDLE);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x + 1), 24.f));
	canShoot = true;
	fireCooldown = 0;
}

void Bullet::update(int deltaTime) 
{
	sprite->update(deltaTime);
	if (Game::instance().getKey(GLFW_KEY_S) && canShoot && fireCooldown == 0)
	{
		if (sprite->animation() != SHOOTING)
		{
			posBullet = player->getPos();
			sprite->changeAnimation(SHOOTING);
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x + 28), float(tileMapDispl.y + posBullet.y + 24)-168 ));
			canShoot = false;
			fireCooldown = 30;
		}
			
	}
	else if (canShoot) 
		sprite->changeAnimation(IDLE);

	int current_frame = sprite->getFrame();
	int block2collide = (current_frame / 7) * 2;
	if (current_frame % 7 >= 3) block2collide += 1;

	//if (sprite->animation() == SHOOTING) cout << current_frame << " " << block2collide << endl;
	if (sprite->animation() == SHOOTING && map->collisionBullet(posBullet, glm::ivec2(16, 192), block2collide)) {
		sprite->changeAnimation(IDLE);
		posBullet = player->getPos();
		canShoot = true;
	}

	if (sprite->animation() == IDLE) {
		posBullet = player->getPos();
	}
	if (fireCooldown > 0) --fireCooldown;
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

glm::vec2 Bullet::getPos()
{
	return posBullet;
}

int Bullet::getHeight()
{
	float x = sprite->getFrame() / 70.f;
	int h = x * 168;
	return h;
}

bool Bullet::shooting()
{
	if (sprite->animation() == SHOOTING) return true;
	else return false;
}

void Bullet::stopShooting()
{
	sprite->changeAnimation(IDLE);
	canShoot = true;
}