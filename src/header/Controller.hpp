#ifndef INCLUDED_CONTROLLER
#define INCLUDED_CONTROLLER

#include "defs.hpp"

#include <SDL.h>
#include <unordered_map>

namespace NES {
  const int NUM_OF_KEYS = 8;

  class Controller {
  public:
    Controller(SDL_Event &e);
    Byte readController();
    void writeController(Byte data);

    void handleKeyDOWN(SDL_Event &event);
    void handleKeyUP(SDL_Event &event);

  private:
    std::unordered_map<SDL_KeyCode, bool> isKeyPressed;
    const SDL_KeyCode KEYS[NUM_OF_KEYS] = {SDLK_a, SDLK_b, SDLK_SELECT, SDLK_ESCAPE, SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT};
    SDL_Event &e;
    bool strobe = false;
    Byte keyStatus = 0;
  };
} // namespace NES

#endif