#ifndef INCLUDED_CPU
#define INCLUDED_CPU

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Bus.hpp"
#include "defs.hpp"

namespace NES {

  class CPU {
  private:
    // bus
    Bus &bus;

    // registers
    Byte r_A = 0x00;
    Byte r_X = 0x00;
    Byte r_Y = 0x00;
    Byte r_SP = 0xff;
    Address r_PC = 0x0000;

    // starus register
    std::map<std::string, Byte> r_status = {
        {"negative", 0}, {"overflow", 0}, {"reserved", 1}, {"break", 0}, {"decimal", 0}, {"interrupt", 0}, {"zero", 0}, {"carry", 0},
    };

  public:
    // constructor
    CPU(Bus &b);

    // set options for debugging
    void test_option();

    // reset
    void reset();

    // Non-Markable-Interrupt
    void NMI();

    void setStatusRegByByte(Byte data);

    // 値に応じてレジスタを変化させる
    void setRegisters(Byte num);

    // スタックに値をpush
    void push_stack(Byte data);

    // スタックから値をpop
    Byte pop_stack();

    // スタックにPCをpush
    void push_PC();

    // ステータスレジスタをスタックに退避
    void push_status_registers();

    // スタックからステータスレジスタを復帰
    void pop_status_registers();

    // PCレジスタをインクリメントしつつその内容を返す
    Byte fetch();

    // アドレッシングモードを判別
    int fetchOperand(AddressingMode mode);

    // 命令コード、オペランドから命令を実行
    void exec(Operation opeName, Address opeland, AddressingMode mode, bool nonOperand);

    // 実行
    int run();
  };
} // namespace NES

#endif