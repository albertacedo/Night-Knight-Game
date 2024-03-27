#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define _USE_MATH_DEFINES
#include <math.h>


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 4






Scene::Scene()
{
	map = NULL;
	player = NULL;
	player = new Player();
	


}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	for (int i = 0; i < 10; i++)
		if (enemies[i] != NULL)
			delete enemies[i];
}


void Scene::init(int lvl,bool newGame)
{
	level = lvl;
	initShaders();
	showText = false;
	player = new Player();
	enemies[0] = new Enemy();
	enemies[1] = new Enemy();
	enemies[2] = new Enemy();
	enemies[3] = new Enemy();
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	derrota = god = victoria =false;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640, 480) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texBackground[0].loadFromFile("images/gameWallpaper.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texBackground[1].loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texBackground[2].loadFromFile("images/victory.png", TEXTURE_PIXEL_FORMAT_RGBA);


	geom[1] = glm::vec2(32, 32);
	object[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texObject[0].loadFromFile("images/doorClosed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texObject[1].loadFromFile("images/doorOpened.png", TEXTURE_PIXEL_FORMAT_RGBA);


	geom[1] = glm::vec2(32, 16);
	object[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texObject[2].loadFromFile("images/key.png", TEXTURE_PIXEL_FORMAT_RGBA);

	geom[1] = glm::vec2(50, 50);
	object[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texObject[3].loadFromFile("images/gema.png", TEXTURE_PIXEL_FORMAT_RGBA);

	

	if (!text.init("fonts/OpenSans-Regular.ttf"))
		cout << "Could not load font!!!" << endl;


	initLevel(level,newGame);
}

void Scene::comprovar_temps(int deltaTime) {
	currentTime += deltaTime;
	temps_actual = 60 - ((currentTime - temps_ini) / 1000);
	
}

void Scene::godMode() {
	showText = true;
	temps_aux = currentTime + 2000;
	god = !god;
}

void Scene::initLevel(int lvl, bool newGame) {
	if (lvl > 2) {
		victoria = true;
	}
	else {
		string s = "levels/level0" + to_string(lvl) + ".txt";
		map = TileMap::createTileMap(s, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		enemies[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		enemies[0]->setPosition(glm::vec2(25 * map->getTileSize(), 12 * map->getTileSize()));
		enemies[0]->setTileMap(map);
		enemies[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		enemies[1]->setPosition(glm::vec2(10 * map->getTileSize(), 12 * map->getTileSize()));
		enemies[1]->setTileMap(map);
		enemies[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		enemies[2]->setPosition(glm::vec2(27 * map->getTileSize(), 24 * map->getTileSize()));
		enemies[2]->setTileMap(map);
		enemies[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		enemies[3]->setPosition(glm::vec2(8 * map->getTileSize(), 24 * map->getTileSize()));
		enemies[3]->setTileMap(map);


		temps_ini = currentTime;
		if (newGame) {
			puntuacio = 0;
			player->setVides(3);
		}
		showKey = haveKey = false;
		showGem = true;
	}
	
}




bool Scene::comprovar_mort() {
	if (god) return false;

	if (player->getMorint()) return true;

	else {
		glm::ivec2 pos1 = player->getPosition();

		for (int i = 0; i < 10; i++) {
			if (enemies[i] != NULL) {
				glm::vec2 pos2 = enemies[i]->getPosition();
				if (pos1.x < (pos2.x + 16) && pos2.x < (pos1.x + 16)) {
					if (pos1.y < (pos2.y + 16) && pos2.y < (pos1.y + 16)) {
						return true;
					}
				}
			}
		}

	}


	return false;
}

bool Scene::comprovarKey() {
	glm::ivec2 pos1 = player->getPosition();
	glm::ivec2 pos2 = glm::vec2(88.f, 256.f);//hardCoded
	if (pos1.x < (pos2.x + 16) && pos2.x < (pos1.x + 16)) {
		if (pos1.y < (pos2.y + 16) && pos2.y < (pos1.y + 16)) {
			haveKey = true;
			return true;
		}
	}
	return false;


}

bool Scene::comprovarDoor() {
	glm::ivec2 pos1 = player->getPosition();
	glm::ivec2 pos2 = glm::vec2(326.f,64.f);//hardCoded
	if (pos1.x < (pos2.x + 16) && pos2.x < (pos1.x + 16) && haveKey) {
		if (pos1.y < (pos2.y + 16) && pos2.y < (pos1.y + 16)) {
			puntuacio += temps_actual * 10;
			return true;
		}
	}
	return false;
}

void Scene::comprovarGemma() {
	glm::ivec2 pos1 = player->getPosition();
	glm::ivec2 pos2 = glm::vec2(378.f, 192.f);//hardCoded
	if (pos1.x < (pos2.x + 16) && pos2.x < (pos1.x + 16)) {
		if (pos1.y < (pos2.y + 16) && pos2.y < (pos1.y + 16)) {
			puntuacio += 500;
			showGem = false;
		}
	}

}


bool Scene::gameOver() {
	return derrota;
}


int Scene::nextLevel() {
	
	++level;
	return level;
}

int Scene::getLevel() {
	return level;
}

int Scene::setLevel(int l) {
	level = l;
	return level;
}

void Scene::update(int deltaTime)
{
	if (!victoria){
		comprovar_temps(deltaTime);
		player->update(deltaTime);
		enemies[0]->update(deltaTime);
		enemies[1]->update(deltaTime);
		enemies[2]->update(deltaTime);
		enemies[3]->update(deltaTime);

		if (!god) {
			bool morint = comprovar_mort();
			player->setMorint(morint);

			if (player->getMorint()) {
				player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
				player->setVides(player->getVides() - 1);
				player->setMorint(false);
			}
		}
		else player->setMorint(false);

		if (map->getRajola() == 0) showKey = true;

		if (!haveKey && showKey && comprovarKey()) haveKey = true;
		derrota = temps_actual <= 0 || player->getVides() <= 0;

		if (showGem) comprovarGemma();
	}
	
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
	if (derrota) {
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		background->render(texBackground[1]);
	}
	else if (victoria) {
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		background->render(texBackground[2]);
		text.render(to_string(puntuacio), glm::vec2(300, 310), 50, glm::vec4(0, 0, 0, 1));
	}
	else {
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		background->render(texBackground[0]);
		
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(360.f, 80.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		object[0]->render(texObject[haveKey]);


		if (showKey && !haveKey) {
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(120.f, 280.f, 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			object[1]->render(texObject[2]);
		}
		if (showGem) {
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(400.f, 195.f + sin(currentTime / 250.f), 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			object[2]->render(texObject[3]);

		}
		
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		map->render();
		player->render();
		float enemy_pos = (sin(currentTime / 500.f) / 10);
		enemies[0]->setPosition(glm::vec2((25 * map->getTileSize() + 400 * enemy_pos), 12 * map->getTileSize()));
		enemies[0]->setOrientation(cos(currentTime / 500.f) / 10);
		enemies[0]->render();
		enemies[1]->setPosition(glm::vec2((10 * map->getTileSize() + 400 * enemy_pos), 12 * map->getTileSize()));
		enemies[1]->setOrientation(cos(currentTime / 500.f) / 10);
		enemies[1]->render();
		enemies[2]->setPosition(glm::vec2((27 * map->getTileSize() + 400 * enemy_pos), 24 * map->getTileSize()));
		enemies[2]->setOrientation(cos(currentTime / 500.f) / 10);
		enemies[2]->render();
		enemies[3]->setPosition(glm::vec2((8 * map->getTileSize() + 400 * enemy_pos), 24 * map->getTileSize()));
		enemies[3]->setOrientation(cos(currentTime / 500.f) / 10);
		enemies[3]->render();

		text.render(to_string(int(temps_actual)), glm::vec2(CAMERA_WIDTH / 2, 50), 30, glm::vec4(1, 1, 1, 1));
		text.render("Lives: " + to_string(int(player->getVides())), glm::vec2(CAMERA_WIDTH - 150, 50), 30, glm::vec4(1, 1, 1, 1));
		text.render("Points: " + to_string(puntuacio), glm::vec2(50, 50), 30, glm::vec4(1, 1, 1, 1));
		//text.render(to_string(player->getPosition().x) + " " + to_string(player->getPosition().y), glm::vec2(50, 150), 30, glm::vec4(1, 1, 1, 1));


		



		if (showText && temps_aux > currentTime) {
			string s = (god) ? "On" : "Off";
			text.render("God Mode: " + s, glm::vec2(400, 100), 30, glm::vec4(1, 1, 1, 1));
		}
		else showText = false;
	}
	
	
	
	
	//text.render("Enemic", glm::vec2(enemy_pos + 235.f, 280.f), 20, glm::vec4(1, 1, 1, 1));
}

void Scene::revealKey() {
	showKey = true;
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



