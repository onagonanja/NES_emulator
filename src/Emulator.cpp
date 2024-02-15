#include "Emulator.hpp"

namespace NES {
  Emulator::Emulator(ScreenBuff &screenBuff) : cpu(bus), ppu(bus, screenBuff) {
    bus.readROM();
    cpu.reset();
  }

  void Emulator::run() {
    for(int i = 0; i < 100; i++) {
      cpu.run();
    }
    ppu.run();
  }
} // namespace NES