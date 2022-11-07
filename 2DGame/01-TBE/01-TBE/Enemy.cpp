#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"
#include "Scene.h"

Enemy::Enemy()
{
	map = NULL;
}

Enemy::~Enemy()
{
	//if (map != NULL) delete map;
}

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, EnemyType type) {
	this->type = type;
	timeCounter = 0;
	bullets = {};

	switch (type)
	{
		case SPACESHIP:
			life = 4;
			direction = glm::vec2(-1, -1);
			size = glm::vec2(32, 32);
			spritesheet.loadFromFile("images/spaceship.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
		case ROBOT:
			life = 5;
			direction = glm::vec2(-1, 0);
			size = glm::vec2(32, 32);
			spritesheet.loadFromFile("images/robot.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);
			/*
			sprite->setNumberAnimations(2);

			sprite->setAnimationSpeed(0, 8);
			sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

			sprite->setAnimationSpeed(1, 8);
			sprite->addKeyframe(1, glm::vec2(0.5f, 1.f));

			sprite->changeAnimation(1);*/
			break;
		case BUG:
			life = 2;
			//direction = glm::vec2(0, -1);
			size = glm::vec2(32, 32);
			spritesheet.loadFromFile("images/bug.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(size, glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
		case TURRET_TOP:
			life = 4;
			size = glm::vec2(16, 16);
			spritesheet.loadFromFile("images/turret_top.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(size, glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
		case TURRET_DOWN:
			life = 4;
			timeCounter = 1000;
			size = glm::vec2(16, 16);
			spritesheet.loadFromFile("images/turret_down.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(size, glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
		case BOSS:
			life = 200;
			size = glm::vec2(160, 208);
			spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(size, glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
		case TIER_1:
			life = 1;
			size = glm::vec2(32, 16);
			spritesheet.loadFromFile("images/force_tier1.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(size, glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
		case TIER_2:
			life = 1;
			size = glm::vec2(16, 16);
			spritesheet.loadFromFile("images/force_tier2.png", TEXTURE_PIXEL_FORMAT_RGBA);
			sprite = Sprite::createSprite(size, glm::vec2(1, 1), &spritesheet, &shaderProgram);
			break;
	}

	spritesheetBullet.loadFromFile("images/enemyBullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBullet = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1, 1), &spritesheetBullet, &shaderProgram);
}

void Enemy::update(int deltaTime, float& currentCamPos)
{
	if (posEnemy.x > CAMERA_WIDTH + currentCamPos) return;

	sprite->update(deltaTime);

	switch (type)
	{
		case SPACESHIP:
			if (timeCounter >= 750)
			{
				direction.y *= -1;
				timeCounter = 0;
			}

			posEnemy.x += 2 * direction.x;
			posEnemy.y += direction.y;
			break;
		case ROBOT:
			if (map->collisionMoveLeft(posEnemy, glm::vec2(32, 32))) direction.x = 1;
			else if (map->collisionMoveRight(posEnemy, glm::vec2(32, 32))) direction.x = -1;

			posEnemy.x += direction.x;
			break;
		case BUG:
			if (posEnemy.y < (CAMERA_HEIGHT - 16) / 2) posEnemy.y += 2;
			else posEnemy.x -= 2;
			break;
		case TURRET_TOP:
			if (timeCounter >= 2000)
			{
				Projectile* p = new Projectile("turret", spriteBullet[0], Game::instance().map);
				p->setType("turret");
				p->shoot(glm::vec2(posEnemy.x, float(size.y + posEnemy.y + 1)), 1);
				p->update(deltaTime, currentCamPos);
				bullets.push_back(p);
				timeCounter = 0;
			}
			break;
		case TURRET_DOWN:
			if (timeCounter >= 2000)
			{
				Projectile* p = new Projectile("turret", spriteBullet[0], Game::instance().map);
				p->setType("turret");
				p->shoot(glm::vec2(posEnemy.x, float(posEnemy.y - 1)), -1);
				p->update(deltaTime, currentCamPos);
				bullets.push_back(p);
				timeCounter = 0;
			}
			break;
		case BOSS:
			if (timeCounter >= 250)
			{
				Projectile* p = new Projectile("boss", spriteBullet[0], Game::instance().map);
				p->setType("boss");
				p->shoot(glm::vec2(float(posEnemy.x - 1), float(rand() % size.y + posEnemy.y)), 1);
				p->update(deltaTime, currentCamPos);
				bullets.push_back(p);
				timeCounter = 0;
			}
			break;
	}

	for (unsigned int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getState() != "destroyed") {
			bullets[i]->update(deltaTime, currentCamPos);
		}
		else {
			bullets.erase(bullets.begin() + i);
			--i;
		}
	}

	timeCounter += deltaTime;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
}

void Enemy::getHit(string type)
{
	if (getType() != TIER_1 && getType() != TIER_2) {
		if (type != "charged") {
			life = life - 2;
		}
		else {
			life = life - 5;
		}

	}

	//if (life <= 0) dead();
}

void Enemy::render(float& currentCamPos)
{
	if (posEnemy.x > CAMERA_WIDTH + currentCamPos || posEnemy.x < currentCamPos) return;
	sprite->render();
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->render();
	}
}

int Enemy::getLife()
{
	return life;
}

EnemyType Enemy::getType()
{
	return type;
}

void Enemy::dead()
{
}

vector<Projectile*> Enemy::getBullets()
{
	return bullets;
}
