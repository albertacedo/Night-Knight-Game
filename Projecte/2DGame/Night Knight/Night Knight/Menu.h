#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "ShaderProgram.h"
#include <vector>
#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "Sprite.h"

class Menu {
public:
	Menu();
	~Menu();

	void init();
	void render();
	void update(int deltaTime);
	void setPosIndex(int pos);
	void initShaders();
	void initBackground();
	void changeState(int newState);
	void changeInstruction(bool act);

private:
	int pos, state, instructionN;
	glm::vec2 positions[3];
	Sprite* background;
	Sprite* creditsBackground;
	float currentTime;
	Texture spritesheet[2];
	TexturedQuad* texQuad[5];//0-> title, 1-> credits, 2-> instructions, 3-> start, 4-> sword
	glm::mat4 projection;
	Texture texs[5];//0-> title, 1-> credits, 2-> instructions, 3-> start, 4-> sword
	TexturedQuad* instruction;
	Texture instrTex[3];
	ShaderProgram texProgram;
};

#endif // _MENU_INCLUDE