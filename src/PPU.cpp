#include "header/PPU.hpp"
#include "defs.hpp"
#include "header/CPU.hpp"
#include "header/Pallete.hpp"

#include <iomanip>
#include <iostream>

namespace NES {

  PPU::PPU(Bus &b, ScreenBuff &sBuff) : bus(b), screenBuff(sBuff) {}

  std::vector<Byte> PPU::getBackGroundPallet(Byte data) {
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

  void PPU::setPPUState() {
    currentDrawPixcel = (currentDrawPixcel + 1) % (CLOCKS_PER_LINE * SCANLINES_PER_FRAME);
    const int x = currentDrawPixcel % CLOCKS_PER_LINE;
    const int y = currentDrawPixcel / CLOCKS_PER_LINE;

    if(y == SCREEN_Y_WIDTH) {
      state = PostRender;
    } else if(y == SCANLINES_PER_FRAME - 1) {
      state = PreRender;
    } else if(y > SCREEN_Y_WIDTH) {
      state = VBlank;
    } else if(x >= SCREEN_X_WIDTH) {
      state = Hblank;
    } else {
      state = Render;
    }
  }

  int PPU::getSpriteNum(int x, int y) {
    int nameTableNum = ((bus.readRAM(0x2000) >> 1) & 1) * 2 + (bus.readRAM(0x2000) & 1);

    int sprite_X = x / 8;
    int sprite_Y = y / 8;

    return bus.readVRAM(NAMETABLESTART[nameTableNum] + sprite_Y * 32 + sprite_X);
  }

  void PPU::setPixelColor(int x, int y, const int *color) {
    screenBuff[y][x][0] = color[0];
    screenBuff[y][x][1] = color[1];
    screenBuff[y][x][2] = color[2];
  }

  void PPU::drawBgPixel(int x, int y) {
    if(x >= SCREEN_X_WIDTH || y >= SCREEN_Y_WIDTH) {
      return;
    }

    int spriteNum = getSpriteNum(x, y);
    int palletNum = getPalletNum(x, y);

    std::vector<Byte> sprite = bus.readCharacterROM(spriteNum);
    std::vector<Byte> pallete = getBackGroundPallet(palletNum);

    int INSIDE_SPRITE_X = x % 8;
    int INSIDE_SPRITE_Y = y % 8;

    setPixelColor(x, y, colors[pallete[sprite[INSIDE_SPRITE_Y * 8 + INSIDE_SPRITE_X]]]);
  }

  // FIXME: get only critical pallete table
  int PPU::getPalletNum(int x, int y) {
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
    return paretTable[y / 16][x / 16];
  }

  // for dev
  void PPU::drawAll() {
    /*---------------------Render BackGround------------------------*/

    Byte nameTable[30][32]; //[縦][横]
    const int NAMETABLENUM = ((bus.readRAM(0x2000) >> 1) & 1) * 2 + (bus.readRAM(0x2000) & 1);

    const int scroll_x = bus.getScrollX();
    const int scroll_y = bus.getScrollY();

    const int base_x = scroll_x + (NAMETABLENUM % 2) * NAMETABLE_X;
    const int base_y = scroll_y + (NAMETABLENUM / 2) * NAMETABLE_Y;

    for(int y = 0; y < 30; y++) {
      for(int x = 0; x < 32; x++) {
        nameTable[y][x] = bus.getNametable(base_y + y, base_x + x);
        // nameTable[i][j] =  bus.readVRAM(NAMETABLESTART[NAMETABLENUM] + i * 32 + j);
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

    int bgPattrnTableAddr = !!(bus.readRAM(0x2000) & (1 << 4)) ? 0x100 : 0x0000;

    for(int i = 0; i < 30; i++) {
      for(int j = 0; j < 32; j++) {
        Byte paletteNUM = paretTable[i / 2][j / 2];
        std::vector<Byte> sprite = bus.readCharacterROM(bgPattrnTableAddr + nameTable[i][j]);
        std::vector<Byte> pallete = getBackGroundPallet(paletteNUM);
        for(int di = 0; di < 8; di++) {
          for(int dj = 0; dj < 8; dj++) {
            screenBuff[i * 8 + di][j * 8 + dj][0] = colors[pallete[sprite[di * 8 + dj]]][0];
            screenBuff[i * 8 + di][j * 8 + dj][1] = colors[pallete[sprite[di * 8 + dj]]][1];
            screenBuff[i * 8 + di][j * 8 + dj][2] = colors[pallete[sprite[di * 8 + dj]]][2];
          }
        }
      }
    }

    /*------------------------------Render Sprite-----------------------------*/

    int spritePatternTableAddr = !!(bus.readRAM(0x2000) & (1 << 3)) ? 0x100 : 0x000;

    for(int i = 0; i < 256; i += 4) {
      if(bus.readSpriteRAM(i) >= 0xEF) {
        continue;
      }
      int y = bus.readSpriteRAM(i) + 1;
      int x = bus.readSpriteRAM(i + 3);
      int palletenum = bus.readSpriteRAM(i + 2) & 0b11;

      if(i < 3) {
        // std::cout << "sprite" << i << " x: " << x << " y: " << y << std::endl;
      }

      // TODO: apply these flags
      bool isBehindBg = !!(bus.readSpriteRAM(i + 2) & 0b00100000);
      bool isReversed = !!(bus.readSpriteRAM(i + 2) & 0b01000000);
      bool isInverted = !!(bus.readSpriteRAM(i + 2) & 0b10000000);

      std::vector<Byte> sprite = bus.readCharacterROM(spritePatternTableAddr + bus.readSpriteRAM(i + 1));
      std::vector<Byte> pallete = getSpriteColor(palletenum);

      for(int dy = 0; dy < 8; dy++) {
        for(int dx = 0; dx < 8; dx++) {
          int absY = y + dy;
          int absX = x + dx;

          if(absY >= 240 || absX >= 256) {
            continue;
          }
          
          if(sprite[dy * 8 + dx] == 0) {
            continue;
          }
          
          screenBuff[absY][absX][0] = colors[pallete[sprite[dy * 8 + dx]]][0];
          screenBuff[absY][absX][1] = colors[pallete[sprite[dy * 8 + dx]]][1];
          screenBuff[absY][absX][2] = colors[pallete[sprite[dy * 8 + dx]]][2];
        }
      }
    }
  }

  // run PPU
  void PPU::run() {
    switch(state) {
    case PreRender:
      // vblank flag reset
      bus.writeRAM(0x2002, bus.readRAM(0x2002) & ~(1 << 7));
      break;
    case Render:
      // sprite 0 hit
      if(bus.readSpriteRAM(3) == currentDrawPixcel % CLOCKS_PER_LINE && bus.readSpriteRAM(0) + 1 == currentDrawPixcel / CLOCKS_PER_LINE) {
        // std::cout << "sprite 0 hit" << std::endl;
        bus.writeRAM(0x2002, bus.readRAM(0x2002) | (1 << 6));
      }
      // drawBgPixel(currentDrawPixcel % CLOCKS_PER_LINE, currentDrawPixcel / CLOCKS_PER_LINE);
      break;
    case PostRender:
      break;
    case VBlank:
      // vblank flag
      bus.writeRAM(0x2002, bus.readRAM(0x2002) | (1 << 7));

      // sprite 0 hit clear
      bus.writeRAM(0x2002, bus.readRAM(0x2002) | (1 << 6));

      if(currentDrawPixcel == (SCREEN_Y_WIDTH + 1) * CLOCKS_PER_LINE + 1) {
        drawAll();
        bool vblankInterruptFlag = !!(bus.readRAM(0x2000) & (1 << 7));
        if(vblankInterrupt != nullptr && vblankInterruptFlag) {
          vblankInterrupt();
        }
        
      }
      break;
    case Hblank:
      break;
    }
    setPPUState();
  }

  void PPU::setVBlankInterrupt(std::function<void(void)> func) { vblankInterrupt = func; }

} // namespace NES