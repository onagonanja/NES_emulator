#include "defs.hpp"
#include <vector>

namespace NES {

  const int RAM_SIZE = 65536;
  const int SPRITE_SIZE = 64;

  class Bus {
  private:
    std::vector<Byte> ram = std::vector<Byte>(RAM_SIZE, 0);
    std::vector<std::vector<Byte>> characterRom;

  public:
    void readROM();
    Byte readRAM(Address addr);
  };
} // namespace NES