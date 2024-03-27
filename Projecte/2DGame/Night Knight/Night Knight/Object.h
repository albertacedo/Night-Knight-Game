#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




enum ObjectName {
	KEY, GEM, CLOCK, NEEDLES
};




class Object
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	ObjectName getName();


private:
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	ShaderProgram* shaderProgram;
	glm::ivec2 tileMapDispl, posObject;

	ObjectName name;

};

#endif