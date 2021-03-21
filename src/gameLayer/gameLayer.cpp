#include "gameLayer.h"
#include "gl2d/gl2d.h"
#include "platformInput.h"

gl2d::Renderer2D renderer;

gl2d::Font font;
gl2d::Texture texture;

struct GameData
{
	float posx=100;
	float posy=100;

}gameData;

bool initGame()
{
	renderer.create();
	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	texture.loadFromFile(RESOURCES_PATH "test.jpg");

	if(!platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData)))
	{
		gameData = GameData();
	}

	return true;
}

bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w= platform::getWindowSizeX();
	h = platform::getWindowSizeY();
	
	renderer.updateWindowMetrics(w, h);
	renderer.clearScreen();
#pragma endregion


#pragma region input
	float speed = 400 * deltaTime;

	if(platform::isKeyHeld(platform::Button::Up))
	{
		gameData.posy -= speed;
	}
	if (platform::isKeyHeld(platform::Button::Down))
	{
		gameData.posy += speed;
	}
	if (platform::isKeyHeld(platform::Button::Left))
	{
		gameData.posx -= speed;
	}
	if (platform::isKeyHeld(platform::Button::Right))
	{
		gameData.posx += speed;
	}

	glm::vec4 colors[4] = { Colors_Orange, Colors_Orange, Colors_Orange, Colors_Orange };

	if(platform::isLMousePressed())
	{
		colors[0] = Colors_Green;
		colors[1] = Colors_Red;
		colors[2] = Colors_Magenta;
		colors[3] = Colors_Blue;
	}

	if(platform::mouseMoved())
	{
		renderer.renderText({ 100,100 }, "Hello", font, Colors_White);
		platform::showMouse(true);
	}else
	{
		platform::showMouse(false);
	}

#pragma endregion



	renderer.renderRectangle({ 10,10, 100, 100 }, colors, {}, 30);

	
	renderer.renderRectangle({ gameData.posx,gameData.posy, 100, 100 }, { 0,0 }, 0, texture);


#pragma region set finishing stuff
	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
