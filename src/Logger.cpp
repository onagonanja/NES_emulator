#include "header/Logger.hpp"
#include "header/defs.hpp"
#include <iomanip>
#include <iostream>

using namespace std;

namespace NES {
  namespace Logger {
    void logOperation(int count, string opeName) {
      cout << hex << setw(4) << setfill('0') << count << " ";
      cout << opeName << " ";
    }

    void logRead(std::string name, Address addr, Byte data) {
      cout << "read" + name + "(" << hex << setw(4) << setfill('0') << addr
           << ")=" << hex << setw(2) << setfill('0') << (int)data << " ";
    }

    void logWrite(std::string name, Address addr, Byte data) {
      cout << "write" + name + "(" << hex << setw(4) << setfill('0') << addr
           << "," << hex << setw(2) << setfill('0') << (int)data << ") ";
    }
  } // namespace Logger
} // namespace NES