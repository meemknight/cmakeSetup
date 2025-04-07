#pragma once
#include <glm/vec2.hpp>
#include <platform/platformInput.h>
#include <vector>
#include <string>

bool initGame();
bool gameLogic(float deltaTime, platform::Input &input);
void closeGame();

namespace platform
{
	///sets the mouse pos relative to the window's drawing area
	void setRelMousePosition(int x, int y);

	bool isFullScreen();
	void setFullScreen(bool f);

	//gets the window size, can be different to the actual framebuffer size
	//you should use getFrameBufferSize if you want the actual ammount of pixels to give to your opengl routines
	glm::ivec2 getWindowSize();

	inline int getWindowSizeX() { return getWindowSize().x; }
	inline int getWindowSizeY() { return getWindowSize().y; }

	//usually is the same as getWindowSize unless you have things like zoom or rezolution.
	//You should use this function if you want to pass this data to glviewport
	glm::ivec2 getFrameBufferSize();

	inline int getFrameBufferSizeX() { return getFrameBufferSize().x; }
	inline int getFrameBufferSizeY() { return getFrameBufferSize().y; }


///gets the mouse pos relative to the window's drawing area
	glm::ivec2 getRelMousePosition();

	void showMouse(bool show);
	bool hasFocused();
	bool mouseMoved();

	bool writeEntireFile(const char *name, void *buffer, size_t size);
	bool readEntireFile(const char *name, void *buffer, size_t size);
	bool readEntireFile(const char *name, std::vector<unsigned char> &buffer);
	bool readEntireFile(const char *name, std::vector<unsigned char> &buffer);
	std::string readEntireFile(const char *name, bool *succeeded = nullptr);
	bool appendToFile(const char *name, void *buffer, size_t s);
	size_t getFileSize(const char *name);

};

