# cmakeSetup

---

## What is it?

This is a CMake setup that loads glfw, OpenGL, stb_image, stb_truetype, and a custom 2d library. It works both on Windows and Linux.

## Why use cmakeSetup?

All I do is configure the project in a good way. It is cross-platform, easy to add libraries, and the input and window opening are implemented. All the rest is up to you, with no weird abstractions, you get the init() update(), and close() functions and you do whatever you want with them.


![](https://github.com/meemknight/photos/blob/master/cmakeSetup1.png)
---

  [FULL EXAMPLE VIDEO!](https://www.youtube.com/watch?v=zJoXMfCI9LM)
  
  [LONGER VERSION!!](https://www.youtube.com/watch?v=XOs2qynEmNE)
  
  [VIDEO ABOUT THE SETUP](https://www.youtube.com/watch?v=K8f73k9HM8M)

---

<p>Opening the Solution:</p> 

<img src="https://raw.githubusercontent.com/meemknight/photos/master/llge1.gif" width="350">

Or

<img src="https://raw.githubusercontent.com/meemknight/photos/master/llge2.gif" width="500">

Running the setup

Go to CMakeLists.txt, <kbd>CTRL + S</kbd> to make sure the solution was built.

Then, from this dropdown select mygame.exe

<img src="https://raw.githubusercontent.com/meemknight/photos/master/llge3.gif" width="200">

<kbd>Ctrl + F5</kbd> to build (<kbd>F5</kbd> oppens the debugger, you usually want to press <kbd>Ctrl + F5</kbd> because it oppens faster like this.

<p>Adding files:<br>
You should add .cpp in src/gameLayer and .h in include/gameLayer. Whenever you add a new file CMake will ask you if you want to add that thing, say NO every time! I am already adding all of the things.
If you accidentally say YES, just remove that file from the CMake.lists
</p>

<p>Refreshing your changes:<br>
After you add a file, the changes should be automatically added but if you want to be sure, you can refresh changes by saving the CMake file. If you want to make a hard refresh (you might have to do that sometimes) close Visual Studio, delete the out folder, reopen VS, <kbd>CTRL + S</kbd> on CMakeLists.txt</p>


# How to use:

  gameLayer.cpp has the game main loop. Add your files in scr/gamelayer and include/gamelayer.
  Look at the example provided to see how to acces user input.
  Also you have to use the RESOURCES_PATH macro to get to your assets path.

  [FULL EXAMPLE VIDEO!](https://www.youtube.com/watch?v=zJoXMfCI9LM)


# IMPORTANT!
  To ship the game: 
  In Cmakelists.txt, set the PRODUCTION_BUILD flag to ON to build a shippable version of your game. This will change the file paths to be relative to your exe (RESOURCES_PATH macro), will remove the console, and also will change the aserts to not allow people to debug them. To make sure the changes take effect I recommend deleting the out folder to make a new clean build!
