#include "header/CPU.hpp"
#include "header/Bus.hpp"
#include "header/Logger.hpp"
#include "header/defs.hpp"
#include "header/operationlist.hpp"
#include "magic_enum.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <time.h>

namespace NES {

  CPU::CPU(Bus &b) : bus(b) {}

  // options for debug
  void CPU::test_option() {
    // 常にVBRANK状態にする。
    bus.writeRAM(0x2002, 0b10000000 | bus.readRAM(0x2002));
  }

  void CPU::setStatusRegByByte(Byte data) {
    r_status["carry"] = data & 1;
    r_status["zero"] = (data >> 1) & 1;
    r_status["interrupt"] = (data >> 2) & 1;
    r_status["decimal"] = (data >> 3) & 1;
    r_status["break"] = (data >> 4) & 1;
    r_status["overflow"] = (data >> 6) & 1;
    r_status["negative"] = (data >> 7) & 1;
  }

  // reset
  void CPU::reset() {
    test_option();
    Address pc_lowByte = bus.readRAM(0xFFFC);
    Address pc_highByte = bus.readRAM(0xFFFD);

    r_PC = (pc_highByte << 8) | pc_lowByte;
    r_SP = 0xFD;

    r_status["interrupt"] = true;
    r_status["reserved"] = 1;
    push_status_registers();
  }

  // Non-Markable-Interrupt
  void CPU::NMI() {
    push_PC();
    push_status_registers();

    int pc_lowByte = bus.readRAM(0xFFFA);
    int pc_highByte = bus.readRAM(0xFFFB);
    r_PC = (pc_highByte << 8) | pc_lowByte;

    r_status["interrupt"] = true;
    r_status["break"] = false;
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
    return res;
  }

  void CPU::push_PC() {
    push_stack((r_PC >> 8) & 0xff);
    push_stack(r_PC & 0xff);
  }

  // ステータスレジスタをスタックに退避
  void CPU::push_status_registers() {
    Byte data = r_status["negative"] << 7 | r_status["overflow"] << 6 | r_status["reserved"] << 5 | r_status["break"] << 4 |
                r_status["decimal"] << 3 | r_status["interrupt"] << 2 | r_status["zero"] << 1 | r_status["carry"];
    push_stack(data);
  }

  // スタックからステータスレジスタを復帰
  void CPU::pop_status_registers() {
    Byte data = pop_stack();
    setStatusRegByByte(data);
  }

  // 実行
  int CPU::run() {
    Byte code = fetch();
    std::pair<Operation, AddressingMode> ope = opelist[code];
    Operation opeName = ope.first;
    AddressingMode addressing = ope.second;
    int data = fetchOperand(addressing);
    exec(opeName, static_cast<Address>(data), addressing, data == -1);
    return cycles[code];
  }

  // レジスタをセット
  void CPU::setRegisters(Byte num) {
    r_status["negative"] = !!(num & (1 << 7));
    r_status["zero"] = (num == 0);
  }

