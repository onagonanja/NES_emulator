#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER

#include "Logger.hpp"
#include "PPU.hpp"
#include "defs.hpp"

#include <SDL.h>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <vector>
#include <time.h>

namespace NES {
  class Renderer {
  public:
    Renderer(ScreenBuff &screenBuff);
    void render();
    void close();
    void showFPS();

  private:
    SDL_Window *window;
    SDL_Renderer *gRenderer;
    ScreenBuff &screenBuff;
    clock_t time = 0;
  };
} // namespace NES

#endif