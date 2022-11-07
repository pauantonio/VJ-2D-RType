#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "TitleScreen.h"
#define SCREEN_X 256
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 8

Scene::Scene()
{
	player = NULL;
	map = NULL;
	enemies.clear();
	locations.clear();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (enemies.size() != 0)
		enemies.clear();
	if (locations.size() != 0)
		locations.clear();
}


void Scene::init()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(4096.f, 256.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	initShaders();
	background = "images/level.png";
	texs.loadFromFile(background, TEXTURE_PIXEL_FORMAT_RGBA);
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(0, 0), texProgram);
	size_x = map->mapSizeX();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setTileMap(map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	currentCamPos = 0.0f;
	win = false;
	loadEnemies();
	spritesheetHeart.loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteHeart = Sprite::createSprite(glm::vec2(8, 8), glm::vec2(1, 1), &spritesheetHeart, &texProgram);
}

void Scene::update(int deltaTime)
{
	if (player->getPlayerLife() <= 0 || win) return;

	currentTime += deltaTime;

	// avançar camera
	bool cameraMovement = currentCamPos + CAMERA_WIDTH - 1 < size_x;
	if (cameraMovement) currentCamPos += deltaTime / 20.0f;
	projection = glm::ortho(currentCamPos, float(CAMERA_WIDTH - 1) + currentCamPos, float(CAMERA_HEIGHT - 1), 0.f);
	player->update(deltaTime, currentCamPos);
	updateEnemies(deltaTime, currentCamPos);

	// enemies colisions and bullets
	for (int j = 0; j < enemies.size(); j++) {
		pair<int, int> posEnemy = enemies[j]->getPos();
		pair<int, int> posPlayer = player->getPos();
		pair<int, int> sizeEnemy = enemies[j]->getSize();

		if ((inRange(posPlayer.first, posPlayer.first + 32, posEnemy.first) || inRange(posPlayer.first, posPlayer.first + 32, posEnemy.first + sizeEnemy.first)) 
			&& (inRange(posPlayer.second, posPlayer.second + 16, posEnemy.second) || inRange(posPlayer.second, posPlayer.second + 16, posEnemy.second + sizeEnemy.second))) {
			if (enemies[j]->getType() == TIER_1) player->addPowerUp("tier1");
			else if (enemies[j]->getType() == TIER_2) player->addPowerUp("tier2");
			else player->getHit("enemy");

			if (enemies[j]->getType() != BOSS) {
				enemies.erase(enemies.begin() + j);
				--j;
			}
			break;
		}

		vector<Projectile*> enemyBullets = enemies[j]->getBullets();
		for (int i = 0; i < enemyBullets.size(); i++) {
			if (enemyBullets[i]->getState() == "destroyed") break;
			glm::ivec2 posProjectile = enemyBullets[i]->getPos();

			if ((inRange(posPlayer.first, posPlayer.first + 32, posProjectile.x) || inRange(posPlayer.first, posPlayer.first + 32, posProjectile.x + 8)) 
				&& (inRange(posPlayer.second, posPlayer.second + 16, posProjectile.y) || inRange(posPlayer.second, posPlayer.second + 16, posProjectile.y + 8))) {
				player->getHit(enemyBullets[i]->getType());
				enemyBullets[i]->setState("destroyed");
			}
		}
	}

	// player bullets
	playerBullets = player->getBullets();
	for (int i = 0; i < playerBullets.size(); i++) {
		if (playerBullets[i]->getState() == "destroyed") break;
		glm::ivec2 posProjectile = playerBullets[i]->getPos();
		for (int j = 0; j < enemies.size(); j++) {
			if (playerBullets[i]->getState() == "destroyed" || enemies[j]->getType() == TIER_1 || enemies[j]->getType() == TIER_2) break;
			pair<int, int> posEnemy = enemies[j]->getPos();
			pair<int, int> sizeEnemy = enemies[j]->getSize();

			if ((inRange(posEnemy.first, posEnemy.first + sizeEnemy.first, posProjectile.x) || inRange(posEnemy.first, posEnemy.first + sizeEnemy.first, posProjectile.x + 8)) 
				&& (inRange(posEnemy.second, posEnemy.second + sizeEnemy.second, posProjectile.y) || inRange(posEnemy.second, posEnemy.second + sizeEnemy.second, posProjectile.y + 8))) {
				enemies[j]->getHit(playerBullets[i]->getType());
				if (enemies[j]->getLife() < 0) {
					if (enemies[j]->getType() == BOSS) win = true;
					enemies.erase(enemies.begin() + j);
					--j;
				}
				if (playerBullets[i]->getType() != "charged" || enemies[j]->getType() == BOSS) playerBullets[i]->setState("destroyed");
			}
		}
	}
}

bool Scene::inRange(int low, int high, int x)
{
	return (low <= x && x <= high);
}

