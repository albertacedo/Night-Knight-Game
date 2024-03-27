#include "Menu.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>

#define SCREEN_X 32
#define SCREEN_Y 16

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

Menu::Menu() {
	currentTime = 0.0f;

}

Menu::~Menu() {
	//if (sprite != NULL) delete sprite;

}

void Menu::init() {
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	pos = state = 0;
	instructionN = 0;
	initShaders();
	initBackground();



}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	if (state == 0) {
		background->update(deltaTime);
	}
	if (state == 1) creditsBackground->update(deltaTime);


}

void Menu::setPosIndex(int newPos) {
	pos = newPos;
}

void Menu::render() {
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//if(state == 0 || state == 1)sprite[state]->render();

	if (state == 0) {
		background->render();
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		//title
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(100.f, 8.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[0]);

		//start
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(256, 145.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[3]->render(texs[3]);

		//instructions
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(200, 273, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[2]->render(texs[2]);

		//credits
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(256, 210, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[1]->render(texs[1]);

		//sword
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(positions[pos], 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[4]->render(texs[4]);
	}
	else if (state == 1) creditsBackground->render();
	else if (state == 2) {

		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		instruction->render(instrTex[instructionN]);



	}

}


void Menu::initShaders()
{
	Shader vShader, fShader;
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

void Menu::initBackground() {

	//background
	spritesheet[0].loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(float(1.f / 14.f), 0.25), &spritesheet[0], &texProgram);
	background->setNumberAnimations(1);

	background->setAnimationSpeed(0, 6);

	for (int i = 0; i < 14; ++i) {
		for (int j = 0; j < 4; ++j) {
			background->addKeyframe(0, glm::vec2(i * float(1.f / 14.f), j * 0.25));
		}
	}

	background->changeAnimation(0);

	//background->setPosition(glm::vec2(float(0), float(0)));





	//title
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(500, 150) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs[0].loadFromFile("images/menu-title.png", TEXTURE_PIXEL_FORMAT_RGBA);


	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(128, 64);
	//credits
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs[1].loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);

	//start
	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs[3].loadFromFile("images/start.png", TEXTURE_PIXEL_FORMAT_RGBA);


	//instructions
	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(256, 64);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs[2].loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);


	//sword
	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(128, 64);
	texQuad[4] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs[4].loadFromFile("images/sword.png", TEXTURE_PIXEL_FORMAT_RGBA);
	positions[0] = glm::vec2(120, 145);
	positions[1] = glm::vec2(120, 210);
	positions[2] = glm::vec2(50, 273);


	//credits-background
	spritesheet[1].loadFromFile("images/credits-background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsBackground = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(float(1.f / 5.f), 0.25), &spritesheet[1], &texProgram);
	creditsBackground->setNumberAnimations(1);

	creditsBackground->setAnimationSpeed(0, 6);

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 4; ++j) {
			creditsBackground->addKeyframe(0, glm::vec2(i * float(1.f / 5.f), j * 0.25));
		}
	}

	creditsBackground->changeAnimation(0);
	creditsBackground->setPosition(glm::vec2(float(0), float(0)));

	//instructions-background
	geom[0] = glm::vec2(0.f, 0.f); geom[1] = glm::vec2(640, 480);
	instruction = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	instrTex[0].loadFromFile("images/instructions-background-0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instrTex[1].loadFromFile("images/instructions-background-1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instrTex[2].loadFromFile("images/instructions-background-2.png", TEXTURE_PIXEL_FORMAT_RGBA);



}

void Menu::changeState(int newState) {
	state = newState;
	if (state == 2) instructionN = 0;
}

void Menu::changeInstruction(bool act) {
	if (act && instructionN < 2) {

		++instructionN;
	}
	else if (!act && instructionN > 0) {
		--instructionN;
	}

}


