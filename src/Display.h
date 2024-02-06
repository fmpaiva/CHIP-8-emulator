#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <array>
#include "Constants.h"

using namespace Constants;

class Display {
public:
    Display();
    ~Display();
    void render();
    void clearScreen();
    bool isOn(int, int);
    void toggle(int, int);

private:
    SDL_Window* m_window {nullptr};
    SDL_Renderer* m_renderer {nullptr};
    std::array<std::array<bool, PIXEL_WIDTH>, PIXEL_HEIGHT> m_data {}; 

    bool init();
    void close();
};

#endif
