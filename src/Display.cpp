#include "Display.h"
#include <SDL2/SDL.h>
#include <iostream>


Display::Display() { 
    clearScreen();
    if (!init()) {
        std::cerr << "Failed to initialize.\n";
        close();
    }
}

Display::~Display() {
    close();
}

void Display::clearScreen() {
    for (int i {0}; i < PIXEL_HEIGHT; ++i) {
        for (int j {0}; j < PIXEL_WIDTH; ++j) {
            m_data[i][j] = false;
        }
    } 
}

bool Display::isOn(int row, int column) {
    return m_data[row][column];
}

void Display::toggle(int row, int column) {
    m_data[row][column] = !m_data[row][column];
}

void Display::render() {
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_renderer);

    for (int i {0}; i < PIXEL_HEIGHT; ++i) {
        for (int j {0}; j < PIXEL_WIDTH; ++j) {
            if (m_data[i][j]) {
                SDL_Rect cellRect {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderFillRect(m_renderer, &cellRect);
            }
        }
    }

    SDL_RenderPresent(m_renderer);
}

bool Display::init() {
    bool success {true};

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to SDL_Init().\n" << SDL_GetError();
        success = false;
    } else {
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		    std::cerr << "Warning: Linear texture filtering not enabled";
		}

#ifdef RELEASE
        m_window = SDL_CreateWindow( "CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
#else
        m_window = SDL_CreateWindow( "CHIP-8_debug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
#endif
        if (m_window == nullptr) {
            std::cerr << "Failed to create window.\n" << SDL_GetError();
            success = false;
        } else {
            m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

            if (m_renderer == nullptr) {
                std::cerr << "Failed to create renderer.\n" << SDL_GetError();
                success = false;
            } else {
				SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
            }
        }
    }

    return success;
}

void Display::close() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_renderer = nullptr;
    m_window = nullptr;
    SDL_Quit();
}

