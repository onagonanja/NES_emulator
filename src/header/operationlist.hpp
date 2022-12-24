#ifndef INCLUDED_OPERATIONLIST
#define INCLUDED_OPERATIONLIST

#include <map>
#include <string>
using namespace std;

vector<map<string, string>> opelist =
    {
        {
            {"fullName", "BRK"},
            {"baseName", "BRK"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_INDX"},
            {"baseName", "ORA"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SLO_INDX"},
            {"baseName", "SLO"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_ZERO"},
            {"baseName", "ORA"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "ASL_ZERO"},
            {"baseName", "ASL"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "SLO_ZERO"},
            {"baseName", "SLO"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "PHP"},
            {"baseName", "PHP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_IMM"},
            {"baseName", "ORA"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "ASL"},
            {"baseName", "ASL"},
            {"mode", "accumulator"},
        },
        {},
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_ABS"},
            {"baseName", "ORA"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "ASL_ABS"},
            {"baseName", "ASL"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "SLO_ABS"},
            {"baseName", "SLO"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BPL"},
            {"baseName", "BPL"},
            {"mode", "relative"},
        },
        {
            {"fullName", "ORA_INDY"},
            {"baseName", "ORA"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SLO_INDY"},
            {"baseName", "SLO"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_ZEROX"},
            {"baseName", "ORA"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "ASL_ZEROX"},
            {"baseName", "ASL"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "SLO_ZEROX"},
            {"baseName", "SLO"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "CLC"},
            {"baseName", "CLC"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_ABSY"},
            {"baseName", "ORA"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SLO_ABSY"},
            {"baseName", "SLO"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ORA_ABSX"},
            {"baseName", "ORA"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "ASL_ABSX"},
            {"baseName", "ASL"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "SLO_ABSX"},
            {"baseName", "SLO"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "JSR_ABS"},
            {"baseName", "JSR"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "AND_INDX"},
            {"baseName", "AND"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "RLA_INDX"},
            {"baseName", "RLA"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "BIT_ZERO"},
            {"baseName", "BIT"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "AND_ZERO"},
            {"baseName", "AND"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "ROL_ZERO"},
            {"baseName", "ROL"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "RLA_ZERO"},
            {"baseName", "RLA"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "PLP"},
            {"baseName", "PLP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "AND_IMM"},
            {"baseName", "AND"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "ROL"},
            {"baseName", "ROL"},
            {"mode", "accumulator"},
        },
        {},
        {
            {"fullName", "BIT_ABS"},
            {"baseName", "BIT"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "AND_ABS"},
            {"baseName", "AND"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "ROL_ABS"},
            {"baseName", "ROL"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "RLA_ABS"},
            {"baseName", "RLA"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BMI"},
            {"baseName", "BMI"},
            {"mode", "relative"},
        },
        {
            {"fullName", "AND_INDY"},
            {"baseName", "AND"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "RLA_INDY"},
            {"baseName", "RLA"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "AND_ZEROX"},
            {"baseName", "AND"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "ROL_ZEROX"},
            {"baseName", "ROL"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "RLA_ZEROX"},
            {"baseName", "RLA"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "SEC"},
            {"baseName", "SEC"},
            {"mode", "implied"},
        },
        {
            {"fullName", "AND_ABSY"},
            {"baseName", "AND"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "RLA_ABSY"},
            {"baseName", "RLA"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "AND_ABSX"},
            {"baseName", "AND"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "ROL_ABSX"},
            {"baseName", "ROL"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "RLA_ABSX"},
            {"baseName", "RLA"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "RTI"},
            {"baseName", "RTI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "EOR_INDX"},
            {"baseName", "EOR"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SRE_INDX"},
            {"baseName", "SRE"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "EOR_ZERO"},
            {"baseName", "EOR"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "LSR_ZERO"},
            {"baseName", "LSR"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "SRE_ZERO"},
            {"baseName", "SRE"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "PHA"},
            {"baseName", "PHA"},
            {"mode", "implied"},
        },
        {
            {"fullName", "EOR_IMM"},
            {"baseName", "EOR"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "LSR"},
            {"baseName", "LSR"},
            {"mode", "accumulator"},
        },
        {},
        {
            {"fullName", "JMP_ABS"},
            {"baseName", "JMP"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "EOR_ABS"},
            {"baseName", "EOR"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "LSR_ABS"},
            {"baseName", "LSR"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "SRE_ABS"},
            {"baseName", "SRE"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BVC"},
            {"baseName", "BVC"},
            {"mode", "relative"},
        },
        {
            {"fullName", "EOR_INDY"},
            {"baseName", "EOR"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SRE_INDY"},
            {"baseName", "SRE"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "EOR_ZEROX"},
            {"baseName", "EOR"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "LSR_ZEROX"},
            {"baseName", "LSR"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "SRE_ZEROX"},
            {"baseName", "SRE"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "CLI"},
            {"baseName", "CLI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "EOR_ABSY"},
            {"baseName", "EOR"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SRE_ABSY"},
            {"baseName", "SRE"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "EOR_ABSX"},
            {"baseName", "EOR"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "LSR_ABSX"},
            {"baseName", "LSR"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "SRE_ABSX"},
            {"baseName", "SRE"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "RTS"},
            {"baseName", "RTS"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ADC_INDX"},
            {"baseName", "ADC"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "RRA_INDX"},
            {"baseName", "RRA"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ADC_ZERO"},
            {"baseName", "ADC"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "ROR_ZERO"},
            {"baseName", "ROR"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "RRA_ZERO"},
            {"baseName", "RRA"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "PLA"},
            {"baseName", "PLA"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ADC_IMM"},
            {"baseName", "ADC"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "ROR"},
            {"baseName", "ROR"},
            {"mode", "accumulator"},
        },
        {},
        {
            {"fullName", "JMP_INDABS"},
            {"baseName", "JMP"},
            {"mode", "indirectAbsolute"},
        },
        {
            {"fullName", "ADC_ABS"},
            {"baseName", "ADC"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "ROR_ABS"},
            {"baseName", "ROR"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "RRA_ABS"},
            {"baseName", "RRA"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BVS"},
            {"baseName", "BVS"},
            {"mode", "relative"},
        },
        {
            {"fullName", "ADC_INDY"},
            {"baseName", "ADC"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "RRA_INDY"},
            {"baseName", "RRA"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ADC_ZEROX"},
            {"baseName", "ADC"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "ROR_ZEROX"},
            {"baseName", "ROR"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "RRA_ZEROX"},
            {"baseName", "RRA"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "SEI"},
            {"baseName", "SEI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ADC_ABSY"},
            {"baseName", "ADC"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "RRA_ABSY"},
            {"baseName", "RRA"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ADC_ABSX"},
            {"baseName", "ADC"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "ROR_ABSX"},
            {"baseName", "ROR"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "RRA_ABSX"},
            {"baseName", "RRA"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "STA_INDX"},
            {"baseName", "STA"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SAX_INDX"},
            {"baseName", "SAX"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "STY_ZERO"},
            {"baseName", "STY"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "STA_ZERO"},
            {"baseName", "STA"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "STX_ZERO"},
            {"baseName", "STX"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "SAX_ZERO"},
            {"baseName", "SAX"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "DEY"},
            {"baseName", "DEY"},
            {"mode", "implied"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "TXA"},
            {"baseName", "TXA"},
            {"mode", "implied"},
        },
        {},
        {
            {"fullName", "STY_ABS"},
            {"baseName", "STY"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "STA_ABS"},
            {"baseName", "STA"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "STX_ABS"},
            {"baseName", "STX"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "SAX_ABS"},
            {"baseName", "SAX"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BCC"},
            {"baseName", "BCC"},
            {"mode", "relative"},
        },
        {
            {"fullName", "STA_INDY"},
            {"baseName", "STA"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {},
        {
            {"fullName", "STY_ZEROX"},
            {"baseName", "STY"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "STA_ZEROX"},
            {"baseName", "STA"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "STX_ZEROY"},
            {"baseName", "STX"},
            {"mode", "zeroPageY"},
        },
        {
            {"fullName", "SAX_ZEROY"},
            {"baseName", "SAX"},
            {"mode", "zeroPageY"},
        },
        {
            {"fullName", "TYA"},
            {"baseName", "TYA"},
            {"mode", "implied"},
        },
        {
            {"fullName", "STA_ABSY"},
            {"baseName", "STA"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "TXS"},
            {"baseName", "TXS"},
            {"mode", "implied"},
        },
        {},
        {},
        {
            {"fullName", "STA_ABSX"},
            {"baseName", "STA"},
            {"mode", "absoluteX"},
        },
        {},
        {},
        {
            {"fullName", "LDY_IMM"},
            {"baseName", "LDY"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "LDA_INDX"},
            {"baseName", "LDA"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "LDX_IMM"},
            {"baseName", "LDX"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "LAX_INDX"},
            {"baseName", "LAX"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "LDY_ZERO"},
            {"baseName", "LDY"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "LDA_ZERO"},
            {"baseName", "LDA"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "LDX_ZERO"},
            {"baseName", "LDX"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "LAX_ZERO"},
            {"baseName", "LAX"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "TAY"},
            {"baseName", "TAY"},
            {"mode", "implied"},
        },
        {
            {"fullName", "LDA_IMM"},
            {"baseName", "LDA"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "TAX"},
            {"baseName", "TAX"},
            {"mode", "implied"},
        },
        {},
        {
            {"fullName", "LDY_ABS"},
            {"baseName", "LDY"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "LDA_ABS"},
            {"baseName", "LDA"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "LDX_ABS"},
            {"baseName", "LDX"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "LAX_ABS"},
            {"baseName", "LAX"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BCS"},
            {"baseName", "BCS"},
            {"mode", "relative"},
        },
        {
            {"fullName", "LDA_INDY"},
            {"baseName", "LDA"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "LAX_INDY"},
            {"baseName", "LAX"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "LDY_ZEROX"},
            {"baseName", "LDY"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "LDA_ZEROX"},
            {"baseName", "LDA"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "LDX_ZEROY"},
            {"baseName", "LDX"},
            {"mode", "zeroPageY"},
        },
        {
            {"fullName", "LAX_ZEROY"},
            {"baseName", "LAX"},
            {"mode", "zeroPageY"},
        },
        {
            {"fullName", "CLV"},
            {"baseName", "CLV"},
            {"mode", "implied"},
        },
        {
            {"fullName", "LDA_ABSY"},
            {"baseName", "LDA"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "TSX"},
            {"baseName", "TSX"},
            {"mode", "implied"},
        },
        {},
        {
            {"fullName", "LDY_ABSX"},
            {"baseName", "LDY"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "LDA_ABSX"},
            {"baseName", "LDA"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "LDX_ABSY"},
            {"baseName", "LDX"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "LAX_ABSY"},
            {"baseName", "LAX"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "CPY_IMM"},
            {"baseName", "CPY"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "CMP_INDX"},
            {"baseName", "CMP"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "DCP_INDX"},
            {"baseName", "DCP"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "CPY_ZERO"},
            {"baseName", "CPY"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "CMP_ZERO"},
            {"baseName", "CMP"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "DEC_ZERO"},
            {"baseName", "DEC"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "DCP_ZERO"},
            {"baseName", "DCP"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "INY"},
            {"baseName", "INY"},
            {"mode", "implied"},
        },
        {
            {"fullName", "CMP_IMM"},
            {"baseName", "CMP"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "DEX"},
            {"baseName", "DEX"},
            {"mode", "implied"},
        },
        {},
        {
            {"fullName", "CPY_ABS"},
            {"baseName", "CPY"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "CMP_ABS"},
            {"baseName", "CMP"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "DEC_ABS"},
            {"baseName", "DEC"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "DCP_ABS"},
            {"baseName", "DCP"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BNE"},
            {"baseName", "BNE"},
            {"mode", "relative"},
        },
        {
            {"fullName", "CMP_INDY"},
            {"baseName", "CMP"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "DCP_INDY"},
            {"baseName", "DCP"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "CMP_ZEROX"},
            {"baseName", "CMP"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "DEC_ZEROX"},
            {"baseName", "DEC"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "DCP_ZEROX"},
            {"baseName", "DCP"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "CLD"},
            {"baseName", "CLD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "CMP_ABSY"},
            {"baseName", "CMP"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "DCP_ABSY"},
            {"baseName", "DCP"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "CMP_ABSX"},
            {"baseName", "CMP"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "DEC_ABSX"},
            {"baseName", "DEC"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "DCP_ABSX"},
            {"baseName", "DCP"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "CPX_IMM"},
            {"baseName", "CPX"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "SBC_INDX"},
            {"baseName", "SBC"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ISB_INDX"},
            {"baseName", "ISB"},
            {"mode", "preIndexedIndirect"},
        },
        {
            {"fullName", "CPX_ZERO"},
            {"baseName", "CPX"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "SBC_ZERO"},
            {"baseName", "SBC"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "INC_ZERO"},
            {"baseName", "INC"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "ISB_ZERO"},
            {"baseName", "ISB"},
            {"mode", "zeroPage"},
        },
        {
            {"fullName", "INX"},
            {"baseName", "INX"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SBC_IMM"},
            {"baseName", "SBC"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SBC_IMM"},
            {"baseName", "SBC"},
            {"mode", "immediate"},
        },
        {
            {"fullName", "CPX_ABS"},
            {"baseName", "CPX"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "SBC_ABS"},
            {"baseName", "SBC"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "INC_ABS"},
            {"baseName", "INC"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "ISB_ABS"},
            {"baseName", "ISB"},
            {"mode", "absolute"},
        },
        {
            {"fullName", "BEQ"},
            {"baseName", "BEQ"},
            {"mode", "relative"},
        },
        {
            {"fullName", "SBC_INDY"},
            {"baseName", "SBC"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ISB_INDY"},
            {"baseName", "ISB"},
            {"mode", "postIndexedIndirect"},
        },
        {
            {"fullName", "NOPD"},
            {"baseName", "NOPD"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SBC_ZEROX"},
            {"baseName", "SBC"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "INC_ZEROX"},
            {"baseName", "INC"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "ISB_ZEROX"},
            {"baseName", "ISB"},
            {"mode", "zeroPageX"},
        },
        {
            {"fullName", "SED"},
            {"baseName", "SED"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SBC_ABSY"},
            {"baseName", "SBC"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOP"},
            {"baseName", "NOP"},
            {"mode", "implied"},
        },
        {
            {"fullName", "ISB_ABSY"},
            {"baseName", "ISB"},
            {"mode", "absoluteY"},
        },
        {
            {"fullName", "NOPI"},
            {"baseName", "NOPI"},
            {"mode", "implied"},
        },
        {
            {"fullName", "SBC_ABSX"},
            {"baseName", "SBC"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "INC_ABSX"},
            {"baseName", "INC"},
            {"mode", "absoluteX"},
        },
        {
            {"fullName", "ISB_ABSX"},
            {"baseName", "ISB"},
            {"mode", "absoluteX"},
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

#endif