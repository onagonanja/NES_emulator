#include "header/CPU.hpp"
#include "header/defs.hpp"
#include "header/operationlist.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

// リセット
void CPU::reset() { registers["PC"] = read(0xFFFC) | 0x8000; }

// メモリ読み込み
int CPU::read(int addr) {
   if (addr >= 0x0800 && addr < 0x2000) {
      return mem[addr - 0x0800]; // ミラー領域
   } else if (addr >= 0x2000 && addr < 0x4000) {
      // PPUレジスタ
      addr = (addr - 0x2000) % 8;
      return mem[addr - 0x2000];
   } else {
      return mem[addr];
   }
}

// メモリに書き込む
void CPU::write(int addr, int num) {
   if (addr >= 0x0800 && addr < 0x2000) {
      // 書き込み先がミラー領域の場合
      mem[addr - 0x0800] = num;
   } else if (addr >= 0x2000 && addr < 0x4000) {
      // 書き込み先がPPUレジスタの場合
      addr = 0x2000 + (addr - 0x2000) % 8;
      if (addr == 0x2007) {
         // PPUDATAレジスタに書き込むことで、PPUADDRレジスタから参照したアドレスへ、VRAMに間接的に書き込む
         writeVRAM(mem[0x2006], num);
         mem[0x2006]++;
      } else if (addr == 0x2006) {
         // 書き込み回数が偶数回ならアドレスの上位16bit,奇数なら下位16bitを書き込む
         if (ppuaddrCnt == 0) {
            mem[0x2006] = num << 8 | (mem[0x2006] & 0x00ff);
         } else {
            mem[0x2006] = num | (mem[0x2006] & 0xff00);
         }
         ppuaddrCnt++;
         ppuaddrCnt %= 2;
      } else {
         mem[addr] = num;
      }
   } else {
      mem[addr] = num;
   }
}

// PCレジスタをインクリメントし、そのアドレスのデータを返す
int CPU::fetch() {
   int res = read(registers["PC"]++);
   return res;
}

// スタックに値をプッシュ
void CPU::push_stack(int data) {
   mem[STACK_START + registers["S"]] = registers["A"];
   registers["S"]--;
}

// スタックからpop
int CPU::pop_stack() {
   int res = mem[STACK_START + registers["S"]];
   registers["S"]++;
   return res;
}

// 実行
void CPU::run() {
   int code = fetch();
   map<string, string> ope = opelist[code];
   string opeName = ope["baseName"];
   string addressing = ope["mode"];
   int data = fetchOperand(addressing);
   exec(opeName, data, addressing);
}

// ROM読み込み
void CPU::readROM() {
   string filename = "./rom/helloworld.text";
   fstream ifs(filename);

   if (ifs.fail()) {
      cerr << "Failed to open file." << endl;
      return;
   }

   char ch;
   int cnt = 0;
   string num = "0x";

   while (ifs.get(ch)) {
      num += ch;
      cnt++;
      if (cnt % 2 == 0) {
         ROMdata.push_back(stoi(num, nullptr, 16)); // ROMデータを1バイトずつ配列に格納
         num = "0x";
      }
   }

   // iNESヘッダーからプログラムROMとキャラクターROMの範囲を割り出す(バイト単位)
   int CharacterRomStart = 0x0010 + (ROMdata[4] * 0x4000); // ヘッダー16バイト+プログラムデータのページ数*ページ数当たりのバイト数
   int CharacterRomEnd = CharacterRomStart + (ROMdata[5] * 0x2000);

   CharacterRom.assign(ROMdata[5] * 0x2000, vector<int>(64));

   // プログラムをメモリにセット
   for (int i = 0; i < CharacterRomStart - 0x10; i++) {
      mem[i + 0x8000] = ROMdata[i + 0x10];
   }

   // キャラクターROMのデータをセット
   for (int i = 0; i * 16 < ROMdata[5] * 0x2000; i++) { // 1スプライトごとに長さ64の配列にする
      for (int j = 0; j < 8; j++) {
         int first = ROMdata[CharacterRomStart + i * 16 + j];
         int second = ROMdata[CharacterRomStart + i * 16 + 8 + j];
         for (int h = 0; h < 8; h++) {
            CharacterRom[i][j * 8 + h] = !!(first & 1 << (7 - h)) + !!(second & 1 << (7 - h)) * 2;
         }
      }
   }
}

