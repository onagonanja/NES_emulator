#ifndef INCLUDED_EMULATOR
#define INCLUDED_EMULATOR

#include "Bus.hpp"
#include "CPU.hpp"
#include "Controller.hpp"
#include "PPU.hpp"
#include "Renderer.hpp"

#include <SDL.h>
#include <vector>

namespace NES {
  class Emulator {
  public:
    Emulator(char *romPath);

    void run();

    void handleKeyDOWN(SDL_Event &event);
    void handleKeyUP(SDL_Event &event);

    void getTestCode();

  private:
    CPU cpu;
    PPU ppu;
    Bus bus;
    ScreenBuff screenBuff;
    Controller controller;
    Renderer renderer;
    SDL_Event e;
  };
} // namespace NES

#endif