void Scene::render()
{
	glm::mat4 modelview;
	texProgram.use();
	if (player->getPlayerLife() <= 0 || win) {
		projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	}
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	if (player->getPlayerLife() <= 0 || win) {
		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(384.f, 256.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		initShaders();
		if (win) background = "images/win.png";
		else background = "images/loose.png";
		texs.loadFromFile(background, TEXTURE_PIXEL_FORMAT_RGBA);
		texQuad->render(texs);
		return;
	}
	texQuad->render(texs);
	map->render();
	player->render();
	renderEnemies(currentCamPos);

	for (int i = 0; i < player->getPlayerLife(); i++) {
		
		spriteHeart->setPosition(glm::vec2(currentCamPos + (0.5 + i) * map->getTileSize(), 15.25 * map->getTileSize()));
		spriteHeart->render();
	}
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


void Scene::godMode(bool active)
{
	player->setInvicibility(active);
}

void Scene::forceMode()
{
	player->addForceCheats();
}

void Scene::teleport(int value)
{
	currentCamPos = (size_x - CAMERA_WIDTH) / 5 * value;
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	update(0);
}

void Scene::loadEnemies()
{
	enemies.clear();
	int tileSize = map->getTileSize();

	// Spaceships
	for (int i = 15; i < 95; i += 3)
	{
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, SPACESHIP);
		enemy->setTileMap(map);
		enemy->setPosition(glm::vec2(i * tileSize, (rand() % 10 + 1) * tileSize));
		enemies.push_back(enemy);
	}

	// Robots
	locations.clear();
	locations = { 4, 8, 26, 32, 40, 60, 80 };
	for (int j = 0; j < locations.size(); j++)
	{
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, ROBOT);
		enemy->setTileMap(map);
		enemy->setPosition(glm::vec2(locations[j] * tileSize, 12 * tileSize));
		enemies.push_back(enemy);
	}

	// Bugs
	locations.clear();
	locations = { 90, 130, 172 };
	for (int h = 0; h < locations.size(); h++)
	{
		for (int k = 0; k < 10; k++) {
			Enemy* enemy = new Enemy();
			enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, BUG);
			enemy->setTileMap(map);
			enemy->setPosition(glm::vec2(locations[h] * tileSize, (-k) * 2 * tileSize));
			enemies.push_back(enemy);
		}
	}
	
	locations.clear();
	locations = { 110, 210 };
	for (int h = 0; h < locations.size(); h++)
	{
		for (int k = 0; k < 5; k++) {
			Enemy* enemy = new Enemy();
			enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, BUG);
			enemy->setTileMap(map);
			enemy->setPosition(glm::vec2((locations[h] + k * 2) * tileSize, (SCREEN_Y / 2 - 1) * tileSize));
			enemies.push_back(enemy);
		}
	}

	// Turrets
	locations.clear();
	locations = { 117, 177, 229 };
	for (int t = 0; t < locations.size(); t++)
	{
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, TURRET_TOP);
		enemy->setTileMap(map);
		enemy->setPosition(glm::vec2(locations[t] * tileSize + tileSize / 2, 3 * tileSize));
		enemies.push_back(enemy);

		Enemy* enemy2 = new Enemy();
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, TURRET_DOWN);
		enemy2->setTileMap(map);
		enemy2->setPosition(glm::vec2(locations[t] * tileSize + tileSize / 2, 11 * tileSize));
		enemies.push_back(enemy2);
	}

	locations.clear();
	locations = { 113, 205, 225 };
	for (int t = 0; t < locations.size(); t++)
	{
		Enemy* enemy = new Enemy();
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, TURRET_TOP);
		enemy->setTileMap(map);
		enemy->setPosition(glm::vec2(locations[t] * tileSize + tileSize / 2, 2 * tileSize));
		enemies.push_back(enemy);

		Enemy* enemy2 = new Enemy();
		enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, TURRET_DOWN);
		enemy2->setTileMap(map);
		enemy2->setPosition(glm::vec2(locations[t] * tileSize + tileSize / 2, 12 * tileSize));
		enemies.push_back(enemy2);
	}

	// Boss
	Enemy* enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, BOSS);
	enemy->setTileMap(map);
	enemy->setPosition(glm::vec2(246 * tileSize, tileSize));
	enemies.push_back(enemy);

	// Force Tier 1
	Enemy* tier1 = new Enemy();
	tier1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, TIER_1);
	tier1->setTileMap(map);
	tier1->setPosition(glm::vec2(100 * tileSize, (SCREEN_Y / 2 - 1) * tileSize));
	enemies.push_back(tier1);

	// Force Tier 2
	Enemy* tier2 = new Enemy();
	tier2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, TIER_2);
	tier2->setTileMap(map);
	tier2->setPosition(glm::vec2(150 * tileSize, (SCREEN_Y / 2 - 1) * tileSize));
	enemies.push_back(tier2);
}

void Scene::renderEnemies(float& currentCamPos)
{
	for (Enemy* e : enemies)
	{
		e->render(currentCamPos);
	}
}

void Scene::updateEnemies(int deltaTime, float& currentCamPos)
{
	for (Enemy* e : enemies)
	{
		e->update(deltaTime, currentCamPos);
	}
}
