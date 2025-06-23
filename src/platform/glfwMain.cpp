
//enet has to be included before other stuff, otherwise you will get linking errors
//don't forget to also link to it in the cmake (you have to uncomment the add_subdirectory line and also
//  add enet in the target_link_libraries list at the end
//#include <enet/enet.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <stb_truetype/stb_truetype.h>
#include "gl2d/gl2d.h"
#include <iostream>
#include <ctime>
#include "platformTools.h"
#include <raudio.h>
#include "platformInput.h"
#include "otherPlatformFunctions.h"
#include "gameLayer.h"
#include <fstream>
#include <chrono>
#include "errorReporting.h"
#include <stringManipulation.h>

#define REMOVE_IMGUI 0

#if REMOVE_IMGUI == 0
	#include "imgui.h"
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "imguiThemes.h"
	#include "IconsForkAwesome.h"
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#undef min
#undef max

#pragma region globals 
bool currentFullScreen = 0;
bool fullScreen = 0;

#pragma endregion



void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_BACKSPACE)
	{
		platform::internal::addToTypedInput(8);
	}

	bool state = 0;

	if(action == GLFW_PRESS)
	{
		state = 1;
	}else if(action == GLFW_RELEASE)
	{
		state = 0;
	}else
	{
		return;
	}

	if(key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
	{
		int index = key - GLFW_KEY_A;
		platform::internal::setButtonState(platform::Button::A + index, state);
	}else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
	{
		int index = key - GLFW_KEY_0;
		platform::internal::setButtonState(platform::Button::NR0 + index, state);
	}else
	{
	//special keys
		//GLFW_KEY_SPACE, GLFW_KEY_ENTER, GLFW_KEY_ESCAPE, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT

		if (key == GLFW_KEY_SPACE)
		{
			platform::internal::setButtonState(platform::Button::Space, state);
		}
		else
		if (key == GLFW_KEY_ENTER)
		{
			platform::internal::setButtonState(platform::Button::Enter, state);
		}
		else
		if (key == GLFW_KEY_ESCAPE)
		{
			platform::internal::setButtonState(platform::Button::Escape, state);
		}
		else
		if (key == GLFW_KEY_UP)
		{
			platform::internal::setButtonState(platform::Button::Up, state);
		}
		else
		if (key == GLFW_KEY_DOWN)
		{
			platform::internal::setButtonState(platform::Button::Down, state);
		}
		else
		if (key == GLFW_KEY_LEFT)
		{
			platform::internal::setButtonState(platform::Button::Left, state);
		}
		else
		if (key == GLFW_KEY_RIGHT)
		{
			platform::internal::setButtonState(platform::Button::Right, state);
		}
		else
		if (key == GLFW_KEY_LEFT_CONTROL)
		{
			platform::internal::setButtonState(platform::Button::LeftCtrl, state);
		}else
		if (key == GLFW_KEY_TAB)
		{
			platform::internal::setButtonState(platform::Button::Tab, state);
		}else
		if (key == GLFW_KEY_LEFT_SHIFT)
		{
			platform::internal::setButtonState(platform::Button::LeftShift, state);
		}else
		if (key == GLFW_KEY_LEFT_ALT)
		{
			platform::internal::setButtonState(platform::Button::LeftAlt, state);
		}

	}
	
};

void mouseCallback(GLFWwindow *window, int key, int action, int mods)
{
	bool state = 0;

	if (action == GLFW_PRESS)
	{
		state = 1;
	}
	else if (action == GLFW_RELEASE)
	{
		state = 0;
	}
	else
	{
		return;
	}

	if(key == GLFW_MOUSE_BUTTON_LEFT)
	{
		platform::internal::setLeftMouseState(state);
	}else
	if (key == GLFW_MOUSE_BUTTON_RIGHT)
	{
		platform::internal::setRightMouseState(state);
	}
	

}

bool windowFocus = 1;

void windowFocusCallback(GLFWwindow *window, int focused)
{
	if (focused)
	{
		windowFocus = 1;
	}
	else
	{
		windowFocus = 0;
		//if you not capture the release event when the window loses focus,
		//the buttons will stay pressed
		platform::internal::resetInputsToZero();
	}
}

void windowSizeCallback(GLFWwindow *window, int x, int y)
{
	platform::internal::resetInputsToZero();
}

int mouseMovedFlag = 0;

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
	mouseMovedFlag = 1;
}

void characterCallback(GLFWwindow *window, unsigned int codepoint)
{
	if (codepoint < 127)
	{
		platform::internal::addToTypedInput(codepoint);
	}
}

#pragma region platform functions

GLFWwindow *wind = 0;

namespace platform
{

	void setRelMousePosition(int x, int y)
	{
		glfwSetCursorPos(wind, x, y);
	}

	bool isFullScreen()
	{
		return fullScreen;
	}

	void setFullScreen(bool f)
	{
		fullScreen = f;
	}

	glm::ivec2 getFrameBufferSize()
	{
		int x = 0; int y = 0;
		glfwGetFramebufferSize(wind, &x, &y);
		return {x, y};
	}

