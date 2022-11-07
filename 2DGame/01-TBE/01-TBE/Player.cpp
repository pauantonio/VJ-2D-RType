#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"



Player::Player() {
	map = NULL;
}

Player::~Player() {
//	if (map != NULL) delete map;
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	life = 5;
	chargingShot = false;
	milisecondsCharged = 0;
	velocity.x = 0;
	velocity.y = 0;
	invincibility = false;
	bulletSprite = {};
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	bullets = {};
	addPowerUp("basic");
	pair<int, int> size;
	string texture;
	size.first = 8;
	size.second = 4;
	const glm::vec2 basic = glm::ivec2(size.first, size.second);
	const glm::vec2 sizesprt = glm::vec2(1, 1);
	texture = "images/chargedbullet.png";
	size.first = 16;
	size.second = 16;
	const glm::vec2 charged = glm::ivec2(size.first, size.second);
	cBullet.loadFromFile(texture, TEXTURE_PIXEL_FORMAT_RGBA);
	chargedBullet = Sprite(charged, sizesprt, &cBullet, &shaderProgram);
	bulletSprite.push_back(chargedBullet);
	
	texture = "images/basicbullet.png";
	bBullet.loadFromFile(texture, TEXTURE_PIXEL_FORMAT_RGBA);
	basicBullet = Sprite(basic , sizesprt , &bBullet, &shaderProgram);
	bulletSprite.push_back(basicBullet);

	t1Bullet.loadFromFile("images/force_tier1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size.first = 32;
	size.second = 16;
	const glm::vec2 tier1 = glm::ivec2(size.first, size.second);
	force_tier1 = Sprite(tier1, sizesprt, &t1Bullet, &shaderProgram);
	bulletSprite.push_back(force_tier1);
	
	t2Bullet.loadFromFile("images/force_tier2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size.first = 8;
	size.second = 8;
	const glm::vec2 tier2 = glm::ivec2(size.first, size.second);
	force_tier2 = Sprite(tier2, sizesprt, &t2Bullet, &shaderProgram);
	bulletSprite.push_back(force_tier2);


}

void Player::update(int deltaTime, float &currentCamPos) {
	if (Game::instance().getKey('e')) {
		if (!chargingShot) {
			chargingShot = true;
			milisecondsCharged += 1;
		}
		else {
			milisecondsCharged += 1;
		}

		

	}

	if (milisecondsCharged >= 120 && !Game::instance().getKey('e')) {
		Projectile* p = new Projectile("player", bulletSprite[0], Game::instance().map);
		p->setType("charged");
		p->shoot(posPlayer, 1);
		p->update(deltaTime, currentCamPos);
		bullets.push_back(p);
		milisecondsCharged = 0;
	}

	sprite->update(deltaTime);
	bool m = false;
	if (Game::instance().getKey('w')) {
		m = true;
		if (!moving) moving = true;
		if (velocity.y < maxVelocity) {
			velocity.y += 2;
		}
		posPlayer.y -= velocity.y;
		if (map->collisionMoveUp(posPlayer, glm::vec2(32, 16), &posPlayer.y)) {
			velocity.y = 0;
			posPlayer.y += 4;
			moving = false;
			m = false;
		}
		if (posPlayer.y < 0) posPlayer.y = 1;
	}

	if (Game::instance().getKey('a')) {
		m = true;
		if (!moving) moving = true;
		if (velocity.x < maxVelocity) velocity.x += 2;
		posPlayer.x -= velocity.x;

		//if (sprite->animation() != MOVE_LEFT)
	//		sprite->changeAnimation(MOVE_LEFT);
		if (map->collisionMoveLeft(posPlayer, glm::vec2(32, 16))) {
			posPlayer.x += velocity.x;
			velocity.x = 0;
	//		sprite->changeAnimation(STAND_LEFT);
		}

		if (posPlayer.x < currentCamPos) {
			posPlayer.x = currentCamPos;
		}

	}

	if (Game::instance().getKey('s')) {
		m = true;
		if (!moving) moving = true;
		if (velocity.y < maxVelocity) {
			velocity.y += 2;
		}
		posPlayer.y += velocity.y;
		if (map->collisionMoveDown(posPlayer, glm::vec2(32, 16), &posPlayer.y)) {
			posPlayer.y -= 2;
			velocity.y = 0;
			moving = false;
			m = false;
		}
		if (posPlayer.y > CAMERA_HEIGHT) {
			posPlayer.y = CAMERA_HEIGHT;
		}
	}

	if (Game::instance().getKey('d')) {

	//	if (sprite->animation() != MOVE_RIGHT)
	//		sprite->changeAnimation(MOVE_RIGHT);
		if (!moving) moving = true;
		if (velocity.x < maxVelocity) velocity.x += 2;
		posPlayer.x += velocity.x;
		if (map->collisionMoveRight(posPlayer, glm::vec2(32, 16))) {
			posPlayer.x -= velocity.x;
			velocity.x = 0;
	//		sprite->changeAnimation(STAND_RIGHT);
		}

		if (posPlayer.x > CAMERA_WIDTH + currentCamPos) {
			posPlayer.x -= CAMERA_WIDTH + currentCamPos;
		}
	}

	if (Game::instance().getKey(' ')) {

		if (lastTimeShot >= 100) {
			lastTimeShot = 0;
			for (unsigned int i = 0; i < PowerUp.size(); i++) {
				int x;
				int directionY = 1;
				if (PowerUp[i] == "basic") {
					x = 1;
				}
				else if (PowerUp[i] == "tier1" && t1Counter >= 500) {
					x = 2;
					t1Counter = 0;
				}
				else if (PowerUp[i] == "tier1" && t1Counter < 500) {
					break;
				}
				else if (PowerUp[i] == "tier2") {
					x = 3;
					Projectile* p = new Projectile("player", bulletSprite[x], Game::instance().map);
					p->setType(PowerUp[i]);
					p->setState("ok");
					p->shoot(posPlayer, directionY);
					p->update(deltaTime, currentCamPos);
					bullets.push_back(p);

					directionY = -1;
				}

				Projectile* p = new Projectile("player", bulletSprite[x], Game::instance().map);
				p->setType(PowerUp[i]);
				p->setState("ok");
				p->shoot(posPlayer, directionY);
				p->update(deltaTime, currentCamPos);
				bullets.push_back(p);
			}
		}
			
	}
	lastTimeShot += deltaTime;
	t1Counter += deltaTime;
	if (!moving) {
		if (velocity.x > 2) velocity.x -= 2;
		if (velocity.x < 2) velocity.x = 2;
		if (velocity.y > 2) velocity.y -= 2;
		if (velocity.y < 2) velocity.y = 2;

	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	if (posPlayer.x < currentCamPos) {
		posPlayer.x = currentCamPos;
	}else if (posPlayer.x > currentCamPos + CAMERA_WIDTH) posPlayer.x = currentCamPos + CAMERA_WIDTH;

	if (posPlayer.y < 0) {
		posPlayer.y = 0;
	}
	else if (posPlayer.y > CAMERA_HEIGHT) posPlayer.y = CAMERA_HEIGHT - 8;

	for (unsigned int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getState() != "destroyed") {
			bullets[i]->update(deltaTime, currentCamPos);
		}
		else {
			bullets.erase(bullets.begin()+i);
			--i;
		}
	}


}

void Player::render()
{
	if (life <= 0) return;
	sprite->render();
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->render();
	}
}

void Player::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos) {
	posPlayer = pos;
}

void Player::getHit(string type) {
	if (invincibility) return;

	if (type != "boss") {
		life = life - 2;
	}else {
		life = life - 5;
	}

	//if (life <= 0) dead();
}

void Player::addPowerUp(string powerup)
{
	if (find(PowerUp.begin(), PowerUp.end(), powerup) == PowerUp.end()) {
		PowerUp.push_back(powerup);
		life += 5;
	}
	
}

void Player::addForceCheats()
{
	addPowerUp("tier1");
	addPowerUp("tier2");
}


void Player::dead() {

}


int Player::getPlayerLife() {
	return life;
}


void Player::setInvicibility(bool active) {
	invincibility = active;
}

vector<Projectile*> Player::getBullets()
{
	return bullets;
}
