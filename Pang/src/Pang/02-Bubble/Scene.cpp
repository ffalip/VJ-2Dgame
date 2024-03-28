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

	map = TileMap::createTileMap("levels/lvl" + std::to_string(idLevel) + ".txt", glm::vec2(16, 16), texProgram);
	bg = Background::createBackground("images/bg1.png", glm::vec2(16, 16), texProgram);

	for (int i = 0; i < 15; ++i) {
		bubble = new Bubble();
		
		if (i == 0) {
			bubble->init(glm::ivec2(16, 16), texProgram, 0, 1.f, 130);
			bubble->setPosition(glm::vec2((20 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		else if (i > 0 && i <= 2) {
			bubble->init(glm::ivec2(16, 16), texProgram, 1, 1.f, 130);
			bubble->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		else if (i >= 3 && i <= 6) {
			bubble->init(glm::ivec2(16, 16), texProgram, 2, 1.f, 130);
			bubble->setPosition(glm::vec2((10 + i) * map->getTileSize(), 10 * map->getTileSize()));
			bubble->setTileMap(map);
		}
		else {
			bubble->init(glm::ivec2(16, 16), texProgram, 3, 1.f, 130);
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
	//inv->setPlayer(player);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	fd = new Food();
	fd->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	fd->setPosition(glm::vec2(40 * map->getTileSize(), 20 * map->getTileSize()));
	fd->setTileMap(map);
	bananaAct = true;
	timeDisp = new Interface();
	timeDisp->init(glm::ivec2(16, 16), texProgram);

	contadorFreeze = 0;
	activarContadorFreeze = invAct = ptAct = dinAct = invAplied = false;
}

void Scene::update(int deltaTime)
{
	if (timeDisp->getLife() <= 0) {
		perdut = true;
		//cout << "perdut" << endl;
	}
	if (!guanyat) {
		bool algunBubbleActive = false;
		for (int i = 0; i < bubblesActives.size(); ++i) {
			if (bubblesActives[i]) {
				algunBubbleActive = true;
			}
		}
		if (algunBubbleActive) guanyat = false;
		else {
			guanyat = true;
			cout << "guanyaaaaaat" << endl;
		} 
	}
	if (Game::instance().getKey(GLFW_KEY_X)) {
		peta(bubblesActives, bubbles, bubExs, 0);
	}

	if (dinAct && player->interseccio(player->getPos(), 32, 32, din->getPosition(), 16, 16)) {
		petaTot(bubblesActives, bubbles, bubExs);
		dinAct = false;
	}

	if (ptAct && player->interseccio(player->getPos(), 32, 32, pt->getPosition(), 16, 16)) {
		contadorFreeze = 0;
		activarContadorFreeze = true;
		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i]->freeze = true;
		}
		ptAct = false;
	}

	if (invAct  && inv->getGetInv() && player->interseccio(player->getPos(), 32, 32, inv->getPosition(), 16, 16)) {
		inv->setApliedTrue();
		inv->setGetInvFalse();
		invAplied = inv->getAplied();
	}

	if (bananaAct && player->interseccio(player->getPos(), 32, 32, fd->getPosition(), 16, 16)) {
		timeDisp->updateScore(50);
		bananaAct = false;
		cout << timeDisp->getScore() << endl;
	}

	if (activarContadorFreeze) {
		++contadorFreeze;
		cout << contadorFreeze << endl;
		if (contadorFreeze >= 300) {
			for (int i = 0; i < bubbles.size(); ++i) {
				bubbles[i]->freeze = false;
				activarContadorFreeze = false;
			}
		}
	}

	currentTime += deltaTime;
	for (int i = 0; i < 15; ++i) {
		if (bubblesActives[i]) {
			bubbles[i]->update(deltaTime);
			if (bullet->shooting() && bubbles[i]->collisionWithBullet(bullet->getPos(), bullet->getHeight(), 8)) {
				peta(bubblesActives, bubbles, bubExs, i);
				bullet->stopShooting();
				if (!dinAct && rand() % 3 == 0) {
					dinAct = true;
					din = new Dinamita();
					din->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					din->setPosition(glm::vec2(bubbles[i] -> getPosition().x +32, bubbles[i]->getPosition().y + 32));
					din->setTileMap(map);
				}
				if (!ptAct && rand() % 3 == 1) {
					ptAct = true;
					pt = new PararTemps();
					pt->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					pt->setPosition(glm::vec2(bubbles[i]->getPosition().x + 32, bubbles[i]->getPosition().y + 32));
					pt->setTileMap(map);
				}
				if (!invAct && rand() % 3 == 2) {
					invAct = true;
					inv = new Invencibility();
					inv->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					inv->setPosition(glm::vec2(bubbles[i]->getPosition().x + 32, bubbles[i]->getPosition().y + 32));
					inv->setTileMap(map);
					inv->setPlayer(player);
				}
			}
			if (bubbles[i]->collisionWithPlayer(player->getPos(), 32, 32)) {
				if (!invAplied) timeDisp->updateLife(timeDisp->getLife() - 1);
				else {
					invAplied = false;
					inv->setApliedFalse();
				}
			}
		}
	}

	for (int i = 0; i < 15; ++i) {
		bubExs[i]->update(deltaTime);
	}

	bullet->update(deltaTime);
	if (invAct) inv->update(deltaTime);
	player->update(deltaTime);
	timeDisp->update(deltaTime);
	if (dinAct) din->update(deltaTime);
	if (ptAct) pt->update(deltaTime);
	if(bananaAct)fd->update(deltaTime);
	
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
	map->updateArrays(glm::vec2(16, 16), texProgram);
	map->render();
	for (int i = 0; i < bubblesActives.size(); ++i) {
		if (bubblesActives[i]) bubbles[i]->render();	
	}

	for (int i = 0; i < bubExs.size(); ++i) {
		
		bubExs[i]->render();
	}

	bullet->render();
	if (invAct) inv->render();
	player->render();
	timeDisp->render();
	if (dinAct) din->render();
	if (ptAct) pt->render();
	if(bananaAct)fd->render();
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

void Scene::peta(vector<bool>& bubblesActives, vector<Bubble*>& bubbles, vector<BubbleExplosions*>& bubExs, int i) {
	//for (int i = 0; i < bubblesActives.size(); ++i) {
		if (bubblesActives[i]) {

			bubEx = new BubbleExplosions();
			bubEx->init(glm::ivec2(16, 16), texProgram);
			bubEx->setPosition(glm::ivec2(bubbles[i]->getPosition().x + 16, bubbles[i]->getPosition().y + 16));
			bubEx->setAnimation(bubbles[i]->getSize());
			bubEx->setTileMap(map);
			bubExs[i] = bubEx;

			bubblesActives[i] = false;

			

			bubExs[i]->setAnimation(bubbles[i]->getSize());
			if ((i + 1) * 2 < bubblesActives.size()) {
				bubble = new Bubble();
				bubble->init(glm::ivec2(16, 16), texProgram, bubbles[i]->getSize() + 1, bubbles[i]->getVelocity(), bubbles[i]->getStartY());
				bubble->setPosition(glm::ivec2(bubbles[i] -> getPosition().x + 4, bubbles[i]->getPosition().y));
				bubble->setTileMap(map);
				bubbles[(i + 1) * 2 - 1] = bubble;
				bubblesActives[(i + 1) * 2 - 1] = true;
				
				bubble = new Bubble();
				bubble->init(glm::ivec2(16, 16), texProgram, bubbles[i]->getSize() + 1, -(bubbles[i]->getVelocity()), bubbles[i]->getStartY());
				bubble->setPosition(glm::ivec2(bubbles[i]->getPosition().x - 4, bubbles[i]->getPosition().y));
				bubble->setTileMap(map);
				bubbles[(i + 1) * 2] = bubble;
				bubblesActives[(i + 1) * 2] = true;
				//break;
				
			}
			//break;
		}
	//}
}

void Scene::petaTot(vector<bool>& bubblesActives, vector<Bubble*>& bubbles, vector<BubbleExplosions*>& bubExs) {
	for (int i = 0; i < bubblesActives.size(); ++i) {
		if (bubblesActives[i] && bubbles[i]->getSize() != 3) {

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
				bubble->init(glm::ivec2(16, 16), texProgram, bubbles[i]->getSize() + 1, bubbles[i]->getVelocity(), bubbles[i]->getStartY()- 20);
				bubble->setPosition(glm::ivec2(bubbles[i]->getPosition().x + rand() % 20, bubbles[i]->getPosition().y));
				bubble->setTileMap(map);
				bubbles[(i + 1) * 2 - 1] = bubble;
				bubblesActives[(i + 1) * 2 - 1] = true;

				bubble = new Bubble();
				bubble->init(glm::ivec2(16, 16), texProgram, bubbles[i]->getSize() + 1, -(bubbles[i]->getVelocity()), bubbles[i]->getStartY()-20);
				bubble->setPosition(glm::ivec2(bubbles[i]->getPosition().x - rand()%20, bubbles[i]->getPosition().y));
				bubble->setTileMap(map);
				bubbles[(i + 1) * 2] = bubble;
				bubblesActives[(i + 1) * 2] = true;
				

			}
		}
	}
}



