#include "header/Logger.hpp"
#include "header/defs.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

namespace NES {
  namespace Logger {
    void clearLog() {
      ofstream outputfile("logs/log.txt", ios::trunc);
      outputfile.close();
    }

    void logOperation(int count, string opeName) {
      ofstream outputfile("logs/log.txt", ios::app);
      outputfile << hex << setw(4) << setfill('0') << count << " ";
      outputfile << opeName << " ";
      outputfile.close();
    }

    void logRead(std::string name, Address addr, Byte data) {
      ofstream outputfile("logs/log.txt", ios::app);
      outputfile << "read" + name + "(" << hex << setw(4) << setfill('0')
                 << addr << ")=" << hex << setw(2) << setfill('0') << (int)data
                 << " ";
      outputfile.close();
    }

    void logWrite(std::string name, Address addr, Byte data) {
      ofstream outputfile("logs/log.txt", ios::app);
      outputfile << "write" + name + "(" << hex << setw(4) << setfill('0')
                 << addr << "," << hex << setw(2) << setfill('0') << (int)data
                 << ") ";
      outputfile.close();
    }

    void logNewLine() {
      ofstream outputfile("logs/log.txt", ios::app);
      outputfile << endl;
      outputfile.close();
    }
  } // namespace Logger
} // namespace NES