#ifndef INCLUDED_CPU
#define INCLUDED_CPU

#include <map>
#include <string>
#include <vector>
using namespace std;

class CPU {
 public:
   //-----------------------CPU-----------------------//

   // レジスタ
   map<string, int> registers = {
       {"A", 0x00}, {"X", 0x00}, {"Y", 0x00}, {"P", 0x00}, {"S", 0xff}, {"PC", 0x8000}, {"negative", false}, {"overflow", false}, {"reserved", true}, {"break", true}, {"decimal", false}, {"interrupt", true}, {"zero", false}, {"carry", false},
   };

   // メモリ
   int mem[65536] = {0};

   // ROMデータを格納する配列
   vector<int> ROMdata;

   // キャラクターRom
   vector<vector<int>> CharacterRom;

   // PPUaddrレジスタの状態
   int ppuaddrCnt = 0;

   // リセット
   void reset();

   // 値に応じてレジスタを変化させる
   void setRegisters(int num);

   // メモリ読み込み
   int read(int addr);

   // メモリ書き込み
   void write(int addr, int num);

   // スタックに値をpush
   void push_stack(int data);

   // スタックから値をpop
   int pop_stack();

   // PCレジスタをインクリメントしつつその内容を返す
   int fetch();

   // アドレッシングモードを判別
   int fetchOperand(string addr);

   // 命令コード、オペランドから命令を実行
   void exec(string opeName, int opeland, string mode);

   // 実行,
   void run();

   // Rom読み込み
   void readROM();

   //-----------------------PPU-------------------------//

   // vram
   int vram[16384] = {0};

   int readVRAM(int addr);

   void writeVRAM(int addr, int val);

   vector<vector<vector<int>>> setPixcels();

   vector<int> getColor(int num);
};

#endif