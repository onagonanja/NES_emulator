#ifndef INCLUDED_BUS
#define INCLUDED_BUS

#include "defs.hpp"

#include <vector>

namespace NES {

  const int RAM_SIZE = 65536;
  const int VRAM_SIZE = 16384;
  const int SPRITERAM_SIZE = 256;
  const int SPRITE_SIZE = 64;

  class Bus {
  private:
    std::vector<Byte> ram = std::vector<Byte>(RAM_SIZE, 0);
    std::vector<Byte> vram = std::vector<Byte>(VRAM_SIZE, 0);
    std::vector<Byte> spriteram = std::vector<Byte>(SPRITERAM_SIZE, 0);
    std::vector<std::vector<Byte>> characterRom;

    Address vram_write_addr = 0;
    bool w = false;

    Byte scroll_x = 0;
    Byte scroll_y = 0;

  public:
    void readROM();

    Byte readRAM(Address addr);
    void writeRAM(Address addr, Byte data);

    Byte readVRAM(Address addr);
    void writeVRAM(Address addr, Byte data);

    Byte readSpriteRAM(Address addr);
    void writeSpriteRAM(Address addr, Byte data);

    std::vector<Byte> readCharacterROM(Address addr);
    void writeCharacterROM(Address addr, std::vector<Byte> data);

    Byte getScrollX();
    Byte getScrollY();
  };
} // namespace NES

#endif