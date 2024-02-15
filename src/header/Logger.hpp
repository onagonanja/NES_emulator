#ifndef INCLUDED_LOGGER
#define INCLUDED_LOGGER

#include "defs.hpp"

#include <string>

namespace NES {
  namespace Logger {

    void clearLog();

    void clearFetchList();
    void addFetchList(int code);
    void logFetch(Byte code);

    void logPCAddress(Address addr);
    void logAddressing(std::string mode);
    void logOperation(std::string opeName);

    void logRead(std::string name, Address addr, Byte data);
    void logWrite(std::string name, Address addr, Byte data);

    void logLoadByte(Byte data);

    void logRegisters(Byte A, Byte X, Byte Y, Byte P, Address PC, Byte SP);

    void logNewLine();
  }; // namespace Logger
} // namespace NES

#endif