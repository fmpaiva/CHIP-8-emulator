#include "Rendering.h"
#include <SDL2/SDL.h>
#include <iostream>

constexpr int SCREEN_WIDTH {640};
constexpr int SCREEN_HEIGHT {320};
SDL_Window* g_window {nullptr};
SDL_Surface* g_screenSurface {nullptr};

bool Rendering::initWindow() {
    bool success {true};
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize. SDL error: \n" << SDL_GetError();
        success = false;
    } else {
	    g_window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	    if (!g_window) {
	        std::cout << "Window could not be created. SDL error: \n" << SDL_GetError();
	        success = false;
	    } else {
	        g_screenSurface = SDL_GetWindowSurface(g_window);
	    }
    }

    return success;
}
