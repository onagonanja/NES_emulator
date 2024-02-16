#include "header/Bus.hpp"
#include "header/CPU.hpp"
#include "header/Emulator.hpp"
#include "header/Logger.hpp"
#include "header/PPU.hpp"
#include "header/defs.hpp"

#include <SDL.h>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <vector>

int main(int argc, char *argv[]) {
  if(SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Quit();
    return 1;
  }

  NES::Emulator *emu = new NES::Emulator();
  emu->run();

  return 0;
}