#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Text.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


class Scene
{

public:
	Scene();
	~Scene();

	void init(int lvl,bool newGame);
	void update(int deltaTime);
	void render();
	void comprovar_temps(int deltaTime);
	bool comprovar_mort();
	void initLevel(int lvl,bool newGame);
	bool gameOver();
	void godMode();
	void revealKey();

	bool comprovarKey();
	bool comprovarDoor();
	void comprovarGemma();

	int nextLevel();
	int getLevel();
	int setLevel(int l);
private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Enemy *enemies[10];
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	float temps_ini, temps_actual,temps_aux;
	bool victoria, derrota,god,showText,doorOpened,showKey,haveKey,showGem;
	int puntuacio,level;
	TexturedQuad* background;
	Texture texBackground[3];

	TexturedQuad* object[3]; //0-> door    1-> key    2-> gem
	Texture texObject[4]; //0->door_closed      1->door_opened       2-> key     3 ->gem
};


#endif // _SCENE_INCLUDE

