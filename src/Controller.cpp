#include "header/Controller.hpp"
#include "header/defs.hpp"

#include <bitset>
#include <iostream>

namespace NES {
  Controller::Controller(SDL_Event &e) : e(e) {}

  void Controller::handleKeyDOWN(SDL_Event &e) { isKeyPressed[static_cast<SDL_KeyCode>(e.key.keysym.sym)] = true; }

  void Controller::handleKeyUP(SDL_Event &e) { isKeyPressed[static_cast<SDL_KeyCode>(e.key.keysym.sym)] = false; }

  Byte Controller::readController() {
    // std::cout << "readController" << std::endl;

    Byte ret = 0;
    if(strobe) {
      ret = isKeyPressed[KEYS[0]];
    } else {
      ret = keyStatus & 0x01;
      keyStatus >>= 1;
    }

    return ret | 0x40;
  }

  void Controller::writeController(Byte data) {
    // strobeがclearされたときにkeyStatusを更新する
    if(strobe && !(data & 0x01)) {
      for(int i = 0; i < NUM_OF_KEYS; i++) {
        keyStatus |= isKeyPressed[KEYS[i]] << i;
      }
    }

    strobe = (data & 0x01);
  }
} // namespace NES