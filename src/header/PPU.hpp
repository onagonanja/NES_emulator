#ifndef INCLUDED_PPU
#define INCLUDED_PPU

#include "defs.hpp"
#include "bus.hpp"

#include <vector>

class CPU;

namespace NES {
  class PPU {
  private:
    Bus &bus;

  public:
    PPU(Bus &b);

    std::vector<std::vector<std::vector<int>>> run();

    std::vector<Byte> getBackGroundColor(Byte data);

    std::vector<Byte> getSpriteColor(Byte data);
  };
} // namespace NES

#endif