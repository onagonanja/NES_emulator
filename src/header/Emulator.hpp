#ifndef INCLUDED_EMULATOR
#define INCLUDED_EMULATOR

#include "Bus.hpp"
#include "CPU.hpp"
#include "PPU.hpp"

#include <vector>

namespace NES {
  class Emulator {
  public:
    Emulator(ScreenBuff &screenBuff);

    void run();

    void handleKeyDOWN(SDL_Event &event);
    void handleKeyUP(SDL_Event &event);

    void getTestCode();

  private:
    CPU cpu;
    PPU ppu;
    Bus bus;
    
  };
} // namespace NES

#endif