// レジスタをセット
void CPU::setRegisters(int num) {
   registers["negative"] = !!(num & (1 << 7));
   registers["zero"] = (num == 0);
}

// アドレッシングモードを判別し、データを取得
int CPU::fetchOperand(string addr) {
   if (addr == "accumulator") {
      return -1;
   } else if (addr == "implied") {
      return -1;
   } else if (addr == "immediate") {
      return fetch();
   } else if (addr == "zeroPage") {
      return fetch();
   } else if (addr == "zeroPageX") {
      int add = fetch();
      return (add + (registers["X"] & 0xFF)) & 0xffff;
   } else if (addr == "zeroPageY") {
      int add = fetch();
      return (add + (registers["Y"] & 0xFF)) & 0xffff;
   } else if (addr == "absolute") {
      int add1 = fetch();
      int add2 = fetch() << 8;
      return (add1 + add2) & 0xffff;
   } else if (addr == "absoluteX") {
      int add1 = fetch();
      int add2 = fetch() << 8;
      return (add1 + add2 + registers["X"]) & 0xffff;
   } else if (addr == "absoluteY") {
      int add1 = fetch();
      int add2 = fetch() << 8;
      return (add1 + add2 + registers["Y"]) & 0xffff;
   } else if (addr == "preIndexedIndirect") {
      int addradd = fetch() + registers["X"];
      return ((mem[addradd + 1] << 8) + mem[addradd]) & 0xffff;
   } else if (addr == "postIndexedIndirect") {
      int addradd = fetch();
      int add1 = mem[addradd] << 8;
      int add2 = mem[addradd + 1];
      return (add1 + add2 + registers["Y"]) && 0xffff;
   } else if (addr == "indirectAbsolute") {
      int addradd = fetch() + (fetch() << 8);
      return mem[addradd] + (mem[addradd + 1] << 8) & 0xffff;
   } else if (addr == "relative") {
      int addr = fetch();
      // 8bitのaddrを16bitの２の補数表現に拡張
      if ((addr & 1 << 7)) {
         addr = 0b1111111100000000 | addr;
      }
      return (addr + registers["PC"]) & 0xffff;
   }
   return -1;
}

