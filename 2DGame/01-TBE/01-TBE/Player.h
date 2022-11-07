#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "TileMap.h"
#include "Projectile.h"
#include <vector>
#include "Sprite.h"


class Player {

public:
	Player();
	~Player();

	void update(int deltaTime, float &currentCamPos);
	void render();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	int getPlayerLife();
	pair<int, int> Player::getPos() const { return { posPlayer.x, posPlayer.y }; }
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void getHit(string type);
	void addPowerUp(string powerup);
	void addForceCheats();
	void setInvicibility(bool active);
	vector<Projectile*> getBullets();
private:
	int life;
	glm::ivec2 tileMapDispl, posPlayer;
	TileMap* map;
	Texture spritesheet;
	int milisecondsCharged;
	void dead();
	bool chargingShot;
	glm::ivec2 velocity;
	bool moving;
	int const maxVelocity = 4;
	vector<Projectile*> bullets;
	Sprite* sprite;
	vector<string> PowerUp;
	int lastTimeShot = 0;
	int t1Counter = 0;
	ShaderProgram shaders;
	Texture bBullet, cBullet, t1Bullet, t2Bullet;
	Sprite basicBullet;
	Sprite chargedBullet;
	Sprite force_tier1;
	Sprite force_tier2;
	vector<Sprite> bulletSprite;
	bool invincibility;
};

#endif // _PLAYER_INCLUDE