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


struct GameData
{


}gameData;



GLuint vertex;
ShaderProgram s;
GLint u_color;
GLint u_mat;

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
		0, 0,
		1, 0,
		1, 1,

		0, 0,
		1, 1,
		0, 1
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


bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w= platform::getWindowSizeX();
	h = platform::getWindowSizeY();
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);

#pragma endregion

	static float sizeX = 100;
	static float sizeY = 100;

	ImGui::Begin("test");

	ImGui::SliderFloat("slider x", &sizeX, 1, 200);
	ImGui::SliderFloat("slider y", &sizeY, 1, 200);

	ImGui::End();


	glm::mat4 resizeMatrix = glm::ortho(-(float)w, (float)w, -(float)h, (float)h);
	glm::mat4 t = glm::scale(glm::mat4(1.f), {sizeX,sizeY,0.f});
	glm::mat4 mat = resizeMatrix * t;

	renderRect({0,0,1}, mat);
		

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