// 命令コード、データから命令を実行
void CPU::exec(string opeName, int data, string mode) {
   if (opeName == "LDA") { // データをレジスタにロード
      if (mode == "immediate") {
         registers["A"] = data;
      } else {
         registers["A"] = read(data);
      }
   } else if (opeName == "LDX") {
      if (mode == "immediate") {
         registers["X"] = data;
      } else {
         registers["X"] = read(data);
      }
   } else if (opeName == "LDY") {
      if (mode == "immediate") {
         registers["Y"] = data;
      } else {
         registers["Y"] = read(data);
      }
   } else if (opeName == "STA") { // レジスタのデータをメモリにストア
      write(data, registers["A"]);
   } else if (opeName == "STX") {
      write(data, registers["X"]);
   } else if (opeName == "STY") {
      write(data, registers["Y"]);
   } else if (opeName == "TAX") { // レジスタの値を別レジスタにコピー
      registers["X"] = registers["A"];
   } else if (opeName == "TAY") {
      registers["Y"] = registers["A"];
   } else if (opeName == "TSX") {
      registers["S"] = registers["X"];
   } else if (opeName == "TXA") {
      registers["X"] = registers["A"];
   } else if (opeName == "TXS") {
      registers["S"] = registers["X"];
   } else if (opeName == "TYA") {
      registers["A"] = registers["Y"];
   } else if (opeName == "ADC") { // 演算命令いろいろ
      int calc = read(data) + registers["A"] + registers["carry"];
      setRegisters(calc);
      registers["A"] = calc & 0xff;
   } else if (opeName == "ASL") {
      if (data == -1) {
         registers["A"] = registers["A"] << 1; // Aレジスタを左シフト(8bit以上は切り捨て) 7bit目をcarryフラグに代入
         setRegisters(registers["A"]);
         registers["A"] = registers["A"] & 0xff;
      } else {
         write(data, read(data) << 1); // 指定されたアドレスのbitを左シフト(8bit以上は切り捨て)
         setRegisters(read(data));
         write(data, read(data) & 0xff);
      }
   } else if (opeName == "BIT") {
      registers["zero"] = !!(registers["A"] & read(data)); // Aレジスタとメモリデータのand演算の結果をZレジスタに格納
      registers["overflow"] = !!(read(data) & 1 << 6);     // メモリデータの6bit目をVレジスタに格納
      registers["negative"] = !!(read(data) & 1 << 7);     // メモリデータの7bit目をNレジスタに格納
   } else if (opeName == "CMP") {
      if (registers["A"] >= read(data)) {
         registers["C"] = 1;
      } else {
         registers["C"] = 0;
      }
   } else if (opeName == "CPX") {
      if (registers["X"] >= read(data)) {
         registers["C"] = 1;
      } else {
         registers["C"] = 0;
      }
   } else if (opeName == "CPY") {
      if (registers["Y"] >= read(data)) {
         registers["C"] = 1;
      } else {
         registers["C"] = 0;
      }
   } else if (opeName == "DEC") {
      write(data, (read(data) + 0xff) & 0xff);
      setRegisters(read(data));
   } else if (opeName == "DEX") {
      registers["X"] = (registers["X"] + 0xff) & 0xff;
      setRegisters(registers["X"]);
   } else if (opeName == "DEY") {
      registers["Y"] = (registers["Y"] + 0xff) & 0xff;
      setRegisters(registers["Y"]);
   } else if (opeName == "EOR") {
      registers["A"] = registers["A"] ^ read(data);
      setRegisters(registers["A"]);
   } else if (opeName == "INC") {
      write(data, (read(data) + 1) & 0xff);
      setRegisters(read(data));
   } else if (opeName == "INX") {
      registers["X"] = (registers["X"] + 1) & 0xff;
      setRegisters(registers["X"]);
   } else if (opeName == "INY") {
      registers["Y"] = (registers["Y"] + 1) & 0xff;
      setRegisters(registers["Y"]);
   } else if (opeName == "LSR") {
      if (data == -1) {
         registers["A"] = (registers["A"] >> 1) & 0xff;
         setRegisters(registers["A"]);
      } else {
         write(data, (read(data) << 1) & 0xff);
         setRegisters(read(data));
      }
   } else if (opeName == "ORA") {
      registers["A"] = registers["A"] | read(data);
   } else if (opeName == "ROL") {
      if (data == -1) {
         registers["A"] = (registers["A"] << 1 | !!(registers["A"] & 1 << 7));
         setRegisters(registers["A"]);
         registers["A"] = registers["A"] & 0xff;
      } else {
         write(data, read(data) << 1 | !!(read(data) & 1));
         setRegisters(read(data));
         write(data, read(data) & 0xff);
      }
   } else if (opeName == "ROR") {
      if (data == -1) {
         registers["A"] = (registers["A"] >> 1 | (!!(registers["A"] & 1)) << 7);
         setRegisters(registers["A"]);
         registers["A"] = registers["A"] & 0xff;
      } else {
         write(data, (read(data) >> 1 | !!(read(data) & 1) << 7));
         setRegisters(read(data));
         write(data, read(data) & 0xff);
      }
   } else if (opeName == "SBC") {
      registers["A"] = registers["A"] + (~read(data) + 1) + (0x100 - !registers["carry"]);
      setRegisters(registers["A"]);
      registers["A"] = registers["A"] & 0xff;
   } else if (opeName == "PHA") {
      push_stack(registers["A"]);
   } else if (opeName == "PHP") {
      push_stack(registers["P"]);
   } else if (opeName == "PLA") {
      registers["A"] = pop_stack();
   } else if (opeName == "PLP") {
      registers["P"] = pop_stack();
   } else if (opeName == "JMP") {
      registers["PC"] = data;
   } else if (opeName == "JSR") {
      int lowByte = (registers["PC"] - 1) & 0xff;
      int highByte = ((registers["PC"] - 1) & 0xff00) >> 8;
      push_stack(highByte);
      push_stack(lowByte);
      registers["PC"] = data;
   } else if (opeName == "RTS") {
      int lowByte = pop_stack();
      int highByte = pop_stack() << 8;
      registers["PC"] = highByte + lowByte + 1;
   } else if (opeName == "RTI") {
      // 割り込みから復帰
   } else if (opeName == "BCC") {
      if (!registers["carry"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BCS") {
      if (registers["carry"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BEQ") {
      if (registers["zero"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BMI") {
      if (registers["negative"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BNE") {
      if (!registers["zero"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BPL") {
      if (!registers["negative"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BVC") {
      if (!registers["overflow"]) {
         registers["PC"] = data;
      }
   } else if (opeName == "BVS") {
      if (registers["overflow"]) {
         registers["PC"] = data;
      }
   }
}