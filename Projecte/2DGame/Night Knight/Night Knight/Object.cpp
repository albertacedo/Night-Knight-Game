#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Object.h"
#include "Game.h"






void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {





}


void update(int deltaTime) {




}


ObjectName getName();


void Object::render()
{
	sprite->render();
}

void Object::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Object::setPosition(const glm::vec2& pos)
{
	posObject = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObject.x), float(tileMapDispl.y + posObject.y)));
}

