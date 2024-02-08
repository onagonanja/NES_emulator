#include "header/Bus.hpp"
#include "header/CPU.hpp"
#include "header/defs.hpp"
#include "header/operationlist.hpp"
#include "header/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <time.h>

namespace NES {

  CPU::CPU(Bus &b) : bus(b) {}

  void hex(int n) {
    // cout << hex << setw(4) << setfill('0') << n << endl;
  }

  // options for debug
  void CPU::test_option() {
    // 常にVBRANK状態にする。
    bus.writeRAM(0x2002, 0b10000000 | bus.readRAM(0x2002));
  }

  // reset
  void CPU::reset() {
    test_option();
    Address pc_lowByte = bus.readRAM(0xFFFC);
    Address pc_highByte = bus.readRAM(0xFFFD);
    r_PC = (pc_highByte << 8) | pc_lowByte;
    push_status_registers();
    r_status["interrupt"] = true;
    // cout << r_PC << endl;
  }

  // Non-Markable-Interrupt
  void CPU::NMI() {
    int pc_lowByte = bus.readRAM(0xFFFA);
    int pc_highByte = bus.readRAM(0xFFFB);
    r_PC = (pc_highByte << 8) | pc_lowByte;
    push_status_registers();
    r_status["interrupt"] = true;
    // cout << r_PC << endl;
  }

  // PCレジスタをインクリメントし、そのアドレスのデータを返す
  Byte CPU::fetch() {
    Byte res = bus.readRAM(r_PC++);
    return res;
  }

  // スタックに値をプッシュ
  void CPU::push_stack(Byte data) {
    bus.writeRAM(STACK_START + r_SP, data);
    r_SP--;
  }

  // スタックからpop
  Byte CPU::pop_stack() {
    Byte res = bus.readRAM(STACK_START + r_SP + 1);
    r_SP++;
    // cout << "pop ";
    hex(res);
    return res;
  }

  // ステータスレジスタをスタックに退避
  void CPU::push_status_registers() {
    Byte data = r_status["negative"] << 7 | r_status["overflow"] << 6 |
                r_status["reserved"] << 5 | r_status["break"] << 4 |
                r_status["decimal"] << 3 | r_status["interrupt"] << 2 |
                r_status["zero"] << 1 | r_status["carry"];
    push_stack(data);
  }

  // スタックからステータスレジスタを復帰
  void CPU::pop_status_registers() {
    Byte data = pop_stack();
    r_status["carry"] = data;
    r_status["zero"] = data >> 1 & 1;
    r_status["interrupt"] = data >> 2 & 1;
    r_status["decimal"] = data >> 3 & 1;
    r_status["break"] = data >> 4 & 1;
    r_status["reserved"] = data >> 5 & 1;
    r_status["overflow"] = data >> 6 & 1;
    r_status["negative"] = data >> 7 & 1;
  }

  // 実行
  void CPU::run() {
    Byte code = fetch();
    std::map<std::string, std::string> ope = opelist[code];
    std::string opeName = ope["baseName"];
    std::string addressing = ope["mode"];

    Logger::logOperation(count, opeName);
    count++;

    ope_appeared.insert(opeName);
    Address data = fetchOperand(addressing);
    exec(opeName, data, addressing);
    
    Logger::logNewLine();
  }

  void CPU::print_appeared_opelist() {
    // cout << "---------------------Appeared
    // Opelations----------------------------" << endl;
    for(auto itr = ope_appeared.begin(); itr != ope_appeared.end(); ++itr) {
      // cout << *itr << endl;
    }
    // cout <<
    // "--------------------------------------------------------------------" <<
    // endl;
  };

  // レジスタをセット
  void CPU::setRegisters(Byte num) {
    r_status["negative"] = !!(num & (1 << 7));
    r_status["zero"] = (num == 0);
  }

  // アドレッシングモードを判別し、データを取得
  Address CPU::fetchOperand(std::string addr) {
    if(addr == "accumulator") {
      return -1;
    } else if(addr == "implied") {
      return -1;
    } else if(addr == "immediate") {
      return fetch();
    } else if(addr == "zeroPage") {
      return fetch();
    } else if(addr == "zeroPageX") {
      Address add = fetch();
      return add + r_X;
    } else if(addr == "zeroPageY") {
      Address add = fetch();
      return add + r_Y;
    } else if(addr == "absolute") {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2;
    } else if(addr == "absoluteX") {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2 + r_X;
    } else if(addr == "absoluteY") {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2 + r_Y;
    } else if(addr == "preIndexedIndirect") {
      Address addradd = fetch() + r_X;
      Address addr1 = bus.readRAM(addradd);
      Address addr2 = static_cast<Address>(bus.readRAM(addradd + 1) << 8);
      return addr1 + addr2;
    } else if(addr == "postIndexedIndirect") {
      Address addradd = fetch();
      Address addr1 = bus.readRAM(addradd);
      Address addr2 = static_cast<Address>(bus.readRAM(addradd + 1)) << 8;
      Address offset = r_Y;
      return addr1 + addr2 + offset;
    } else if(addr == "indirectAbsolute") {
      Address addradd_low = fetch();
      Address addradd_high = fetch();
      Address addr1 = bus.readRAM(addradd_low + (addradd_high << 8));
      Address addr2 =
          bus.readRAM((addradd_low + 1) & 0xff + (addradd_high) << 8);
      return addr1 + addr2;
    } else if(addr == "relative") {
      Address addr = static_cast<Address>(fetch());
      // 8bitのaddrを16bitの２の補数表現に拡張
      if((addr & (1 << 7))) {
        addr = 0b1111111100000000 | addr;
      }
      return addr + r_PC;
    }
    return -1;
  }

