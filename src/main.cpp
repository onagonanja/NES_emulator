#include "header/Bus.hpp"
#include "header/CPU.hpp"
#include "header/PPU.hpp"
#include "header/defs.hpp"
#include "header/Logger.hpp"

#include <SDL.h>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <vector>

int main(int argc, char *argv[]) {
  if(SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Quit();
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_X_MIN * PIXEL_SIZE, SCREEN_Y_MIN * PIXEL_SIZE, 0);

  SDL_Renderer *gRenderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if(window == NULL) {
    SDL_Quit();
    return 1;
  }

  NES::Logger::clearLog();

  bool quit = false;

  NES::Bus bus = NES::Bus();
  NES::CPU cpu = NES::CPU(bus);
  NES::PPU ppu = NES::PPU(bus);

  bus.readROM();
  cpu.reset();
  
  // mainloop
  while(!quit) {
    SDL_RenderClear(gRenderer);
    SDL_Event e;

    while(SDL_PollEvent(&e)) {
      if(SDL_QUIT == e.type) {
        quit = true;
        break;
      }
    }

    for(int i = 0; i < 1000; i++) {
      cpu.run();
    }

    std::vector<std::vector<std::vector<int>>> color = ppu.run();

    SDL_Rect fillRect;
    fillRect.h = PIXEL_SIZE;
    fillRect.w = PIXEL_SIZE;
    for(int y = 0; y < SCREEN_Y_MIN; y++) {
      for(int x = 0; x < SCREEN_X_MIN; x++) {
        fillRect.x = x * PIXEL_SIZE;
        fillRect.y = y * PIXEL_SIZE;
        SDL_SetRenderDrawColor(gRenderer, color[y][x][0], color[y][x][1],
                               color[y][x][2], 0);
        SDL_RenderFillRect(gRenderer, &fillRect);
      }
    }
    SDL_RenderPresent(gRenderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}