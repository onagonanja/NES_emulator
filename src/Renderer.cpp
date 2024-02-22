#include "header/Renderer.hpp"
#include "header/defs.hpp"

namespace NES {
  Renderer::Renderer(ScreenBuff &screenBuff) : screenBuff(screenBuff) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_X_MIN * PIXEL_SIZE,
                              SCREEN_Y_MIN * PIXEL_SIZE, 0);
    gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(gRenderer);
  }

  void Renderer::render() {
    SDL_Rect fillRect;
    fillRect.h = PIXEL_SIZE;
    fillRect.w = PIXEL_SIZE;

    for(int y = 0; y < SCREEN_Y_MIN; y++) {
      for(int x = 0; x < SCREEN_X_MIN; x++) {
        fillRect.x = x * PIXEL_SIZE;
        fillRect.y = y * PIXEL_SIZE;

        SDL_SetRenderDrawColor(gRenderer, screenBuff[y][x][0], screenBuff[y][x][1], screenBuff[y][x][2], 0);
        SDL_RenderFillRect(gRenderer, &fillRect);
      }
    }
    SDL_RenderPresent(gRenderer);
    showFPS();
  }

  void Renderer::close() {
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  void Renderer::showFPS() {
    clock_t now = clock();
    //std::cout << "FPS: " << 1.0 / ((now - time) / (double)CLOCKS_PER_SEC) << std::endl;
    time = now;
  }
} // namespace NES