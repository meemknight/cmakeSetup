#pragma once
#include "gl2d/gl2d.h"
#include <string>


namespace glui
{

	void gluiInit();

	void defaultErrorFunc(const char* msg);
	using errorFuncType = decltype(defaultErrorFunc);

	void renderFrame(
	gl2d::Renderer2D &renderer,
	gl2d::Font &font,
	glm::ivec2 mousePos,
	bool mouseClick,
	bool mouseHeld,
	bool mouseReleased,
	bool escapeReleased,
	const std::string &typedInput,
	float deltaTime
	);

	bool Button(std::string name,
		const gl2d::Color4f colors, const gl2d::Texture texture = {});

	void Texture(int id, gl2d::Texture t, gl2d::Color4f colors = {1,1,1,1}, glm::vec4 textureCoords = {0,1,1,0});

	bool ButtonWithTexture(int id, gl2d::Texture t, gl2d::Color4f colors = {1,1,1,1}, glm::vec4 textureCoords = {0,1,1,0});

	bool Toggle(std::string name,
		const gl2d::Color4f colors, bool* toggle, const gl2d::Texture texture = {}, const gl2d::Texture overTexture = {});

	//returns true if you should render it, clicked is optional
	bool CustomWidget(int id, glm::vec4 *transform, bool *hovered = 0, bool *clicked = 0);

	void Text(std::string name,
		const gl2d::Color4f colors);

	void InputText(std::string name,
		char* text, size_t textSizeWithNullChar, gl2d::Color4f color = {0,0,0,0}, const gl2d::Texture texture = {});

	void sliderFloat(std::string name, float *value, float min, float max, 
		gl2d::Texture sliderTexture = {}, gl2d::Color4f sliderColor = {1,1,1,1},
		gl2d::Texture ballTexture = {}, gl2d::Color4f ballColor = {1,1,1,1} );

	void sliderInt(std::string name, int *value, int min, int max,
		gl2d::Texture sliderTexture = {}, gl2d::Color4f sliderColor = {1,1,1,1},
		gl2d::Texture ballTexture = {}, gl2d::Color4f ballColor = {1,1,1,1});

	void colorPicker(std::string name, float *color3Component, gl2d::Texture sliderTexture = {},
		gl2d::Texture ballTexture = {});

	void newColum(int id);

	void PushId(int id);

	void PopId();

	void BeginMenu(std::string name, const gl2d::Color4f colors, const gl2d::Texture texture);
	void EndMenu();

	void Begin(int id);
	void End();

};