  // 命令コード、データから命令を実行
  void CPU::exec(std::string opeName, Address data, std::string mode) {
    if(opeName == "LDA") { // データをレジスタにロード
      if(mode == "immediate") {
        r_A = data;
      } else {
        r_A = bus.readRAM(data);
      }
      setRegisters(r_A);
    } else if(opeName == "LDX") {
      if(mode == "immediate") {
        r_X = data;
      } else {
        r_X = bus.readRAM(data);
      }
      setRegisters(r_X);
    } else if(opeName == "LDY") {
      if(mode == "immediate") {
        r_Y = data;
      } else {
        r_Y = bus.readRAM(data);
      }
      setRegisters(r_Y);
    } else if(opeName == "STA") { // レジスタのデータをメモリにストア
      bus.writeRAM(data, r_A);
    } else if(opeName == "STX") {
      bus.writeRAM(data, r_X);
    } else if(opeName == "STY") {
      bus.writeRAM(data, r_Y);
    } else if(opeName == "TAX") { // レジスタの値を別レジスタにコピー
      r_X = r_A;
      setRegisters(r_X);
    } else if(opeName == "TAY") {
      r_Y = r_A;
      setRegisters(r_Y);
    } else if(opeName == "TSX") {
      r_X = r_SP;
      setRegisters(r_X);
    } else if(opeName == "TXA") {
      r_A = r_X;
      setRegisters(r_X);
    } else if(opeName == "TXS") {
      r_SP = r_X;
      setRegisters(r_SP);
    } else if(opeName == "TYA") {
      r_A = r_Y;
      setRegisters(r_A);
    } else if(opeName == "ADC") { // 演算命令いろいろ
      int calc;
      if(mode == "immediate") {
        calc = data + r_A + r_status["carry"];
      } else {
        calc = bus.readRAM(data) + r_A + r_status["carry"];
      }
      r_status["carry"] = !!(calc > 0xff);
      setRegisters(calc);
      r_A = calc & 0xff;
    } else if(opeName == "AND") {
      int calc;
      if(mode == "immediate") {
        calc = data & r_A;
      } else {
        calc = bus.readRAM(data) & r_A;
      }
      setRegisters(calc);
      r_A = calc & 0xff;
    } else if(opeName == "ASL") {
      if(data == -1) {
        r_status["carry"] = !!(r_A & (1 << 7));
        r_A = (r_A << 1) & 0xff; // Aレジスタを左シフト(8bit以上は切り捨て)
                                 // 7bit目をcarryフラグに代入
        setRegisters(r_A);
      } else {
        r_status["carry"] = !!(bus.readRAM(data) & (1 << 7));
        bus.writeRAM(
            data,
            (bus.readRAM(data) << 1) &
                0xff); // 指定されたアドレスのbitを左シフト(8bit以上は切り捨て)
        setRegisters(bus.readRAM(data));
      }
    } else if(opeName == "BIT") {
      r_status["zero"] = !!(
          r_A &
          bus.readRAM(
              data)); // Aレジスタとメモリデータのand演算の結果をZレジスタに格納
      r_status["overflow"] =
          !!(bus.readRAM(data) &
             (1 << 6)); // メモリデータの6bit目をVレジスタに格納
      r_status["negative"] =
          !!(bus.readRAM(data) &
             (1 << 7)); // メモリデータの7bit目をNレジスタに格納
    } else if(opeName == "CMP") {
      data = (mode == "immediate") ? data : bus.readRAM(data);
      int diff = r_A - data;
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } else if(opeName == "CPX") {
      data = (mode == "immediate") ? data : bus.readRAM(data);
      int diff = r_X - data;
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } else if(opeName == "CPY") {
      data = (mode == "immediate") ? data : bus.readRAM(data);
      int diff = r_Y - data;
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } else if(opeName == "DEC") {
      bus.writeRAM(data, (bus.readRAM(data) + 0xff) & 0xff);
      setRegisters(bus.readRAM(data));
    } else if(opeName == "DEX") {
      r_X = (r_X + 0xff) & 0xff;
      setRegisters(r_X);
      hex(r_X);
    } else if(opeName == "DEY") {
      r_Y = (r_Y + 0xff) & 0xff;
      setRegisters(r_Y);
      hex(r_Y);
    } else if(opeName == "EOR") {
      data = (mode == "immediate" ? data : bus.readRAM(data));
      r_A = r_A ^ data;
      setRegisters(r_A);
    } else if(opeName == "INC") {
      bus.writeRAM(data, (bus.readRAM(data) + 1) & 0xff);
      setRegisters(bus.readRAM(data));
    } else if(opeName == "INX") {
      r_X = (r_X + 1) & 0xff;
      setRegisters(r_X);
    } else if(opeName == "INY") {
      r_Y = (r_Y + 1) & 0xff;
      hex(r_Y);
      setRegisters(r_Y);
    } else if(opeName == "LSR") {
      if(data == -1) {
        r_status["carry"] = (r_A & 0b1);
        r_A = (r_A >> 1) & 0xff;
        setRegisters(r_A);
      } else {
        r_status["carry"] = (bus.readRAM(data) & 0b1);
        bus.writeRAM(data, (bus.readRAM(data) >> 1) & 0xff);
        setRegisters(bus.readRAM(data));
      }
    } else if(opeName == "ORA") {
      data = (mode == "immediate") ? data : bus.readRAM(data);
      r_A = r_A | data;
      setRegisters(r_A);
    } else if(opeName == "ROL") {
      if(data == -1) {
        int tmp = r_A;
        r_A = (r_A << 1) | r_status["carry"];
        r_status["carry"] = !!(tmp & 1 << 7);
        setRegisters(r_A);
        r_A = r_A & 0xff;
      } else {
        int tmp = bus.readRAM(data);
        bus.writeRAM(data, bus.readRAM(data) << 1 | r_status["carry"]);
        r_status["carry"] = !!(tmp & 1 << 7);
        setRegisters(bus.readRAM(data));
        bus.writeRAM(data, bus.readRAM(data) & 0xff);
      }
    } else if(opeName == "ROR") {
      if(data == -1) {
        int tmp = r_A;
        r_A = ((r_A >> 1) | (r_status["carry"] << 7)) & 0xff;
        r_status["carry"] = !!(tmp & 0b1);
        setRegisters(r_A);
      } else {
        int tmp = bus.readRAM(data);
        bus.writeRAM(data, (bus.readRAM(data) >> 1 | (r_status["carry"] << 7)) &
                               0xff);
        r_status["carry"] = !!(tmp & 0b1);
        setRegisters(bus.readRAM(data));
      }
    } else if(opeName == "SBC") {
      data = (mode == "immediate") ? data : bus.readRAM(data);
      r_A = r_A + (~data + 1) + (0x100 - !r_status["carry"]);
      r_status["carry"] = !!(r_A > 0xff);
      setRegisters(r_A);
      r_A = r_A & 0xff;
    } else if(opeName == "PHA") {
      push_stack(r_A);
    } else if(opeName == "PHP") {
      push_status_registers();
    } else if(opeName == "PLA") {
      r_A = pop_stack();
    } else if(opeName == "PLP") {
      pop_status_registers();
    } else if(opeName == "JMP") {
      r_PC = data;
    } else if(opeName == "JSR") {
      int lowByte = (r_PC - 1) & 0xff;
      int highByte = ((r_PC - 1) & 0xff00) >> 8;
      push_stack(highByte);
      push_stack(lowByte);
      r_PC = data;
    } else if(opeName == "RTS") {
      int lowByte = pop_stack();
      int highByte = pop_stack() << 8;
      r_PC = highByte + lowByte + 1;
    } else if(opeName == "RTI") {
      // 割り込みから復帰
      pop_status_registers();
      int pc_lowByte = pop_stack();
      int pc_highByte = pop_stack();
      r_PC = (pc_highByte << 8) | pc_lowByte;
    } else if(opeName == "BCC") {
      if(!r_status["carry"]) {
        r_PC = data;
      }
    } else if(opeName == "BCS") {
      if(r_status["carry"]) {
        r_PC = data;
      }
    } else if(opeName == "BEQ") {
      if(r_status["zero"]) {
        r_PC = data;
      }
    } else if(opeName == "BMI") {
      if(r_status["negative"]) {
        r_PC = data;
      }
    } else if(opeName == "BNE") {
      if(!r_status["zero"]) {
        r_PC = data;
      }
    } else if(opeName == "BPL") {
      if(!r_status["negative"]) {
        r_PC = data;
      }
    } else if(opeName == "BVC") {
      if(!r_status["overflow"]) {
        r_PC = data;
      }
    } else if(opeName == "BVS") {
      if(r_status["overflow"]) {
        r_PC = data;
      }
    } else if(opeName == "CLC") {
      r_status["carry"] = false;
    } else if(opeName == "CLD") {
      r_status["decimal"] = false;
    } else if(opeName == "CLI") {
      r_status["interrupt"] = false;
    } else if(opeName == "CLV") {
      r_status["overflow"] = false;
    } else if(opeName == "SEC") {
      r_status["carry"] = true;
    } else if(opeName == "SED") {
      r_status["decimal"] = true;
    } else if(opeName == "SEI") {
      r_status["interrupt"] = true;
    }
    // 非公式
    else if(opeName == "NOPD") {
      r_PC++;
    }
  }
} // namespace NES