#include "Projectile.h"
#include <string>
#include <iostream>
#include <glm/detail/type_vec.hpp>
#include "Scene.h"
#include "TileMap.h"

using namespace std;


Projectile::Projectile(string t, Sprite spr, TileMap *tilemap) {
	map = tilemap;
	type = t;
	sprite = spr;
}

Projectile::~Projectile() {
	//if (map != NULL) delete map;
}


string Projectile::getType() {
	return type;
}

void Projectile::setType(string t) {
	type = t;
}

void Projectile::render()
{
	sprite.render();
}

void Projectile::shoot(glm::vec2 pos, int directionY) {
	setPosition(glm::vec2(pos.x, pos.y));
	if (type == "turret") {
		velocity.first = -2;
		velocity.second = 2*directionY;
	}
	else if (type == "boss") {
		velocity.first = -5;
		velocity.second = 0;
	}
	else { //Playershots
		setPosition(glm::vec2(float(pos.x + 32), float(pos.y + 8)));
		if (type == "basic") {
			velocity.first = 7;
			velocity.second = 0;
		}
		else if(type == "charged") {
			velocity.first = 10;
			velocity.second = 0;
		}
		else if (type == "tier1") {
			velocity.first = 15;
			velocity.second = 0;
		}
		else if (type == "tier2") {
			velocity.first = 5;
			velocity.second = 5*directionY;
		}
	}
}

void Projectile::update(int deltaTime, float currentCamPos) {
	posProjectile.x += velocity.first;
	posProjectile.y += velocity.second;
	sprite.setPosition(glm::vec2(float(posProjectile.x), float(posProjectile.y)));
	if (posProjectile.x < currentCamPos || posProjectile.x > CAMERA_WIDTH + currentCamPos || posProjectile.y < 0 || posProjectile.y > CAMERA_HEIGHT) {
		setState("destroyed");
	}else if (tileMapCollision()) {
		setState("destroyed");
	}
}

void Projectile::setPosition(glm::vec2& pos) {
	posProjectile.x = pos.x;
	posProjectile.y = pos.y;
}

void Projectile::setTileMap(TileMap *tilemap) {
	map = tilemap;
}

bool Projectile::tileMapCollision() {
	glm::ivec2 p(posProjectile.x, posProjectile.y);
	if (map->collisionMoveLeft(p, glm::ivec2(16, 16)) || map->collisionMoveRight(p, glm::ivec2(16, 16))) {
		if (type != "tier2") return true;
		//tier 2
		velocity.second *= -1;
		return false;
	}
	else if(map->collisionMoveUp(p, glm::ivec2(16, 16),&p.y) || map->collisionMoveDown(p, glm::ivec2(16, 16), &p.y)) {
		if (type != "tier2") return true;
		//tier 2
		velocity.second *= -1;
		return false;
	}
	return false;
}

string Projectile::getState() {
	return state;
}

void Projectile::setState(string s) {
	state = s;
}