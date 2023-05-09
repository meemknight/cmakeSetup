#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>


struct GameData
{
	glm::vec2 rectPos = {100,100};

}gameData;

gl2d::Renderer2D renderer;

bool initGame()
{
	gl2d::init();

	renderer.create();


	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));


	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX();
	h = platform::getFrameBufferSizeY();
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);

	renderer.updateWindowMetrics(w, h);

#pragma endregion


	if (platform::isKeyHeld(platform::Button::Left))
	{
		gameData.rectPos.x -= deltaTime * 50;
	}
	if (platform::isKeyHeld(platform::Button::Right))
	{
		gameData.rectPos.x += deltaTime * 50;
	}
	if (platform::isKeyHeld(platform::Button::Up))
	{
		gameData.rectPos.y -= deltaTime * 50;
	}
	if (platform::isKeyHeld(platform::Button::Down))
	{
		gameData.rectPos.y += deltaTime * 50;
	}

	gameData.rectPos = glm::clamp(gameData.rectPos, glm::vec2{0,0}, glm::vec2{w - 100,h - 100});
	renderer.renderRectangle({gameData.rectPos, 100, 100}, Colors_Blue);



	ImGui::ShowDemoWindow();

	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
