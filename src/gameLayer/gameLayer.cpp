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
#include <shader.h>
#include <glui/glui.h>

struct GameData
{


}gameData;

GLuint vertex;
ShaderProgram s;
GLint u_color;
GLint u_mat;

//for button
gl2d::Renderer2D renderer2d;
gl2d::Font font;

bool initGame()
{
	glDisable(GL_CULL_FACE);

	//x y
	/*
	float points[] =
	{
		-1, -1,
		1, -1,
		1, 1,

		-1, -1,
		1, 1,
		-1, 1
	};
	*/

	float points[] =
	{
		1, 0.7,
		-1, 0.7,
		-1, -0.7,

		1, 0.7,
		-1, -0.7,
		1, -0.7
	};

	glGenBuffers(1, &vertex); // var vertex;


	// vertex data -> VERTEX_ARRAY (VBO) = vertex
	glBindBuffer(GL_ARRAY_BUFFER, vertex); //seteaza bufferul curent cu care lucram


	//VERTEX_ARRAY = points
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // aici ii zici cum arata datele tale (2 floaturi per component)
	//index, cate coordonate, ce tip de date, normalizare, dimensiunea unui punct, de unde incep

	glEnableVertexAttribArray(0); //da folosim chestia asta, 0 reprezinta indexul

	// vertex data					-> VERTEX_ARRAY = vertex
	// cum arata datele tale		-> VAO = new VAO(float, 2 componente)
	// shader						-> Shader s = new Shader(yourShader);

	s = ShaderProgram(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");

	u_color = s.getUniformLocation("u_color");
	u_mat = s.getUniformLocation("u_mat");


	gl2d::init();
	renderer2d.create();

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");
	
	glui::gluiInit();


	return true;
}


void renderRect(glm::vec3 color, glm::mat4 mat)
{
	s.bind();
	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glUniform3f(u_color, color.r, color.g, color.b);

	glUniformMatrix4fv(u_mat, 1, GL_FALSE, &mat[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderMasina(glm::vec3 color, glm::vec2 deplasare, glm::mat4 resizeMatrix)
{

	glm::mat4 matrTransl1 = glm::translate(glm::mat4(1.0f), glm::vec3(deplasare, 0));
	glm::mat4 matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(deplasare + glm::vec2(80,0), 0.0));

	glm::mat4 sc3 = glm::scale(glm::mat4(1.f), {180,160,0.f});
	glm::mat4 sc4 = glm::scale(glm::mat4(1.f), {60,100,0.f});

	glm::mat4 masina1 = resizeMatrix * matrTransl1 * sc3;
	glm::mat4 parbriz1 = resizeMatrix * matrTransl2 * sc4;

	renderRect(color, masina1);
	renderRect({0.7,1.0,1.0}, parbriz1);

}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float f)
{
	return a * (1 - f) + b * f;
};

void renderMasinaPolitie(glm::vec2 deplasare, glm::mat4 resizeMatrix)
{

	glm::mat4 matrTransl3 = glm::translate(glm::mat4(1.0f), glm::vec3(deplasare, 0.0));
	glm::mat4 matrTransl4 = glm::translate(glm::mat4(1.0f), glm::vec3(deplasare + glm::vec2(80, 0), 0.0));
	glm::mat4 matrTransl5 = glm::translate(glm::mat4(1.0f), glm::vec3(deplasare + glm::vec2(-20, +30), 0.0));
	glm::mat4 matrTransl6 = glm::translate(glm::mat4(1.0f), glm::vec3(deplasare + glm::vec2(-20, -20), 0.0));

	glm::mat4 sc3 = glm::scale(glm::mat4(1.f), {180,160,0.f});
	glm::mat4 sc4 = glm::scale(glm::mat4(1.f), {60,100,0.f});
	glm::mat4 sc5 = glm::scale(glm::mat4(1.f), {30,30,0.f});

	glm::mat4 masina2 = resizeMatrix * matrTransl3 * sc3;
	glm::mat4 parbriz2 = resizeMatrix * matrTransl4 * sc4;
	glm::mat4 sirena1 = resizeMatrix * matrTransl5 * sc5;
	glm::mat4 sirena2 = resizeMatrix * matrTransl6 * sc5;

	float time = ((float)clock() / CLOCKS_PER_SEC) * 4;

	glm::vec3 redColor =  lerp({1.0,0.0,0.0}, {1,1,1}, std::pow((std::sin(time)+1)/2.f, 1.5) );
	glm::vec3 blueColor = lerp({0.0,0.0,1.0}, {1,1,1}, std::pow((std::cos(time)+1)/2.f, 1.5) );

	renderRect({0.0,0.0,0.0}, masina2);
	renderRect({0.7,1.0,1.0}, parbriz2);
	renderRect(redColor, sirena1);
	renderRect(blueColor, sirena2);
}

bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getWindowSizeX();
	h = platform::getWindowSizeY();

	renderer2d.updateWindowMetrics(w, h);

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.7f, 0.0f, 1.0f);

#pragma endregion

	static bool play = 0;

	if (!play)
	{
		gl2d::enableNecessaryGLFeatures();

		glui::Begin(1);
	
		if (glui::Button("Play", {0.1,0.7,0.2,1}))
		{
			play = true;
		}
		
		glui::End();

		glui::renderFrame(renderer2d, font, platform::getRelMousePosition(),
			platform::isLMousePressed(), platform::isLMouseHeld(), platform::isLMouseReleased(),
			platform::isKeyReleased(platform::Button::Escape), platform::getTypedInput(), deltaTime);

		renderer2d.flush();

		return true;

	}


	static float deplasare = -200;
	static float deplasareMasina = 0;
	static float deplasareMasinaPolitie = 0;
	static float deplasareMasinaPolitieY = 0;
	static float speedMasina = 270;
	static float speedMasinaPolitie = 530;
	//static float speed = 244;
	static float speed = speedMasinaPolitie;

	deplasare += deltaTime * speed;
	deplasareMasina += deltaTime * speedMasina;
	deplasareMasinaPolitie += deltaTime * speedMasinaPolitie;

	//ImGui::Begin("test");
	//
	//ImGui::DragFloat("speed", &speed, 1);
	//ImGui::DragFloat("speed masina", &speedMasina, 1);
	//ImGui::DragFloat("speed masina politie", &speedMasinaPolitie, 1);
	//
	//ImGui::End();


	glm::mat4 resizeMatrix = glm::ortho(-(float)w + deplasare, (float)w + deplasare, -(float)h, (float)h);
	glm::mat4 sc1 = glm::scale(glm::mat4(1.f), {(float)10000,(float)600,0.f});
	glm::mat4 sc2 = glm::scale(glm::mat4(1.f), {50,15,0.f});

	glm::mat4 sosea = resizeMatrix * sc1;
	renderRect({0.5,0.5,0.5}, sosea);


	for (int i = -10000; i < 10000; i += 300)
	{
		float x = (float)i;
		glm::mat4 matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.f, 0.0));
		glm::mat4 linie = resizeMatrix * matrTransl * sc2;
		renderRect({1.0,1.0,1.0}, linie);
	}

	renderMasina({0.5,0.0,0.0}, {0.f+ deplasareMasina, -180.f}, resizeMatrix);
	renderMasina({0.0,0.2,0.7}, {400.f + deplasareMasina, -180.f}, resizeMatrix);
	renderMasina({0.1,0.8,0.1}, {900.f + deplasareMasina, -180.f}, resizeMatrix);


	//logica depasire
	static float timer = 0;
	timer += deltaTime;

	if (timer < 1.f)
	{

	}
	else if (timer < 2.f)
	{
		deplasareMasinaPolitieY += deltaTime * 400;
	}
	else if (timer < 11.f)
	{

	}
	else if (timer < 12)
	{
		deplasareMasinaPolitieY -= deltaTime * 400;
	}


	renderMasinaPolitie({-1000.f + deplasareMasinaPolitie, -180.f + deplasareMasinaPolitieY}, resizeMatrix);
	renderMasina({0.2,0.1,0.4}, {-1000.f + deplasareMasinaPolitie - 600, -180.f + deplasareMasinaPolitieY}, resizeMatrix);


	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
