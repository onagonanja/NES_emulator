#ifndef INCLUDED_PPU
#define INCLUDED_PPU

#include "bus.hpp"
#include "defs.hpp"

#include <vector>

class CPU;

namespace NES {
  using ScreenBuff = std::vector<std::vector<std::vector<int>>>;

  const int SCREEN_X_WIDTH = 256;
  const int SCREEN_Y_WIDTH = 240;

  const int CLOCKS_PER_LINE = 341;
  const int SCANLINES_PER_FRAME = 262;

  const Address NAMETABLESTART[4] = {0x2000, 0x2400, 0x2800, 0x2C00};

  enum PPUState { PreRender, Render, PostRender, VBlank, Hblank };

  class PPU {
  private:
    Bus &bus;
    ScreenBuff &screenBuff;
    PPUState state = PreRender;

    int currentDrawPixcel = 0;

  public:
    PPU(Bus &b, ScreenBuff &screenBuff);

    void run();

    void setPPUState();

    void drawBgPixel(int x, int y);

    void setPixelColor(int x, int y, const int *color);

    void drawAll();

    std::vector<Byte> getBackGroundPallet(Byte data);

    std::vector<Byte> getSpriteColor(Byte data);

    int getSpriteNum(int x, int y);

    int getPalletNum(int x, int y);
  };
} // namespace NES

#endif