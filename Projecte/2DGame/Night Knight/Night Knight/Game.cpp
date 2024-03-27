#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	

	pos = 0;
	state = 0;
	
	menu.init();
}

bool Game::update(int deltaTime)
{
	if(state == -1) scene.update(deltaTime);
	else menu.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (state == -1) scene.render();
	else menu.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		if (state !=0) {
			
			menu.changeState(0);
			state = 0;
		}
		else
			bPlay = false;
	}

	if (key == 13) {//tecla enter-> per seleccionar la pantalla que volem
		if (pos == 0) {
			scene.init(0,true);
			state = -1;
		}
		else {
			menu.changeState(pos);
			state = pos;
		}

	}
	if (state == -1 && scene.gameOver() && key == 114) scene.init(scene.getLevel(),true);

	if (state == -1 && key == 103) scene.godMode();
	if (state == -1 && key == 107) scene.revealKey();
	if (state == -1 && key == 32 && scene.comprovarDoor()) scene.initLevel(scene.nextLevel(),false);
	if (state == 0) {
		if (key == 48) {
			state = -1;
			scene.init(scene.setLevel(0),true);
		}
		else if (key == 49) {
			state = -1;
			scene.init(scene.setLevel(1),true);
		}
		else if (key == 50) {
			state = -1;
			scene.init(scene.setLevel(2),true);
		}
		

	}



	
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	if (key == GLUT_KEY_UP && state >=0) {
		if (pos > 0) {
			pos -= 1;
			menu.setPosIndex(pos);
		}
	}
	else if (key == GLUT_KEY_DOWN && state >= 0) {
		if (pos < 2) {
			pos += 1;
			menu.setPosIndex(pos);

		}
	}
	else if (key == GLUT_KEY_LEFT && state == 2) {
		menu.changeInstruction(false);


	}
	else if (key == GLUT_KEY_RIGHT && state == 2) {
		menu.changeInstruction(true);

	}

	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





