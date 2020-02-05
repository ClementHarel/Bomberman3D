cmake_minimum_required(VERSION 3.10)    # CMake version check
project(bomberman)                 # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)              # Enable c++11 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES ./src/main.cpp ./src/IndieStudio.cpp ./src/Title.cpp ./src/MapEditor/BomberMap.cpp ./src/MapEditor/MapEditor.cpp ./src/Item.cpp ./src/Items/Bomb.cpp ./src/Items/Bonus.cpp ./src/Game.cpp ./src/Player.cpp ./src/FX/Explosion.cpp)
find_library(X11R6 NAMES Xxf86vm Xext X11)
find_library(IRRLICHT Irrlicht PATHS ${CMAKE_SOURCE_DIR}/src)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(bomberman ${SOURCE_FILES})
target_link_libraries(bomberman ${IRRLICHT} ${X11R6} -lGL -lXxf86vm -lXext -lX11 -lsfml-audio)
