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

  private:
    CPU cpu;
    PPU ppu;
    Bus bus;
  };
} // namespace NES

#endif