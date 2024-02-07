#include "header/CPU.hpp"
#include "header/Pallete.hpp"
#include <iomanip>
#include <iostream>
using namespace std;

// VRAMを読み込む
Byte CPU::readVRAM(Address addr) {
  // 0x3F10,0x3F14,0x3F18,0x3F1Cは0x3F00,0x3F04,0x3F08,0x3F0Cのミラー
  if(addr == 0x3F10 || addr == 0x3F14 || addr == 0x3F18 || addr == 0x3F1C) {
    return vram[addr - 0x10];
  } else if(0x3000 <= addr && addr <= 0x3eff) {
    return vram[0x2000 + (addr - 0x3000) % 0xF00];
  } else if(0x3f20 <= addr && addr <= 0x3fff) {
    return vram[0x3f00 + (addr - 0x3f20) % 0x20];
  } else {
    return vram[addr];
  }
}

// VRAMに書き込む
void CPU::writeVRAM(Address addr, Byte num) {
  // cout << "writeVRAMbefore(" << hex << setw(4) << setfill('0') << addr << "," << hex << setw(4) << setfill('0') << num << ") ";
  if(0x3000 <= addr && addr <= 0x3eff) {
    addr = 0x2000 + (addr - 0x3000) % 0xF00;
  } else if(0x3f20 <= addr && addr <= 0x3fff) {
    addr = 0x3f00 + ((addr - 0x3f20) % 0x20);
  }
  vram[addr] = num;
  // cout << "writeVRAMafter(" << hex << setw(4) << setfill('0') << addr << "," << hex << setw(4) << setfill('0') << num << ") ";
}

Byte CPU::readSpriteRAM(Address addr) { return spriteram[addr]; }

void CPU::writeSpriteRAM(Address addr, Byte num) {
  spriteram[addr] = num;
  // cout << "write sprite:" << addr << " " << num << endl;
}

vector<Byte> CPU::getBackGroundColor(Byte data) {
  vector<Byte> res(4);
  res[0] = readVRAM(0x3f00 + data * 4);
  res[1] = readVRAM(0x3f00 + data * 4 + 1);
  res[2] = readVRAM(0x3f00 + data * 4 + 2);
  res[3] = readVRAM(0x3f00 + data * 4 + 3);
  return res;
}

vector<Byte> CPU::getSpriteColor(Byte data) {
  vector<Byte> res(4);
  res[0] = readVRAM(0x3f10 + data * 4);
  res[1] = readVRAM(0x3f10 + data * 4 + 1);
  res[2] = readVRAM(0x3f10 + data * 4 + 2);
  res[3] = readVRAM(0x3f10 + data * 4 + 3);
  return res;
}

int getSpritNum(int y, int x) { return 1; }

// vramからピクセルデータを取得してそのデータを返す
vector<vector<vector<int>>> CPU::setPixcels() {
  /*------------------Render BackGround------------------------*/

  int nameTable[30][32]; //[縦][横]
  const int NAMETABLENUM = (read(0x2000) & 1 << 1) * 2 + (read(0x2000) & 1);
  const int NAMETABLESTART[4] = {0x2000, 0x2400, 0x2800, 0x2C00};

  for(int i = 0; i < 30; i++) {
    for(int j = 0; j < 32; j++) {
      nameTable[i][j] = readVRAM(NAMETABLESTART[NAMETABLENUM] + i * 32 + j);
    }
  }

  int paretTable[15][16]; //[縦][横]
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      int data = readVRAM(0x23C0 + i * 8 + j);
      paretTable[i * 2][j * 2] = data & 0b11;
      paretTable[i * 2][j * 2 + 1] = (data & 0b1100) >> 2;
      if(i * 2 + 1 < 15) {
        paretTable[i * 2 + 1][j * 2] = (data & 0b110000) >> 4;
        paretTable[i * 2 + 1][j * 2 + 1] = (data & 0b11000000) >> 6;
      }
    }
  }

  vector<vector<vector<int>>> res(30 * 8, vector<vector<int>>(32 * 8, vector<int>(3)));

  for(int i = 0; i < 30; i++) {
    for(int j = 0; j < 32; j++) {
      int paletteNUM = paretTable[i / 2][j / 2];
      vector<Byte> sprite = CharacterRom[nameTable[i][j]];
      vector<Byte> pallete = getBackGroundColor(paletteNUM);
      for(int di = 0; di < 8; di++) {
        for(int dj = 0; dj < 8; dj++) {
          res[i * 8 + di][j * 8 + dj][0] = colors[pallete[sprite[di * 8 + dj]]][0];
          res[i * 8 + di][j * 8 + dj][1] = colors[pallete[sprite[di * 8 + dj]]][1];
          res[i * 8 + di][j * 8 + dj][2] = colors[pallete[sprite[di * 8 + dj]]][2];
        }
      }
    }
  }

  /*----------------------Render Sprite------------------------*/

  for(int i = 0; i < 256; i += 4) {
    if(spriteram[i] >= 0xff) {
      continue;
    }
    int y = spriteram[i] + 1;
    int x = spriteram[i + 3];
    int palletenum = spriteram[i + 2] & 0b11;
    vector<Byte> sprite = CharacterRom[spriteram[i + 1]];
    vector<Byte> pallete = getSpriteColor(palletenum);
    for(int dy = 0; dy < 8; dy++) {
      for(int dx = 0; dx < 8; dx++) {
        int absY = y + dy;
        int absX = x + dx;
        if(absX >= 240 || absY >= 256) {
          continue;
        }
        res[absX][absY][0] = colors[pallete[sprite[dx * 8 + dy]]][0];
        res[absX][absY][1] = colors[pallete[sprite[dx * 8 + dy]]][1];
        res[absX][absY][2] = colors[pallete[sprite[dx * 8 + dy]]][2];
      }
    }
  }

  return res;
}