#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <array>

class Interpreter;

constexpr int PIXEL_HEIGHT {32};
constexpr int PIXEL_WIDTH {64};
constexpr int SCREEN_WIDTH {640};
constexpr int SCREEN_HEIGHT {320};
constexpr int CELL_SIZE {SCREEN_WIDTH / PIXEL_WIDTH};

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
