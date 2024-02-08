#ifndef INCLUDED_PPU
#define INCLUDED_PPU

#include "bus.hpp"
#include "defs.hpp"

#include <vector>

class CPU;

namespace NES {
  using ScreenBuff = std::vector<std::vector<std::vector<int>>>;

  class PPU {
  private:
    Bus &bus;
    ScreenBuff &screenBuff;

  public:
    PPU(Bus &b, ScreenBuff &screenBuff);

    void run();

    std::vector<Byte> getBackGroundColor(Byte data);

    std::vector<Byte> getSpriteColor(Byte data);
  };
} // namespace NES

#endif