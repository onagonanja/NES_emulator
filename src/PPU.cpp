#include "header/CPU.hpp"
#include "header/Pallete.hpp"
#include <iomanip>
#include <iostream>
using namespace std;

// VRAMを読み込む
int CPU::readVRAM(int addr) {
   if (0x3000 <= addr && addr <= 0x3eff) {
      return vram[0x2000 + (addr - 0x3000) % 0xF00];
   } else if (0x3f20 <= addr && addr <= 0x3fff) {
      return vram[0x3f00 + (addr - 0x3f20) % 0x20];
   } else {
      return vram[addr];
   }
}

// VRAMに書き込む
void CPU::writeVRAM(int addr, int num) {
   cout << "writeVRAMbefore(" << hex << setw(4) << setfill('0') << addr << "," << hex << setw(4) << setfill('0') << num << ") ";
   if (0x3000 <= addr && addr <= 0x3eff) {
      addr = 0x2000 + (addr - 0x3000) % 0xF00;
   } else if (0x3f20 <= addr && addr <= 0x3fff) {
      addr = 0x3f00 + ((addr - 0x3f20) % 0x20);
   }
   vram[addr] = num;
   cout << "writeVRAMafter(" << hex << setw(4) << setfill('0') << addr << "," << hex << setw(4) << setfill('0') << num << ") ";
}

vector<int> CPU::getColor(int num) {
   vector<int> res(4);
   res[0] = readVRAM(0x3f00 + num * 4);
   res[1] = readVRAM(0x3f00 + num * 4 + 1);
   res[2] = readVRAM(0x3f00 + num * 4 + 2);
   res[3] = readVRAM(0x3f00 + num * 4 + 3);
   return res;
}

int getSpritNum(int y, int x) { return 1; }

// vramからピクセルデータを取得してそのデータを返す
vector<vector<vector<int>>> CPU::setPixcels() {
   int nameTable[30][32]; //[縦][横]

   for (int i = 0; i < 30; i++) {
      for (int j = 0; j < 32; j++) {
         nameTable[i][j] = readVRAM(0x2000 + i * 32 + j);
      }
   }

   int paretTable[15][16]; //[縦][横]
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
         int data = readVRAM(0x23C0 + i * 8 + j);
         paretTable[i * 2][j * 2] = data & 0b11;
         paretTable[i * 2][j * 2 + 1] = (data & 0b1100) >> 2;
         if (i * 2 + 1 < 15) {
            paretTable[i * 2 + 1][j * 2] = (data & 0b110000) >> 4;
            paretTable[i * 2 + 1][j * 2 + 1] = (data & 0b11000000) >> 6;
         }
      }
   }

   vector<vector<vector<int>>> res(30 * 8, vector<vector<int>>(32 * 8, vector<int>(3)));

   for (int i = 0; i < 30; i++) {
      for (int j = 0; j < 32; j++) {
         int paletteNUM = paretTable[i / 2][j / 2];
         vector<int> sprite = CharacterRom[nameTable[i][j]];
         vector<int> pallete = getColor(paletteNUM);
         for (int di = 0; di < 8; di++) {
            for (int dj = 0; dj < 8; dj++) {
               res[i * 8 + di][j * 8 + dj][0] = colors[pallete[sprite[di * 8 + dj]]][0];
               res[i * 8 + di][j * 8 + dj][1] = colors[pallete[sprite[di * 8 + dj]]][1];
               res[i * 8 + di][j * 8 + dj][2] = colors[pallete[sprite[di * 8 + dj]]][2];
            }
         }
      }
   }
   return res;
}