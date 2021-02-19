# cmakeSetup

---

This is a cmake setup that loads glfw, opengl, stb_image, stb_truetype and a custom 2d library. It works both on windows and linux.

![](https://github.com/meemknight/photos/blob/master/cmakeSetup1.png)
---

# Configurations:

  1. In Cmakelists.txt you can set the RESOURCES_PATH macro to the root folder for sharing the compiled program. By default it is the absolute path to the assets folder.
  2. In toold.s you cand change INTERNAL_BUILD to 0 when sharing the exe so the custom asserts don't let the user to continue debugging or using the program after a failed assertion.
