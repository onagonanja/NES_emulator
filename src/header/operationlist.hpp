#ifndef INCLUDED_OPERATIONLIST
#define INCLUDED_OPERATIONLIST

#include <utility>
#include <string>

#include "defs.hpp"

namespace NES {
  std::vector<std::pair<Operation, AddressingMode>> opelist = {
      {
          BRK,
          IMPLIED,
      },
      {

          ORA,
          PREINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SLO,
          PREINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          ORA,
          ZERO_PAGE,
      },
      {

          ASL,
          ZERO_PAGE,
      },
      {

          SLO,
          ZERO_PAGE,
      },
      {

          PHP,
          IMPLIED,
      },
      {

          ORA,
          IMMEDIATE,
      },
      {

          ASL,
          ACCUMULATOR,
      },
      {},
      {

          NOPI,
          IMPLIED,
      },
      {

          ORA,
          ABSOLUTE,
      },
      {

          ASL,
          ABSOLUTE,
      },
      {

          SLO,
          ABSOLUTE,
      },
      {

          BPL,
          RELATIVE,
      },
      {

          ORA,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SLO,
          POSTINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          ORA,
          ZERO_PAGE_X,
      },
      {

          ASL,
          ZERO_PAGE_X,
      },
      {

          SLO,
          ZERO_PAGE_X,
      },
      {

          CLC,
          IMPLIED,
      },
      {

          ORA,
          ABSOLUTE_Y,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SLO,
          ABSOLUTE_Y,
      },
      {

          NOPI,
          IMPLIED,
      },
      {

          ORA,
          ABSOLUTE_X,
      },
      {

          ASL,
          ABSOLUTE_X,
      },
      {

          SLO,
          ABSOLUTE_X,
      },
      {

          JSR,
          ABSOLUTE,
      },
      {

          AND,
          PREINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          RLA,
          PREINDEXEDINDIRECT,
      },
      {

          BIT,
          ZERO_PAGE,
      },
      {

          AND,
          ZERO_PAGE,
      },
      {

          ROL,
          ZERO_PAGE,
      },
      {

          RLA,
          ZERO_PAGE,
      },
      {

          PLP,
          IMPLIED,
      },
      {

          AND,
          IMMEDIATE,
      },
      {

          ROL,
          ACCUMULATOR,
      },
      {},
      {

          BIT,
          ABSOLUTE,
      },
      {

          AND,
          ABSOLUTE,
      },
      {

          ROL,
          ABSOLUTE,
      },
      {

          RLA,
          ABSOLUTE,
      },
      {

          BMI,
          RELATIVE,
      },
      {

          AND,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          RLA,
          POSTINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          AND,
          ZERO_PAGE_X,
      },
      {

          ROL,
          ZERO_PAGE_X,
      },
      {

          RLA,
          ZERO_PAGE_X,
      },
      {

          SEC,
          IMPLIED,
      },
      {

          AND,
          ABSOLUTE_Y,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          RLA,
          ABSOLUTE_Y,
      },
      {

          NOPI,
          IMPLIED,
      },
      {

          AND,
          ABSOLUTE_X,
      },
      {

          ROL,
          ABSOLUTE_X,
      },
      {

          RLA,
          ABSOLUTE_X,
      },
      {

          RTI,
          IMPLIED,
      },
      {

          EOR,
          PREINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SRE,
          PREINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          EOR,
          ZERO_PAGE,
      },
      {

          LSR,
          ZERO_PAGE,
      },
      {

          SRE,
          ZERO_PAGE,
      },
      {

          PHA,
          IMPLIED,
      },
      {

          EOR,
          IMMEDIATE,
      },
      {

          LSR,
          ACCUMULATOR,
      },
      {},
      {

          JMP,
          ABSOLUTE,
      },
      {

          EOR,
          ABSOLUTE,
      },
      {

          LSR,
          ABSOLUTE,
      },
      {

          SRE,
          ABSOLUTE,
      },
      {

          BVC,
          RELATIVE,
      },
      {

          EOR,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SRE,
          POSTINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          EOR,
          ZERO_PAGE_X,
      },
      {

          LSR,
          ZERO_PAGE_X,
      },
      {

          SRE,
          ZERO_PAGE_X,
      },
      {

          CLI,
          IMPLIED,
      },
      {

          EOR,
          ABSOLUTE_Y,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SRE,
          ABSOLUTE_Y,
      },
      {

          NOPI,
          IMPLIED,
      },
      {

          EOR,
          ABSOLUTE_X,
      },
      {

          LSR,
          ABSOLUTE_X,
      },
      {

          SRE,
          ABSOLUTE_X,
      },
      {

          RTS,
          IMPLIED,
      },
      {

          ADC,
          PREINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          RRA,
          PREINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          ADC,
          ZERO_PAGE,
      },
      {

          ROR,
          ZERO_PAGE,
      },
      {

          RRA,
          ZERO_PAGE,
      },
      {

          PLA,
          IMPLIED,
      },
      {

          ADC,
          IMMEDIATE,
      },
      {

          ROR,
          ACCUMULATOR,
      },
      {},
      {

          JMP,
          INDIRECT_ABSOLUTE,
      },
      {

          ADC,
          ABSOLUTE,
      },
      {

          ROR,
          ABSOLUTE,
      },
      {

          RRA,
          ABSOLUTE,
      },
      {

          BVS,
          RELATIVE,
      },
      {

          ADC,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          RRA,
          POSTINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          ADC,
          ZERO_PAGE_X,
      },
      {

          ROR,
          ZERO_PAGE_X,
      },
      {

          RRA,
          ZERO_PAGE_X,
      },
      {

          SEI,
          IMPLIED,
      },
      {

          ADC,
          ABSOLUTE_Y,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          RRA,
          ABSOLUTE_Y,
      },
      {

          NOPI,
          IMPLIED,
      },
      {

          ADC,
          ABSOLUTE_X,
      },
      {

          ROR,
          ABSOLUTE_X,
      },
      {

          RRA,
          ABSOLUTE_X,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          STA,
          PREINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          SAX,
          PREINDEXEDINDIRECT,
      },
      {

          STY,
          ZERO_PAGE,
      },
      {

          STA,
          ZERO_PAGE,
      },
      {

          STX,
          ZERO_PAGE,
      },
      {

          SAX,
          ZERO_PAGE,
      },
      {

          DEY,
          IMPLIED,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          TXA,
          IMPLIED,
      },
      {},
      {

          STY,
          ABSOLUTE,
      },
      {

          STA,
          ABSOLUTE,
      },
      {

          STX,
          ABSOLUTE,
      },
      {

          SAX,
          ABSOLUTE,
      },
      {

          BCC,
          RELATIVE,
      },
      {

          STA,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {},
      {

          STY,
          ZERO_PAGE_X,
      },
      {

          STA,
          ZERO_PAGE_X,
      },
      {

          STX,
          ZERO_PAGE_Y,
      },
      {

          SAX,
          ZERO_PAGE_Y,
      },
      {

          TYA,
          IMPLIED,
      },
      {

          STA,
          ABSOLUTE_Y,
      },
      {

          TXS,
          IMPLIED,
      },
      {},
      {},
      {

          STA,
          ABSOLUTE_X,
      },
      {},
      {},
      {

          LDY,
          IMMEDIATE,
      },
      {

          LDA,
          PREINDEXEDINDIRECT,
      },
      {

          LDX,
          IMMEDIATE,
      },
      {

          LAX,
          PREINDEXEDINDIRECT,
      },
      {

          LDY,
          ZERO_PAGE,
      },
      {

          LDA,
          ZERO_PAGE,
      },
      {

          LDX,
          ZERO_PAGE,
      },
      {

          LAX,
          ZERO_PAGE,
      },
      {

          TAY,
          IMPLIED,
      },
      {

          LDA,
          IMMEDIATE,
      },
      {

          TAX,
          IMPLIED,
      },
      {},
      {

          LDY,
          ABSOLUTE,
      },
      {

          LDA,
          ABSOLUTE,
      },
      {

          LDX,
          ABSOLUTE,
      },
      {

          LAX,
          ABSOLUTE,
      },
      {

          BCS,
          RELATIVE,
      },
      {

          LDA,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          LAX,
          POSTINDEXEDINDIRECT,
      },
      {

          LDY,
          ZERO_PAGE_X,
      },
      {

          LDA,
          ZERO_PAGE_X,
      },
      {

          LDX,
          ZERO_PAGE_Y,
      },
      {

          LAX,
          ZERO_PAGE_Y,
      },
      {

          CLV,
          IMPLIED,
      },
      {

          LDA,
          ABSOLUTE_Y,
      },
      {

          TSX,
          IMPLIED,
      },
      {},
      {

          LDY,
          ABSOLUTE_X,
      },
      {

          LDA,
          ABSOLUTE_X,
      },
      {

          LDX,
          ABSOLUTE_Y,
      },
      {

          LAX,
          ABSOLUTE_Y,
      },
      {

          CPY,
          IMMEDIATE,
      },
      {

          CMP,
          PREINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          DCP,
          PREINDEXEDINDIRECT,
      },
      {

          CPY,
          ZERO_PAGE,
      },
      {

          CMP,
          ZERO_PAGE,
      },
      {

          DEC,
          ZERO_PAGE,
      },
      {

          DCP,
          ZERO_PAGE,
      },
      {

          INY,
          IMPLIED,
      },
      {

          CMP,
          IMMEDIATE,
      },
      {

          DEX,
          IMPLIED,
      },
      {},
      {

          CPY,
          ABSOLUTE,
      },
      {

          CMP,
          ABSOLUTE,
      },
      {

          DEC,
          ABSOLUTE,
      },
      {

          DCP,
          ABSOLUTE,
      },
      {

          BNE,
          RELATIVE,
      },
      {

          CMP,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          DCP,
          POSTINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          CMP,
          ZERO_PAGE_X,
      },
      {

          DEC,
          ZERO_PAGE_X,
      },
      {

          DCP,
          ZERO_PAGE_X,
      },
      {

          CLD,
          IMPLIED,
      },
      {

          CMP,
          ABSOLUTE_Y,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          DCP,
          ABSOLUTE_Y,
      },
      {

          NOPI,
          IMPLIED,
      },
      {

          CMP,
          ABSOLUTE_X,
      },
      {

          DEC,
          ABSOLUTE_X,
      },
      {

          DCP,
          ABSOLUTE_X,
      },
      {

          CPX,
          IMMEDIATE,
      },
      {

          SBC,
          PREINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          ISB,
          PREINDEXEDINDIRECT,
      },
      {

          CPX,
          ZERO_PAGE,
      },
      {

          SBC,
          ZERO_PAGE,
      },
      {

          INC,
          ZERO_PAGE,
      },
      {

          ISB,
          ZERO_PAGE,
      },
      {

          INX,
          IMPLIED,
      },
      {

          SBC,
          IMMEDIATE,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          SBC,
          IMMEDIATE,
      },
      {

          CPX,
          ABSOLUTE,
      },
      {

          SBC,
          ABSOLUTE,
      },
      {

          INC,
          ABSOLUTE,
      },
      {

          ISB,
          ABSOLUTE,
      },
      {

          BEQ,
          RELATIVE,
      },
      {

          SBC,
          POSTINDEXEDINDIRECT,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          ISB,
          POSTINDEXEDINDIRECT,
      },
      {

          NOPD,
          IMPLIED,
      },
      {

          SBC,
          ZERO_PAGE_X,
      },
      {

          INC,
          ZERO_PAGE_X,
      },
      {

          ISB,
          ZERO_PAGE_X,
      },
      {

          SED,
          IMPLIED,
      },
      {

          SBC,
          ABSOLUTE_Y,
      },
      {

          NOP,
          IMPLIED,
      },
      {

          ISB,
          ABSOLUTE_Y,
      },
      {

          NOPI,
          IMPLIED,
      },
      {

          SBC,
          ABSOLUTE_X,
      },
      {

          INC,
          ABSOLUTE_X,
      },
      {

          ISB,
          ABSOLUTE_X,
      },
  };

  const int cycles[] = {
      /*0x00*/ 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
      /*0x10*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
      /*0x20*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
      /*0x30*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
      /*0x40*/ 6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
      /*0x50*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
      /*0x60*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
      /*0x70*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
      /*0x80*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
      /*0x90*/ 2, 6, 2, 6, 4, 4, 4, 4, 2, 4, 2, 5, 5, 4, 5, 5,
      /*0xA0*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
      /*0xB0*/ 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
      /*0xC0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
      /*0xD0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
      /*0xE0*/ 2, 6, 3, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
      /*0xF0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  };
} // namespace NES

#endif