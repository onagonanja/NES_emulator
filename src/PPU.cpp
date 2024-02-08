#include "defs.hpp"
#include "header/CPU.hpp"
#include "header/PPU.hpp"
#include "header/Pallete.hpp"

#include <iomanip>
#include <iostream>

namespace NES {

  PPU::PPU(Bus &b) : bus(b) {}

  std::vector<Byte> PPU::getBackGroundColor(Byte data) {
    std::vector<Byte> res(4);
    res[0] = bus.readVRAM(0x3f00 + data * 4);
    res[1] = bus.readVRAM(0x3f00 + data * 4 + 1);
    res[2] = bus.readVRAM(0x3f00 + data * 4 + 2);
    res[3] = bus.readVRAM(0x3f00 + data * 4 + 3);
    return res;
  }

  std::vector<Byte> PPU::getSpriteColor(Byte data) {
    std::vector<Byte> res(4);
    res[0] = bus.readVRAM(0x3f10 + data * 4);
    res[1] = bus.readVRAM(0x3f10 + data * 4 + 1);
    res[2] = bus.readVRAM(0x3f10 + data * 4 + 2);
    res[3] = bus.readVRAM(0x3f10 + data * 4 + 3);
    return res;
  }

  // vramからピクセルデータを取得してそのデータを返す
  std::vector<std::vector<std::vector<int>>> PPU::run() {
    /*------------------Render BackGround------------------------*/

    Byte nameTable[30][32]; //[縦][横]
    const int NAMETABLENUM =
        ((bus.readRAM(0x2000) & 1) << 1) * 2 + (bus.readRAM(0x2000) & 1);
    const Address NAMETABLESTART[4] = {0x2000, 0x2400, 0x2800, 0x2C00};

    for(int i = 0; i < 30; i++) {
      for(int j = 0; j < 32; j++) {
        nameTable[i][j] =
            bus.readVRAM(NAMETABLESTART[NAMETABLENUM] + i * 32 + j);
      }
    }

    Byte paretTable[15][16]; //[縦][横]
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
        int data = bus.readVRAM(0x23C0 + i * 8 + j);
        paretTable[i * 2][j * 2] = data & 0b11;
        paretTable[i * 2][j * 2 + 1] = (data & 0b1100) >> 2;
        if(i * 2 + 1 < 15) {
          paretTable[i * 2 + 1][j * 2] = (data & 0b110000) >> 4;
          paretTable[i * 2 + 1][j * 2 + 1] = (data & 0b11000000) >> 6;
        }
      }
    }

    std::vector<std::vector<std::vector<int>>> res(
        30 * 8, std::vector<std::vector<int>>(32 * 8, std::vector<int>(3)));

    for(int i = 0; i < 30; i++) {
      for(int j = 0; j < 32; j++) {
        Byte paletteNUM = paretTable[i / 2][j / 2];
        std::vector<Byte> sprite = bus.readCharacterROM(nameTable[i][j]);
        std::vector<Byte> pallete = getBackGroundColor(paletteNUM);
        for(int di = 0; di < 8; di++) {
          for(int dj = 0; dj < 8; dj++) {
            res[i * 8 + di][j * 8 + dj][0] =
                colors[pallete[sprite[di * 8 + dj]]][0];
            res[i * 8 + di][j * 8 + dj][1] =
                colors[pallete[sprite[di * 8 + dj]]][1];
            res[i * 8 + di][j * 8 + dj][2] =
                colors[pallete[sprite[di * 8 + dj]]][2];
          }
        }
      }
    }

    /*----------------------Render Sprite------------------------*/

    for(int i = 0; i < 256; i += 4) {
      if(bus.readSpriteRAM(i) >= 0xff) {
        continue;
      }
      int y = bus.readSpriteRAM(i) + 1;
      int x = bus.readSpriteRAM(i + 3);
      int palletenum = bus.readSpriteRAM(i + 2) & 0b11;
      std::vector<Byte> sprite = bus.readCharacterROM(bus.readSpriteRAM(i + 1));
      std::vector<Byte> pallete = getSpriteColor(palletenum);
      for(int dy = 0; dy < 8; dy++) {
        for(int dx = 0; dx < 8; dx++) {
          int absY = y + dy;
          int absX = x + dx;
          if(absX >= 240 || absY >= 256) {
            continue;
          }
          res[absX][absY][0] = colors[pallete[sprite[dx * 8 + dy]]][0];
          res[absX][absY][1] = colors[pallete[sprite[dx * 8 + dy]]][1];
          res[absX][absY][2] = colors[pallete[sprite[dx * 8 + dy]]][2];
        }
      }
    }

    return res;
  }
} // namespace NES