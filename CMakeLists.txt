cmake_minimum_required(VERSION 3.12)
project(Snake)

include(FindSDL2_ttf.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Cmake")

set(SDL2_INCLUDE_DIR /Library/Frameworks/SDL2.framework/)
set(SDL2_LIBRARY:FILEPATH=/Library/Frameworks/SDL2.framework)
set(SDL2_LIBRARY:FILEPATH=/Library/Frameworks/SDL2_ttf.framework/)

set(SDL_TTF_INCLUDE_DIRS /Library/Frameworks/SDL2_ttf.framework/)
set(SDL_TTF_LIBRARIES /Library/Frameworks/SDL2_ttf.framework)

include_directories(.)
include_directories (${SDL2_INCLUDE_DIR})
include_directories (${SDL2_TTF_INCLUDE_DIR})

find_library(SDL2_LIBRARY SDL2)
find_library(SDL_TTF_LIBRARIES SDL2_ttf)

add_executable(${PROJECT_NAME}
        SnakeGame.cpp
        SnakeGame.h
        main.cpp
        SnakeBoard.cpp
        SnakeBoard.h
        Snake.cpp Snake.h)

message(SDL_LIBRARY: ${SDL2_LIBRARY})
message(SDL_TTF_LIBRARIES: ${SDL2_TTF_LIBRARIES})
message(SDL_TTF_found: ${SDL_TTF_FOUND})

target_link_libraries(Snake ${SDL2_LIBRARY})
target_link_libraries(Snake ${SDL_TTF_LIBRARIES})

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})