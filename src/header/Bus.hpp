#ifndef INCLUDED_BUS
#define INCLUDED_BUS

#include "defs.hpp"

#include <SDL.h>
#include <functional>
#include <unordered_map>
#include <vector>

namespace NES {

  const int RAM_SIZE = 65536;
  const int VRAM_SIZE = 16384;
  const int SPRITERAM_SIZE = 256;
  const int SPRITE_SIZE = 64;

  const int NAMETABLE_X = 32;
  const int NAMETABLE_Y = 30;

  enum NAMETABLE_MIRRORRING { HORIZONAL, VERTICAL };

  class Bus {
  private:
    Byte ram[RAM_SIZE] = {0};
    Byte vram[VRAM_SIZE] = {0};
    Byte spriteram[SPRITERAM_SIZE] = {0};
    Byte nametable[30 * 2][32 * 2] = {0};
    std::vector<std::vector<Byte>> characterRom;

    Address vram_write_addr = 0;
    Address sprite_ram_write_addr = 0;
    bool w = false;

    Byte scroll_x = 0;
    Byte scroll_y = 0;

    Byte banks;

    NAMETABLE_MIRRORRING mirrorring = HORIZONAL;

    std::unordered_map<Address, std::function<Byte(void)>> readCallbacks;
    std::unordered_map<Address, std::function<void(Byte)>> writeCallbacks;

  public:
    void handleKeyDOWN(SDL_Event &event);
    void handleKeyUP(SDL_Event &event);

    void readROM(char *romPath);

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

    Byte getNametable(int y, int x);

    void setReadCallback(Address addr, std::function<Byte(void)> func);
    void setWriteCallback(Address addr, std::function<void(Byte)> func);
  };
} // namespace NES

#endif