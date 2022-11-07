#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include <vector>
#include "Projectile.h"
#include "TexturedQuad.h"
#include "Enemy.h"
#include "Player.h"
#include "Sprite.h"



#define CAMERA_WIDTH 384
#define CAMERA_HEIGHT 256


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();
	TileMap* map;
	Player* player;
	void init();
	void update(int deltaTime);
	void render();
	void godMode(bool active);
	void forceMode();
	void teleport(int value);
	void loadEnemies();
	void renderEnemies(float& currentCamPos);
	void updateEnemies(int deltaTime, float& currentCamPos);
private:
	void initShaders();
	bool inRange(int low, int high, int x);
private:
	ShaderProgram texProgram;
	float currentTime;
	float currentCamPos;
	glm::mat4 projection;
	string background;
	TexturedQuad* texQuad;
	Texture texs;
	int size_x;
	vector<Enemy*> enemies;
	vector<Projectile*> playerBullets;
	vector<int> locations;
	bool win;
	Texture spritesheetHeart;
	Sprite* spriteHeart;
};


#endif // _SCENE_INCLUDE

