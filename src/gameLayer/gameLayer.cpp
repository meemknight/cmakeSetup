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
gl2d::Texture t;
gl2d::Font f;

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	//loading the saved data. Loading an entire structure like this makes savind game data very easy.
	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));


	t.loadFromFile(RESOURCES_PATH "test.jpg", true);
	f.createFromFile(RESOURCES_PATH "roboto_black.ttf");

	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

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
	renderer.renderRectangle({gameData.rectPos, 100, 100}, t);

	renderer.renderText({200, 200}, "Hello my first game", f, Colors_White);


	renderer.flush();



	//ImGui::ShowDemoWindow();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{

	//saved the data.
	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
