#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "TileMap.h"
#include "Projectile.h"
#include "Sprite.h"
#include <vector>

enum EnemyType { SPACESHIP, ROBOT, BUG, TURRET_TOP, TURRET_DOWN, BOSS, TIER_1, TIER_2 };

class Enemy {

public:
	Enemy();
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, EnemyType type);
	~Enemy();

	void update(int deltaTime, float& currentCamPos);

	pair<int, int> Enemy::getPos() const { return { posEnemy.x, posEnemy.y }; }
	pair<int, int> Enemy::getSize() const { return { size.x, size.y }; }

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void getHit(string type);
	void render(float& currentCamPos);
	int getLife();
	EnemyType getType();
	vector<Projectile*> getBullets();
private:
	int life;
	glm::ivec2 tileMapDispl, posEnemy;
	TileMap* map;
	void dead();
	Texture spritesheet;
	Sprite* sprite;
	Texture spritesheetBullet;
	Sprite* spriteBullet;
	EnemyType type;
	glm::ivec2 direction;
	glm::ivec2 size;
	int timeCounter;
	vector<Projectile*> bullets;
};

#endif // _ENEMY_INCLUDE