#include "defs.hpp"

#include <string>

namespace NES {
  namespace Logger {
    void logOperation(int count, std::string opeName);

    void logRead(std::string name, Address addr, Byte data);
    void logWrite(std::string name, Address addr, Byte data);
  }; // namespace Logger
} // namespace NES