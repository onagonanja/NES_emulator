#ifndef INCLUDED_DEFS
#define INCLUDED_DEFS

#include <cstdint>

#define PIXEL_SIZE 3
#define SCREEN_X_MIN 256
#define SCREEN_Y_MIN 240
#define STACK_START 0x0100

namespace NES {
  using Byte = std::uint8_t;
  using Address = std::uint16_t;

  enum Operation {
    NOP,
    BRK,
    JSR,
    RTI,
    RTS,
    JMP,
    JMPI,
    PHP,
    PLP,
    PHA,
    PLA,
    DEY,
    DEX,
    TAY,
    INY,
    INX,
    CLC,
    SEC,
    CLI,
    SEI,
    TYA,
    CLV,
    CLD,
    SED,
    TXA,
    TXS,
    TAX,
    TSX,

    ASL,
    ROL,
    LSR,
    ROR,
    STX,
    LDX,
    DEC,
    INC,

    ORA,
    AND,
    EOR,
    ADC,
    STA,
    LDA,
    CMP,
    SBC,

    BIT,
    STY,
    LDY,
    CPY,
    CPX,

    IRQ,
    NMI,
    BRK_,

    SLO,
    RLA,
    SRE,
    RRA,
    SAX,
    LAX,
    DCP,
    ISB,
    BPL,
    BMI,
    BVC,
    BVS,
    BCC,
    BCS,
    BNE,
    BEQ,

    NOPD,
    NOPI,
  };

  enum AddressingMode {
    IMPLIED,
    PREINDEXEDINDIRECT,
    ZERO_PAGE,
    ABSOLUTE,
    POSTINDEXEDINDIRECT,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    RELATIVE,
    IMMEDIATE,
    ACCUMULATOR,
    INDIRECT,
    INDIRECT_ABSOLUTE,
    ABSOLUTE_INDIRECT,
    ZERO_PAGE_INDIRECT,
  };
} // namespace NES

#endif