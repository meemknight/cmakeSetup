#https://github.com/meemknight/cmakeSetup

cmake_minimum_required(VERSION 3.13)
project(mygame)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glfw-3.3.2)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/stb_image)
add_subdirectory(thirdparty/stb_truetype)
add_subdirectory(thirdparty/gl2d)



# Define MY_SOURCES to be a list of all the source files for my game 
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game
#add_definitions(-DRESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


if(MSVC) # If using the VS compiler...

	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	
endif()

if(WIN32)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PLATFORM_WIN32="1") 
  
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/gl2d/include")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glfw glad stb_image stb_truetype gl2d)

