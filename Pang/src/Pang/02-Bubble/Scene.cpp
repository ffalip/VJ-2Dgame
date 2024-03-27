#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 0


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/lvl2.txt", glm::vec2(16, 16), texProgram);
	bg = Background::createBackground("images/bg1.png", glm::vec2(16, 16), texProgram);

	for (int i = 0; i < 15; ++i) {
		bubble = new Bubble();
		
		if (i == 0) {
			bubble->init(glm::ivec2(16, 16), texProgram, 0, 1.f);
			bubble->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		else if (i > 0 && i <= 2) {
			bubble->init(glm::ivec2(16, 16), texProgram, 1, 1.f);
			bubble->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		else if (i >= 3 && i <= 6) {
			bubble->init(glm::ivec2(16, 16), texProgram, 2, 1.f);
			bubble->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		else {
			bubble->init(glm::ivec2(16, 16), texProgram, 3, 1.f);
			bubble->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		
		bubbles.push_back(bubble);
	}
	for (int i = 0; i < 15; ++i) {
		bubblesActives.push_back(false);
	}
	bubblesActives[0] = true;

	for (int i = 0; i < 15; ++i) {
		bubEx = new BubbleExplosions();
		bubEx->init(glm::ivec2(16, 16), texProgram);
		bubEx->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
		bubEx->setTileMap(map);
		bubExs.push_back(bubEx);
	}

	bullet = new Bullet();
	bullet->init(glm::ivec2(16, 16), texProgram);
	bullet->setTileMap(map);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	bullet->setPlayer(player);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	timeDisp = new Interface();
	timeDisp->init(glm::ivec2(16, 16), texProgram);
	
}

void Scene::update(int deltaTime)
{
	if (Game::instance().getKey(GLFW_KEY_X)) {
		peta(bubblesActives, bubbles, bubExs);
	}
	currentTime += deltaTime;
	for (int i = 0; i < 15; ++i) {
		if (bubblesActives[i]) bubbles[i]->update(deltaTime);
	}
	for (int i = 0; i < 15; ++i) {
		bubExs[i]->update(deltaTime);
	}
	bullet->update(deltaTime);
	player->update(deltaTime);
	timeDisp->update(deltaTime);

}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	bg->render();
	map->render();
	for (int i = 0; i < 15; ++i) {
		if (bubblesActives[i]) bubbles[i]->render();	
	}

	for (int i = 0; i < 15; ++i) {
		
		bubExs[i]->render();
	}

	bullet->render();
	player->render();
	timeDisp->render();

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::peta(vector<bool>& bubblesActives, vector<Bubble*>& bubbles, vector<BubbleExplosions*>& bubExs) {
	for (int i = 0; i < bubblesActives.size(); ++i) {
		if (bubblesActives[i]) {

			bubEx = new BubbleExplosions();
			bubEx->init(glm::ivec2(16, 16), texProgram);
			bubEx->setPosition(glm::ivec2(bubbles[i]->getPosition().x + 8, bubbles[i]->getPosition().y + 8));
			bubEx->setAnimation(bubbles[i]->getSize());
			bubEx->setTileMap(map);
			bubExs[i] = bubEx;

			bubblesActives[i] = false;

			

			bubExs[i]->setAnimation(bubbles[i]->getSize());
			if ((i + 1) * 2 < bubblesActives.size()) {
				bubble = new Bubble();
				bubble->init(glm::ivec2(16, 16), texProgram, bubbles[i]->getSize() + 1, bubbles[i]->getVelocity());
				bubble->setPosition(glm::ivec2(bubbles[i] -> getPosition().x + 4, bubbles[i]->getPosition().y));
				bubble->setTileMap(map);
				bubbles[(i + 1) * 2 - 1] = bubble;
				bubblesActives[(i + 1) * 2 - 1] = true;
				
				bubble = new Bubble();
				bubble->init(glm::ivec2(16, 16), texProgram, bubbles[i]->getSize() + 1, -(bubbles[i]->getVelocity()));
				bubble->setPosition(glm::ivec2(bubbles[i]->getPosition().x - 4, bubbles[i]->getPosition().y));
				bubble->setTileMap(map);
				bubbles[(i + 1) * 2] = bubble;
				bubblesActives[(i + 1) * 2] = true;
				break;
				
			}
			break;
		}
	}
}



