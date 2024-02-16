#ifndef INCLUDED_DEFS
#define INCLUDED_DEFS

#include <cstdint>

#define PIXEL_SIZE 3
#define SCREEN_X_MIN 256
#define SCREEN_Y_MIN 240
#define STACK_START 0x0100

namespace NES {
  using Byte = std::uint8_t;
  using Address = std::uint16_t;
} // namespace NES

#endif