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

  void Emulator::handleKeyDOWN(SDL_Event &event) {
    bus.handleKeyDOWN(event);
  }

  void Emulator::handleKeyUP(SDL_Event &event) {
    bus.handleKeyUP(event);
  }

  void Emulator::run() {
    int cycle = cpu.run();
    for(int i = 0; i < 3 * cycle; i++) {
      ppu.run();
    }
  }
} // namespace NES