  // アドレッシングモードを判別し、データを取得
  int CPU::fetchOperand(AddressingMode mode) {
    switch(mode) {
    case ACCUMULATOR:
      return -1;
    case IMPLIED:
      return -1;
    case IMMEDIATE: {
      return fetch();
    } break;
    case ZERO_PAGE: {
      return fetch();
    } break;
    case ZERO_PAGE_X: {
      Byte add = fetch();
      return (add + r_X) & 0xff;
    } break;
    case ZERO_PAGE_Y: {
      Byte add = fetch();
      return (add + r_Y) & 0xff;
    } break;
    case ABSOLUTE: {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2;
    } break;
    case ABSOLUTE_X: {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2 + r_X;
    } break;
    case ABSOLUTE_Y: {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2 + r_Y;
    } break;
    case PREINDEXEDINDIRECT: {
      Byte addradd = fetch() + r_X;
      Address addr1 = bus.readRAM(addradd);
      Address addr2 = static_cast<Address>(bus.readRAM(static_cast<Byte>(addradd + 1))) << 8;
      return addr1 + addr2;
    } break;
    case POSTINDEXEDINDIRECT: {
      Address addradd = fetch();
      Address addr1 = bus.readRAM(addradd);
      Address addr2 = static_cast<Address>(bus.readRAM(static_cast<Byte>(addradd + 1))) << 8;
      Address offset = r_Y;
      return addr1 + addr2 + offset;
    } break;
    case INDIRECT_ABSOLUTE: {
      Address addradd_low = fetch();
      Address addradd_high = fetch();
      Address addr1 = bus.readRAM(addradd_low + (addradd_high << 8));
      Address addr2 = bus.readRAM(((addradd_low + 1) & 0xff) + (addradd_high << 8));
      return addr1 + (addr2 << 8);
    } break;
    case RELATIVE: {
      Address addr = static_cast<Address>(fetch());
      // 8bitのaddrを16bitの２の補数表現に拡張
      if((addr & (1 << 7))) {
        addr = 0b1111111100000000 | addr;
      }
      return addr + r_PC;
    } break;
    default:
      std::cout << "unknown addressing mode" << std::endl;
      return -1;
    }
  }
  // 命令コード、データから命令を実行
  void CPU::exec(Operation opeName, Address data, AddressingMode mode, bool nonOperand) {
    switch(opeName) {
    case LDA: {
      if(mode == IMMEDIATE) {
        r_A = data;
      } else {
        r_A = bus.readRAM(data);
      }
      setRegisters(r_A);
      // Logger::logLoadByte(r_A);
    } break;
    case LDX: {
      if(mode == IMMEDIATE) {
        r_X = data;
      } else {
        r_X = bus.readRAM(data);
      }
      setRegisters(r_X);
      // Logger::logLoadByte(r_X);
    } break;
    case LDY: {
      if(mode == IMMEDIATE) {
        r_Y = data;
      } else {
        r_Y = bus.readRAM(data);
      }
      setRegisters(r_Y);
      // Logger::logLoadByte(r_Y);
    } break;
    case STA: {
      bus.writeRAM(data, r_A);
    } break;
    case STX: {
      bus.writeRAM(data, r_X);
    } break;
    case STY: {
      bus.writeRAM(data, r_Y);
    } break;
    case TAX: {
      r_X = r_A;
      setRegisters(r_X);
    } break;
    case TAY: {
      r_Y = r_A;
      setRegisters(r_Y);
    } break;
    case TSX: {
      r_X = r_SP;
      setRegisters(r_X);
    } break;
    case TXA: {
      r_A = r_X;
      setRegisters(r_X);
    } break;
    case TXS: {
      r_SP = r_X;
    } break;
    case TYA: {
      r_A = r_Y;
      setRegisters(r_A);
    } break;
    case ADC: {
      int operand = (mode == IMMEDIATE) ? data : bus.readRAM(data);
      int calc = operand + r_A + r_status["carry"];
      r_status["carry"] = !!(calc & 0x100);
      r_status["overflow"] = (r_A ^ calc) & (operand ^ calc) & 0x80;
      setRegisters(calc & 0xff);
      r_A = calc & 0xff;
    } break;
    case AND: {
      int calc;
      if(mode == IMMEDIATE) {
        calc = data & r_A;
      } else {
        calc = bus.readRAM(data) & r_A;
      }
      setRegisters(calc);
      r_A = calc;
    } break;
    case ASL: {
      if(nonOperand) {
        r_status["carry"] = !!(r_A & (1 << 7));
        r_A = (r_A << 1) & 0xff; // Aレジスタを左シフト(8bit以上は切り捨て)
                                 // 7bit目をcarryフラグに代入
        setRegisters(r_A);
      } else {
        r_status["carry"] = !!(bus.readRAM(data) & (1 << 7));
        bus.writeRAM(data,
                     (bus.readRAM(data) << 1) & 0xff); // 指定されたアドレスのbitを左シフト(8bit以上は切り捨て)
        setRegisters(bus.readRAM(data));
      }
    } break;
    case BIT: {
      r_status["zero"] = !(r_A & bus.readRAM(data)); // Aレジスタとメモリデータのand演算の結果をZレジスタに格納
      r_status["overflow"] = !!(bus.readRAM(data) & (1 << 6)); // メモリデータの6bit目をVレジスタに格納
      r_status["negative"] = !!(bus.readRAM(data) & (1 << 7)); // メモリデータの7bit目をNレジスタに格納
    } break;
    case CMP: {
      data = (mode == IMMEDIATE) ? data : bus.readRAM(data);
      Address diff = r_A - data;
      r_status["carry"] = !(diff & 0x100);
      setRegisters(diff & 0xff);
    } break;
    case CPX: {
      data = (mode == IMMEDIATE) ? data : bus.readRAM(data);
      int diff = static_cast<int>(r_X) - static_cast<int>(data);
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } break;
    case CPY: {
      data = (mode == IMMEDIATE) ? data : bus.readRAM(data);
      int diff = static_cast<int>(r_Y) - static_cast<int>(data);
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } break;
    case DEC: {
      bus.writeRAM(data, (bus.readRAM(data) + 0xff) & 0xff);
      setRegisters(bus.readRAM(data));
    } break;
    case DEX: {
      r_X = (r_X + 0xff) & 0xff;
      setRegisters(r_X);
    } break;
    case DEY: {
      r_Y = (r_Y + 0xff) & 0xff;
      setRegisters(r_Y);
    } break;
    case EOR: {
      data = (mode == IMMEDIATE ? data : bus.readRAM(data));
      r_A = r_A ^ data;
      setRegisters(r_A);
    } break;
    case INC: {
      bus.writeRAM(data, (bus.readRAM(data) + 1) & 0xff);
      setRegisters(bus.readRAM(data));
    } break;
    case INX: {
      r_X = (r_X + 1) & 0xff;
      setRegisters(r_X);
    } break;
    case INY: {
      r_Y = (r_Y + 1) & 0xff;
      setRegisters(r_Y);
    } break;
    case LSR: {
      if(nonOperand) {
        r_status["carry"] = (r_A & 0b1);
        r_A = (r_A >> 1) & 0xff;
        setRegisters(r_A);
      } else {
        r_status["carry"] = (bus.readRAM(data) & 0b1);
        bus.writeRAM(data, (bus.readRAM(data) >> 1) & 0xff);
        setRegisters(bus.readRAM(data));
      }
    } break;
    case ORA: {
      data = (mode == IMMEDIATE) ? data : bus.readRAM(data);
      r_A = r_A | data;
      setRegisters(r_A);
    } break;
    case ROL: {
      if(nonOperand) {
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
    } break;
    case ROR: {
      if(nonOperand) {
        int tmp = r_A;
        r_A = ((r_A >> 1) | (r_status["carry"] << 7)) & 0xff;
        r_status["carry"] = !!(tmp & 0b1);
        setRegisters(r_A);
      } else {
        int tmp = bus.readRAM(data);
        bus.writeRAM(data, (bus.readRAM(data) >> 1 | (r_status["carry"] << 7)) & 0xff);
        r_status["carry"] = !!(tmp & 0b1);
        setRegisters(bus.readRAM(data));
      }
    } break;
    case SBC: {
      data = (mode == IMMEDIATE) ? data : bus.readRAM(data);
      Address result = r_A - data - !r_status["carry"];
      r_status["carry"] = !(result & 0x100);
      r_status["overflow"] = (r_A ^ result) & (~data ^ result) & 0x80;
      r_A = result & 0xff;
      setRegisters(r_A);
      r_A = r_A & 0xff;
      // Logger::logLoadByte(data);
      // Logger::logLoadByte(r_A);
    } break;
    case PHA: {
      push_stack(r_A);
    } break;
    case PHP: {
      push_status_registers();
    } break;
    case PLA: {
      r_A = pop_stack();
      setRegisters(r_A);
    } break;
    case PLP: {
      pop_status_registers();
    } break;
    case JMP: {
      r_PC = data;
    } break;
    case JSR: {
      int lowByte = (r_PC - 1) & 0xff;
      int highByte = ((r_PC - 1) & 0xff00) >> 8;
      push_stack(highByte);
      push_stack(lowByte);
      r_PC = data;
    } break;
    case RTS: {
      int lowByte = pop_stack();
      int highByte = pop_stack() << 8;
      r_PC = highByte + lowByte + 1;
    } break;
    case RTI: {
      // 割り込みから復帰
      pop_status_registers();
      int pc_lowByte = pop_stack();
      int pc_highByte = pop_stack();
      r_PC = (pc_highByte << 8) | pc_lowByte;
    } break;
    case BCC: {
      if(!r_status["carry"]) {
        r_PC = data;
      }
    } break;
    case BCS: {
      if(r_status["carry"]) {
        r_PC = data;
      }
    } break;
    case BEQ: {
      if(r_status["zero"]) {
        r_PC = data;
      }
    } break;
    case BMI: {
      if(r_status["negative"]) {
        r_PC = data;
      }
    } break;
    case BNE: {
      if(!r_status["zero"]) {
        r_PC = data;
      }
    } break;
    case BPL: {
      if(!r_status["negative"]) {
        r_PC = data;
      }
    } break;
    case BVC: {
      if(!r_status["overflow"]) {
        r_PC = data;
      }
    } break;
    case BVS: {
      if(r_status["overflow"]) {
        r_PC = data;
      }
    } break;
    case CLC: {
      r_status["carry"] = false;
    } break;
    case CLD: {
      r_status["decimal"] = false;
    } break;
    case CLI: {
      r_status["interrupt"] = false;
    } break;
    case CLV: {
      r_status["overflow"] = false;
    } break;
    case SEC: {
      r_status["carry"] = true;
    } break;
    case SED: {
      r_status["decimal"] = true;
    } break;
    case SEI: {
      r_status["interrupt"] = true;
    } break;
    case NOPD: {
      r_PC++;
    } break;
    default:
      std::cout << "unknown operation: " << opeName << std::endl;
    }
  }
} // namespace NES