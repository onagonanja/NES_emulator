#ifndef INCLUDED_CPU
#define INCLUDED_CPU

#include <map>
#include <set>
#include <string>
#include <vector>

#include "defs.hpp"

using namespace std;

class CPU {
 public:
   //-----------------------CPU-----------------------//

   // レジスタ
   map<string, Address> registers = {
       {"A", 0x00}, {"X", 0x00}, {"Y", 0x00}, {"P", 0x00}, {"S", 0xff}, {"PC", 0x0000}, {"negative", false}, {"overflow", false}, {"reserved", true}, {"break", true}, {"decimal", false}, {"interrupt", true}, {"zero", false}, {"carry", false},
   };

   // メモリ
   Byte mem[65536] = {0};

   // キャラクターRom
   vector<vector<Byte>> CharacterRom;

   // PPUaddrレジスタの状態
   bool ppuaddrCnt = false;

   // PPU書き込み先のアドレス
   Address ppu_write_addr = 0;

   // testようのoption
   void test_option();

   // リセット
   void reset();

   // Non-Markable-Interrupt
   void NMI();

   // 値に応じてレジスタを変化させる
   void setRegisters(Byte num);

   // メモリ読み込み
   Byte read(Address addr);

   // メモリ書き込み
   void write(Address addr, Byte num);

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
   Address fetchOperand(string addr);

   // 命令コード、オペランドから命令を実行
   void exec(string opeName, Address opeland, string mode);

   // 実行,
   void run();

   // Rom読み込み
   void readROM();

   //-----------------------PPU-------------------------//

   // vram
   int vram[16384] = {0};

   // spriteram
   int spriteram[256] = {0};

   int readVRAM(int addr);

   void writeVRAM(int addr, int val);

   int readSpriteRAM(int addr);

   void writeSpriteRAM(int addr, int num);

   vector<vector<vector<int>>> setPixcels();

   vector<int> getBackGroundColor(int num);

   vector<int> getSpriteColor(int num);

   //-----------------------Debug-------------------------//

   set<string> ope_appeared;

   void print_appeared_opelist();

   int count = 0;
};

#endif