#include "header/Logger.hpp"
#include "header/defs.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

namespace NES {
  namespace Logger {
    const bool isLog = false;

    bool f = false;

    int fetchList[3] = {-1, -1, -1};

    int lineNum = 0;

    int LOG_REGS_FROM = 50;

    void clearLog() {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::trunc);
      outputfile.close();
    }

    void clearFetchList() {
      for(int i = 0; i < 3; i++) {
        fetchList[i] = -1;
      }
    }

    void addFetchList(int code) {
      for(int i = 0; i < 3; i++) {
        if(fetchList[i] == -1) {
          fetchList[i] = code;
          break;
        }
      }
    }

    void logFetch(Byte code) {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << uppercase << hex << setw(2) << setfill('0') << (int)code << " ";
      outputfile.close();
      f = true;
    }

    void logPCAddress(Address addr) {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << uppercase << hex << setw(4) << setfill('0') << addr << "  ";
      outputfile.close();
    }

    void logAddressing(string mode) {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << mode << " ";
      outputfile.close();
    }

    void logOperation(string opeName) {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);

      for(int i = 0; i < 3; i++) {
        if(fetchList[i] != -1) {
          outputfile << uppercase << hex << setw(2) << setfill('0') << (int)fetchList[i] << " ";
        } else {
          outputfile << "   ";
        }
      }

      outputfile << " " << opeName << " ";

      outputfile.close();
      f = true;
    }

    void logRead(std::string name, Address addr, Byte data) {
      if(!isLog || !f)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << "read" + name + "(" << hex << setw(4) << setfill('0') << addr << ")=" << hex << setw(2) << setfill('0') << (int)data
                 << " ";
      outputfile.close();
    }

    void logWrite(std::string name, Address addr, Byte data) {
      if(!isLog || !f)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << "$" << uppercase << hex << setw(2) << setfill('0') << addr << " = " << hex << setw(2) << setfill('0') << (int)data
                 << " ";
      /*
      outputfile << "write" + name + "(" << hex << setw(4) << setfill('0')
                 << addr << "," << hex << setw(2) << setfill('0') << (int)data
                 << ") ";
      */
      outputfile.close();
    }

    void logLoadByte(Byte data) {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << "#$" << uppercase << hex << setw(2) << setfill('0') << (int)data << " ";
      outputfile.close();
    }

    void logPreIndexdIndirect(Byte addradd, Address addr1, Address addr2) {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << std::uppercase << std::hex << std::setw(4) << "addr1: " << addr1 << " addr2: " << addr2 << " addradd: " << (int)addradd
                 << " result: " << addr1 + addr2 << " ";
      outputfile.close();
    }

    void logRegisters(Byte A, Byte X, Byte Y, Byte P, Address PC, Byte SP) {
      return;
      if(!isLog)
        return;

      ifstream inputfile("logs/log.log", ios::in);

      // read final line
      string line;
      while(getline(inputfile, line)) {
        if(inputfile.eof()) {
          break;
        }
      }
      inputfile.close();

      ofstream outputfile("logs/log.log", ios::app);

      for(int i = line.length(); i < LOG_REGS_FROM; i++) {
        outputfile << " ";
      }

      outputfile << "A:" << uppercase << hex << setw(2) << setfill('0') << (int)A << " ";
      outputfile << "X:" << uppercase << hex << setw(2) << setfill('0') << (int)X << " ";
      outputfile << "Y:" << uppercase << hex << setw(2) << setfill('0') << (int)Y << " ";
      outputfile << "P:" << uppercase << hex << setw(2) << setfill('0') << (int)P << " ";
      outputfile << "SP:" << uppercase << hex << setw(2) << setfill('0') << (int)SP << " ";
      outputfile.close();
    }

    void logNewLine() {
      if(!isLog)
        return;
      ofstream outputfile("logs/log.log", ios::app);
      outputfile << endl;
      outputfile.close();
      lineNum++;
      f = false;
    }
  } // namespace Logger
} // namespace NES