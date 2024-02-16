#include "Emulator.hpp"

#include <SDL.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <time.h>

namespace NES {
  const Address PAD1 = 0x4016;

  Emulator::Emulator()
      : cpu(bus), ppu(bus, screenBuff), controller(e), renderer(screenBuff),
        screenBuff(SCREEN_Y_WIDTH, std::vector<std::vector<int>>(SCREEN_X_WIDTH, std::vector<int>(3, 0))) {
    NES::Logger::clearLog();
    bus.readROM();
    bus.setReadCallback(PAD1, [&]() { return controller.readController(); });
    bus.setWriteCallback(PAD1, [&](Byte data) { controller.writeController(data); });
    cpu.reset();
    ppu.setVBlankInterrupt([&]() {
      renderer.render();
      cpu.NMI();
    });
  }

  void Emulator::handleKeyDOWN(SDL_Event &event) { controller.handleKeyDOWN(event); }

  void Emulator::handleKeyUP(SDL_Event &event) { controller.handleKeyUP(event); }

  void Emulator::run() {
    bool quit = false;
    // mainloop
    while(!quit) {
      while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
          quit = true;
        } else if(e.type == SDL_KEYDOWN) {
          handleKeyDOWN(e);
        } else if(e.type == SDL_KEYUP) {
          handleKeyUP(e);
        }
      }

      int cycle = cpu.run();
      for(int i = 0; i < 3 * cycle; i++) {
        ppu.run();
      }
    }

    renderer.close();
  }

  // for nestest.nes
  void Emulator::getTestCode() {
    int code1 = bus.readRAM(0x0002);
    int code2 = bus.readRAM(0x0003);
    std::cout << "0x0002: " << std::uppercase << std::hex << code1 << std::endl;
    std::cout << "0x0003: " << std::uppercase << std::hex << code2 << std::endl;
  }
} // namespace NES