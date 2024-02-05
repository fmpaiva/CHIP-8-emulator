#include "Display.h"
#include <SDL2/SDL.h>
#include <iostream>


Display::Display() {
    // DEBUG
    for (int i {20}; i < PIXEL_HEIGHT; ++i) {
        for (int j {5}; j < PIXEL_WIDTH; ++j) {
            m_display[i][j] = true;
        }
    }

    if (!init()) {
        std::cerr << "Failed to initialize.\n";
        close();
    }
}

Display::~Display() {
    close();
}

void Display::render() {
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_renderer);

    for (int i {0}; i < PIXEL_HEIGHT; ++i) {
        for (int j {0}; j < PIXEL_WIDTH; ++j) {
            if (m_display[i][j]) {
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
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init()", SDL_GetError(), 0);
        success = false;
    } else {
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "SDL_SetHint()",
		        "Linear texture filtering not enabled", 0 
		    );
		}

#ifdef RELEASE
        m_window = SDL_CreateWindow( "CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
#else
        m_window = SDL_CreateWindow( "CHIP-8_debug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
#endif
        if (m_window == nullptr) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow()", SDL_GetError(), 0);
            success = false;
        } else {
            m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

            if (m_renderer == nullptr) {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateRenderer()", SDL_GetError(), 0);
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