	glm::ivec2 getRelMousePosition()
	{
		double x = 0, y = 0;
		glfwGetCursorPos(wind, &x, &y);
		return { x, y };
	}

	glm::ivec2 getWindowSize()
	{
		int x = 0; int y = 0;
		glfwGetWindowSize(wind, &x, &y);
		return { x, y };
	}

	//todo test
	void showMouse(bool show)
	{
		if(show)
		{
			glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}else
		{
			glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}

	bool hasFocused()
	{
		return windowFocus;
	}

	bool mouseMoved()
	{
		return mouseMovedFlag;
	}

	bool writeEntireFile(const char *name, void *buffer, size_t size)
	{
		std::ofstream f(name, std::ios::binary);

		if(!f.is_open())
		{
			return 0;
		}

		f.write((char*)buffer, size);

		f.close();

		return 1;
	}

	bool appendToFile(const char *name, void *buffer, size_t s)
	{
		std::ofstream f(name, std::ios::binary | std::ios::app);

		if (f.is_open())
		{
			f.write((char *)buffer, s);
			f.close();
			return true;
		}
		else
		{
			return false;
		}
	}


	bool readEntireFile(const char *name, void *buffer, size_t size)
	{
		std::ifstream f(name, std::ios::binary);

		if (!f.is_open())
		{
			return 0;
		}

		f.read((char *)buffer, size);

		f.close();

		return 1;
	}

	bool readEntireFile(const char *name, std::vector<unsigned char> &buffer)
	{
		std::ifstream f(name, std::ios::binary | std::ios::ate);
		if (!f.is_open())
			{ return false; }

		std::streamsize fileSize = f.tellg();
		f.seekg(0, std::ios::beg);

		buffer.resize(static_cast<size_t>(fileSize));

		if (!f.read((char*)(buffer.data()), fileSize))
		{
			f.close();
			return false;
		}

		f.close();
		return true;
	}

	bool readEntireFile(const char *name, std::vector<char> &buffer)
	{
		std::ifstream f(name, std::ios::binary | std::ios::ate);
		if (!f.is_open())
		{
			return false;
		}

		std::streamsize fileSize = f.tellg();
		f.seekg(0, std::ios::beg);

		buffer.resize(static_cast<size_t>(fileSize));

		if (!f.read((char *)(buffer.data()), fileSize))
		{
			f.close();
			return false;
		}

		f.close();
		return true;
	}

	std::string readEntireFile(const char *name, bool *succeeded)
	{
		if (succeeded) { *succeeded = false; }

		std::ifstream f(name, std::ios::binary | std::ios::ate);
		if (!f.is_open())
		{
			return "";
		}

		std::streamsize fileSize = f.tellg();
		f.seekg(0, std::ios::beg);

		std::string buffer;

		buffer.resize(static_cast<size_t>(fileSize));

		if (!f.read((char *)(buffer.data()), fileSize))
		{
			f.close();
			return "";
		}

		f.close();
		if (succeeded) { *succeeded = true; }
		return buffer;
	}

	size_t getFileSize(const char *name)
	{
		std::ifstream f(name, std::ios::binary | std::ios::ate);
		if (!f.is_open())
			{ return 0; }

		std::streamsize fileSize = f.tellg();
		f.close();
		return fileSize;
	}


};


#pragma endregion


int main()
{

#ifdef _WIN32
#ifdef _MSC_VER 
#if PRODUCTION_BUILD == 0
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
#endif
#endif
#endif


#pragma region window and opengl

	permaAssertComment(glfwInit(), "err initializing glfw");
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif


	int w = 500;
	int h = 500;
	wind = glfwCreateWindow(w, h, "geam", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	glfwSwapInterval(1);

	glfwSetKeyCallback(wind, keyCallback);
	glfwSetMouseButtonCallback(wind, mouseCallback);
	glfwSetWindowFocusCallback(wind, windowFocusCallback);
	glfwSetWindowSizeCallback(wind, windowSizeCallback);
	glfwSetCursorPosCallback(wind, cursorPositionCallback);
	glfwSetCharCallback(wind, characterCallback);

	//permaAssertComment(gladLoadGL(), "err initializing glad");
	permaAssertComment(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "err initializing glad");

	enableReportGlErrors();

#pragma endregion

#pragma region gl2d
	gl2d::init();
#pragma endregion


#pragma region imgui
	#if REMOVE_IMGUI == 0
		ImGui::CreateContext();
		//ImGui::StyleColorsDark();				//you can use whatever imgui theme you like!
		//imguiThemes::yellow();
		//imguiThemes::gray();
		//imguiThemes::green();
		//imguiThemes::red();
		imguiThemes::embraceTheDarkness();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		io.FontGlobalScale = 2; //adjust scale

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 0.f;
			style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
		}
	
		ImGui_ImplGlfw_InitForOpenGL(wind, true);
		ImGui_ImplOpenGL3_Init("#version 330");


		if(1) //load font awesome
		{
			//https://pixtur.github.io/mkdocs-for-imgui/site/FONTS/
			//https://github.com/juliettef/IconFontCppHeaders
			//https://fontawesome.com/v4/icons/

			
			if (1)
			{
				// use the default font
				io.Fonts->AddFontDefault();
			}
			else
			{
				//load custom font
				io.Fonts->AddFontFromFileTTF(RESOURCES_PATH "arial.ttf", 16);
			}
			
		
			
			// Merge FontAwesome into the default font
			ImFontConfig config;
			config.MergeMode = true;
			config.PixelSnapH = true;
			config.GlyphMinAdvanceX = 16.0f; // adjust as needed
			
			static const ImWchar icon_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
			io.Fonts->AddFontFromFileTTF(RESOURCES_PATH "fontawesome-webfont.ttf", 16.0f, &config, icon_ranges);
			
			// Build fonts after all additions
			io.Fonts->Build();
		

			//make one icon larger
			//{
			//	ImVector<ImWchar> ranges;
			//	ImFontGlyphRangesBuilder builder;
			//	builder.AddChar(0xf016);//ICON_FK_FILE_O
			//	builder.AddChar(0xf114);//ICON_FK_FOLDER_O
			//	builder.BuildRanges(&ranges);
			//
			//	io.Fonts->AddFontFromFileTTF(RESOURCES_PATH "fontawesome-webfont.ttf",
			//		150.f / io.FontGlobalScale, 0, ranges.Data);
			//}

		}


	#endif
#pragma endregion

#pragma region audio
	//InitAudioDevice();

	//Music m = LoadMusicStream(RESOURCES_PATH "target.ogg");
	//Music m = {};
	//UpdateMusicStream(m);
	//StopMusicStream(m);
	//PlayMusicStream(m);

#pragma endregion

#pragma region initGame
	if (!initGame())
	{
		return 0;
	}
#pragma endregion


	//long lastTime = clock();
	
	auto stop = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(wind))
	{
		//UpdateMusicStream(m);
		//PlayMusicStream(m);

	#pragma region deltaTime

		//long newTime = clock();
		//float deltaTime = (float)(newTime - lastTime) / CLOCKS_PER_SEC;
		//lastTime = clock();
		auto start = std::chrono::high_resolution_clock::now();

		float deltaTime = (std::chrono::duration_cast<std::chrono::nanoseconds>(start - stop)).count() / 1000000000.0;
		stop = std::chrono::high_resolution_clock::now();

		float augmentedDeltaTime = deltaTime;
		if (augmentedDeltaTime > 1.f / 10) { augmentedDeltaTime = 1.f / 10; }
	
	#pragma endregion

	#pragma region imgui
		#if REMOVE_IMGUI == 0
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		#endif
	#pragma endregion

	#pragma region game logic

		platform::Input input = {};

		input.deltaTime = augmentedDeltaTime;
		input.hasFocus = platform::hasFocused();
		memcpy(input.buttons, platform::getAllButtons(), sizeof(input.buttons));
		input.controller = platform::getControllerButtons();
		input.mouseX = platform::getRelMousePosition().x;
		input.mouseY = platform::getRelMousePosition().y;
		input.lMouse = platform::getLMouseButton();
		input.rMouse = platform::getRMouseButton();
		strlcpy(input.typedInput, platform::getTypedInput(), sizeof(input.typedInput));

		if (!gameLogic(augmentedDeltaTime, input))
		{
			closeGame();
			return 0;
		}

	#pragma endregion


	#pragma region fullscreen 

		if (platform::hasFocused() && currentFullScreen != fullScreen)
		{
			static int lastW = w;
			static int lastH = w;
			static int lastPosX = 0;
			static int lastPosY = 0;

			if (fullScreen)
			{
				lastW = w;
				lastH = h;

				//glfwWindowHint(GLFW_DECORATED, NULL); // Remove the border and titlebar..  
				glfwGetWindowPos(wind, &lastPosX, &lastPosY);


				//auto monitor = glfwGetPrimaryMonitor();
				auto monitor = getCurrentMonitor(wind);


				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				// switch to full screen
				glfwSetWindowMonitor(wind, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

				currentFullScreen = 1;

			}
			else
			{
				//glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // 
				glfwSetWindowMonitor(wind, nullptr, lastPosX, lastPosY, lastW, lastH, 0);

				currentFullScreen = 0;
			}

		}

	#pragma endregion

	#pragma region reset flags

		mouseMovedFlag = 0;
		platform::internal::updateAllButtons(deltaTime);
		platform::internal::resetTypedInput();

	#pragma endregion

	#pragma region window stuff

		#pragma region imgui
				#if REMOVE_IMGUI == 0
				ImGui::Render();
				int display_w, display_h;
				glfwGetFramebufferSize(wind, &display_w, &display_h);
				glViewport(0, 0, display_w, display_h);
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Update and Render additional Platform Windows
				// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
				//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					GLFWwindow* backup_current_context = glfwGetCurrentContext();
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
					glfwMakeContextCurrent(backup_current_context);
				}
			#endif
		#pragma endregion

		glfwSwapBuffers(wind);
		glfwPollEvents();

	#pragma endregion

	}

	closeGame();

	//if you want the console to stay after closing the window
	//std::cin.clear();
	//std::cin.get();
}