#ifndef INCLUDED_CPU
#define INCLUDED_CPU

#include <map>
#include <set>
#include <string>
#include <vector>

#include "defs.hpp"
#include "Bus.hpp"

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
        {"negative", 0}, {"overflow", 0},  {"reserved", 0}, {"break", 0},
        {"decimal", 0},  {"interrupt", 0}, {"zero", 0},     {"carry", 0},
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

    // 値に応じてレジスタを変化させる
    void setRegisters(Byte num);

    // スタックに値をpush
    void push_stack(Byte data);

    // スタックから値をpop
    Byte pop_stack();

    // ステータスレジスタをスタックに退避
    void push_status_registers();

    // スタックからステータスレジスタを復帰
    void pop_status_registers();

    // PCレジスタをインクリメントしつつその内容を返す
    Byte fetch();

    // アドレッシングモードを判別
    Address fetchOperand(std::string addr);

    // 命令コード、オペランドから命令を実行
    void exec(std::string opeName, Address opeland, std::string mode);

    // 実行,
    void run();

    //-----------------------Debug-------------------------//

    std::set<std::string> ope_appeared;

    void print_appeared_opelist();

    int count = 0;
  };
} // namespace NES

#endif