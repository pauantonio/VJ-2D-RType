#pragma once
#include <string>
#include <iostream>
#include <glm/detail/type_vec.hpp>
#include "TileMap.h"
#include "Sprite.h"
using namespace glm;

class Projectile {
public:
	Projectile(string t, Sprite spr, TileMap *tilemap);
	~Projectile();
	void setType(string t);
	void render();
	string getType();
	void shoot(glm::vec2 posPlayer, int directionY);
	void update(int deltaTime, float currentCamPos);
	void setTileMap(TileMap *tilemap);
	glm::vec2 getPos() const { return { posProjectile }; }
	void setState(string s);
	string getState();
private:
	string type;
	string sender;
	void setPosition(glm::vec2& pos);
	pair<int,int> velocity;
	glm::vec2 posProjectile;
	bool tileMapCollision();
	TileMap *map;
	string state;
	Sprite sprite;
	Texture spritesheet;
	glm::ivec2 tileMapDispl;
};