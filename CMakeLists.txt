
# cmake settings
cmake_minimum_required(VERSION 3.5)
project(dreamray
	DESCRIPTION "raycaster engine"
	HOMEPAGE_URL "https://github.com/erysdren/dreamray/"
	LANGUAGES C
	VERSION 0.0.1
)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/external/sdl2-cmake-modules")

# sdl2
find_package(SDL2 REQUIRED)

# eui
option(EUI_BUILD_EXAMPLES "Build Example Applications" OFF)
option(EUI_INSTALL_TARGET "Enable Install Target" OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/eui)

# libmath
find_library(LIBMATH "m")

# executable
add_executable(dreamray
	${PROJECT_SOURCE_DIR}/source/main.c
	${PROJECT_SOURCE_DIR}/source/dreamray.c
	${PROJECT_SOURCE_DIR}/source/console.c
	${PROJECT_SOURCE_DIR}/source/menu.c
	${PROJECT_SOURCE_DIR}/source/game.c)
target_link_libraries(dreamray PUBLIC SDL2::Main ${LIBMATH} eui_sdl2)
add_custom_target(dreamray_copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/gfx ${PROJECT_BINARY_DIR}/gfx)
add_dependencies(dreamray dreamray_copy_assets)
