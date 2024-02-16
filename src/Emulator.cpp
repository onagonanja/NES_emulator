#include "Emulator.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace NES {
  Emulator::Emulator(ScreenBuff &screenBuff) : cpu(bus), ppu(bus, screenBuff) {
    bus.readROM();
    cpu.reset();
  }

  void Emulator::handleKeyDOWN(SDL_Event &event) { bus.handleKeyDOWN(event); }

  void Emulator::handleKeyUP(SDL_Event &event) { bus.handleKeyUP(event); }

  void Emulator::run() {
    int cycle = cpu.run();
    for(int i = 0; i < 3 * cycle; i++) {
      ppu.run();
    }
  }

  // for nestest.nes
  void Emulator::getTestCode() {
    int code1 = bus.readRAM(0x0002);
    int code2 = bus.readRAM(0x0003);
    std::cout << "0x0002: " << std::uppercase << std::hex << code1 << std::endl;
    std::cout << "0x0003: " << std::uppercase << std::hex << code2 << std::endl;
  }
} // namespace NES