#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <stb_truetype/stb_truetype.h>
#include "gl2d/gl2d.h"
#include <iostream>
#include <ctime>
#include "tools.h"

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#undef min
#undef max


int main()
{

#ifdef PLATFORM_WIN32
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
#endif

	permaAssertComment(glfwInit(), "err initializing glfw");


	glfwWindowHint(GLFW_SAMPLES, 4);

	int w = 500;
	int h = 500;
	GLFWwindow* wind = glfwCreateWindow(w, h, "geam", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	glfwSwapInterval(1);

	permaAssertComment(gladLoadGL(), "err initializing glad");
	
	gl2d::init();
	
	gl2d::Renderer2D renderer;
	renderer.create();

	
	gl2d::Font font(RESOURCES_PATH "roboto_black.ttf");
	gl2d::Texture texture(RESOURCES_PATH "test.jpg");

	long lastTime = clock();

	while (!glfwWindowShouldClose(wind))
	{
		int w = 0; int h = 0;
		glfwGetWindowSize(wind, &w, &h);
		renderer.updateWindowMetrics(w, h);
		renderer.clearScreen();

	#pragma region movement

		long newTime = clock();
		float deltaTime = (float)(newTime - lastTime) / 1000.f;
		lastTime = clock();


	#pragma endregion

		renderer.renderRectangle({ 100,350, 100, 100 }, { 0,0 }, 0, texture);
		glm::vec4 colors[4] = { Colors_Orange,Colors_Orange ,Colors_Orange ,Colors_Orange };
		renderer.renderRectangle({ 10,10, 100, 100 }, colors, {}, 30);

		renderer.renderText({ 100,100 }, "Hello", font, Colors_White);

		renderer.flush();
		glfwSwapBuffers(wind);
		glfwPollEvents();
	}
}