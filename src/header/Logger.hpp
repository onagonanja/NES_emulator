#include "defs.hpp"

#include <string>

namespace NES {
  namespace Logger {
    void clearLog();
    void logOperation(int count, std::string opeName);

    void logRead(std::string name, Address addr, Byte data);
    void logWrite(std::string name, Address addr, Byte data);

    void logNewLine();
  }; // namespace Logger
} // namespace NES