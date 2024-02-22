#include "header/Bus.hpp"
#include "header/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

namespace NES {

  void Bus::handleKeyDOWN(SDL_Event &e) {
    if(e.key.keysym.sym == SDLK_ESCAPE) {
      std::cout << "START" << std::endl;
    } else if(e.key.keysym.sym == SDLK_UP) {
      std::cout << "UP" << std::endl;
    } else if(e.key.keysym.sym == SDLK_DOWN) {
      std::cout << "DOWN" << std::endl;
    } else if(e.key.keysym.sym == SDLK_LEFT) {
      std::cout << "LEFT" << std::endl;
    } else if(e.key.keysym.sym == SDLK_RIGHT) {
      std::cout << "RIGHT" << std::endl;
    }
  }

  void Bus::handleKeyUP(SDL_Event &e) {
    if(e.key.keysym.sym == SDLK_ESCAPE) {
    } else if(e.key.keysym.sym == SDLK_UP) {
    } else if(e.key.keysym.sym == SDLK_DOWN) {
    } else if(e.key.keysym.sym == SDLK_LEFT) {
    } else if(e.key.keysym.sym == SDLK_RIGHT) {
    }
  }

  void Bus::readROM() {
    // std::string filename = "./rom/roulette.nes";
    // std::string filename = "./rom/NEStress.NES";
    // std::string filename = "./rom/nestest.nes";
    // std::string filename = "./rom/scroll.nes";
    std::string filename = "./rom/masmix.nes";
    // std::string filename = "./rom/TK20NTSC.NES";
    // std::string filename = "./rom/hello.nes";
    // std::string filename = "./rom/nesmas.nes";
    // std::string filename = "./rom/firedemo.nes";
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    if(!ifs) {
      // cout << "ファイルが開けません";
      exit(1);
    }
    // cout << filename << endl;
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    ifs.seekg(0);
    unsigned char *data = new unsigned char[size];
    ifs.read((char *)data, size);

    // iNESヘッダーからプログラムROMとキャラクターROMの範囲を割り出す(バイト単位)
    int CharacterRomStart = 0x0010 + (data[4] * 0x4000); // ヘッダー16バイト + プログラムデータのページ数 *
                                                         // ページ数当たりのバイト数
    int CharacterRomEnd = CharacterRomStart + (data[5] * 0x2000);
    characterRom.assign(data[5] * 0x2000, std::vector<Byte>(64));

    std::cout << "CharacterRomStart: " << CharacterRomStart << std::endl;
    std::cout << "CharacterRomEnd: " << CharacterRomEnd << std::endl;

    int mapper_low = (data[6] & 0xF0) >> 4;
    int mapper_high = data[7] & 0xF0;
    int mapper = mapper_low | mapper_high;

    std::cout << "mapper: " << mapper << std::endl;

    banks = data[4];
    std::cout << "16KB PRG-ROM Banks: " << (int)banks << std::endl;

    // set nametable mirrorring
    if(data[6] & 1) {
      mirrorring = VERTICAL;
    } else {
      mirrorring = HORIZONAL;
    }

    // プログラムをメモリにセット
    for(int i = 0; i < CharacterRomStart - 0x10; i++) {
      ram[i + 0x8000] = data[i + 0x10];
    }

    // キャラクターROMのデータをセット
    for(int i = 0; i * 16 < data[5] * 0x2000; i++) { // 1スプライトごとに長さ64の配列にする
      for(int j = 0; j < 8; j++) {
        int first = data[CharacterRomStart + i * 16 + j];
        int second = data[CharacterRomStart + i * 16 + 8 + j];
        for(int h = 0; h < 8; h++) {
          characterRom[i][j * 8 + h] = !!(first & 1 << (7 - h)) + !!(second & 1 << (7 - h)) * 2;
        }
      }
    }
  }

  Byte Bus::readRAM(Address addr) {
    Byte res;
    // ミラー領域に対する読み込み
    if(addr >= 0x0800 && addr < 0x2000) {
      res = ram[addr - 0x0800];
    }
    // PPUレジスタに対する読み込み
    else if(addr >= 0x2000 && addr < 0x4000) {
      addr = 0x2000 + (addr - 0x2000) % 8;

      if(addr == 0x2000) {
        // PPUマスタースレーブ、常に1
        ram[0x2000] |= (1 << 6);
        res = ram[addr];
      } else if(addr == 0x2002) {
        // 読み込みで0x2005をクリア？
        ram[0x2005] = 0;
        res = ram[addr];
      } else if(addr == 0x2007) {
        res = readVRAM(vram_write_addr);
        Logger::logreadVRAM(vram_write_addr, res);
        // PPUメモリアドレスを自動で加算
        if(ram[0x2000] & (1 << 2)) {
          vram_write_addr += 32;
        } else {
          vram_write_addr++;
        }
      } else {
        res = ram[addr];
      }

    } else if(addr == 0x4016) {
      if(readCallbacks.find(addr) != readCallbacks.end()) {
        res = readCallbacks[addr]();
      }
    } else if(addr >= 0xC000 && addr < 0xFFFF) {
      if(banks == 1) {
        res = ram[addr - 0x4000];
      } else {
        res = ram[addr];
      }
    } else {
      res = ram[addr];
    }

    // Logger::logRead("RAM", addr, res);
    return res;
  }

  // メモリ書き込み
  void Bus::writeRAM(Address addr, Byte data) {
    Logger::logWrite("RAM", addr, data);

    // WRAMのミラー領域に対する書き込み
    if(addr >= 0x0800 && addr < 0x2000) {
      ram[0x0800 + addr % 0x0800] = data;
    }
    // PPUレジスタに対する書き込み
    else if(addr >= 0x2000 && addr < 0x4000) {
      // std::cout << "write PPU regs: " << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << addr << " " << (int)data
      //<< std::endl;
      addr = 0x2000 + (addr - 0x2000) % 8;
      if(addr == 0x2003) {
        // std::cout << "write sprite ram addr: " << data << std::endl;
        writeVRAM(ram[0x2003], data);
        sprite_ram_write_addr = data;
      } else if(addr == 0x2004) {
        // スプライトメモリアドレス(0x2003)で指定されたアドレスへデータを書き込む。
        // 書き込む度にスプライトメモリアドレスはインクリメントされる
        // std::cout << "write sprite ram :" << sprite_ram_write_addr << " " << data << std::endl;
        writeSpriteRAM(sprite_ram_write_addr, data);
        sprite_ram_write_addr++;
        ram[0x2003]++;
      } else if(addr == 0x2005) {
        // 書き込み回数が偶数回ならScroll_X,奇数ならScroll_Yを書き込む
        if(w == 0) {
          scroll_x = data;
        } else {
          scroll_y = data;
        }
        w ^= 1;
      } else if(addr == 0x2006) {
        // 書き込み回数が偶数回ならアドレスの上位8bit,奇数なら下位8bitを書き込む
        if(w == 0) {
          vram_write_addr &= 0x00ff;
          vram_write_addr |= static_cast<Address>(data) << 8;
          ram[0x2006] = data;
        } else {
          vram_write_addr &= 0xff00;
          vram_write_addr |= data;
          ram[0x2006] = data;
        }
        w ^= 1;
      } else if(addr == 0x2007) {
        // PPUDATAレジスタに書き込むことでPPUADDRレジスタから参照したアドレスへVRAMに間接的に書き込む
        writeVRAM(vram_write_addr, data);
        if(ram[0x2000] & (1 << 2)) {
          vram_write_addr += 32;
        } else {
          vram_write_addr++;
        }
      } else {
        ram[addr] = data;
      }
    } else if(addr == 0x4014) {
      for(int i = 0; i < 256; i++) {
        writeSpriteRAM(i, readRAM((static_cast<Address>(data) << 8) + i));
      }
    } else if(addr == 0x4016) {
      if(writeCallbacks.find(addr) != writeCallbacks.end()) {
        writeCallbacks[addr](data);
      }
    } else {
      ram[addr] = data;
    }
  }

  // VRAMを読み込む
  Byte Bus::readVRAM(Address addr) {
    // 0x3F10,0x3F14,0x3F18,0x3F1Cは0x3F00,0x3F04,0x3F08,0x3F0Cのミラー
    if(addr == 0x3F10 || addr == 0x3F14 || addr == 0x3F18 || addr == 0x3F1C) {
      return vram[addr - 0x10];
    } else if(0x2000 <= addr && addr < 0x23C0) {
      // nametable[0]
      return getNametable((addr - 0x2000) / NAMETABLE_X, (addr - 0x2000) % NAMETABLE_X);
    } else if(0x2400 <= addr && addr < 0x27C0) {
      // nametable[1]
      return getNametable((addr - 0x2400) / NAMETABLE_X, NAMETABLE_X + (addr - 0x2400) % NAMETABLE_X);
    } else if(0x2800 <= addr && addr < 0x2BC0) {
      // nametable[2]
      return getNametable(NAMETABLE_Y + (addr - 0x2800) / NAMETABLE_X, (addr - 0x2800) % NAMETABLE_X);
    } else if(0x2800 <= addr && addr < 0x2BC0) {
      // nametable[3]
      return getNametable(NAMETABLE_Y + (addr - 0x2C00) / NAMETABLE_X, NAMETABLE_X + (addr - 0x2C00) % NAMETABLE_X);
    } else if(0x3000 <= addr && addr <= 0x3eff) {
      return vram[0x2000 + (addr - 0x3000) % 0xF00];
    } else if(0x3f20 <= addr && addr <= 0x3fff) {
      return vram[0x3f00 + (addr - 0x3f20) % 0x20];
    } else {
      return vram[addr];
    }
  }

  // VRAMに書き込む
  void Bus::writeVRAM(Address addr, Byte num) {
    if(0x3000 <= addr && addr <= 0x3eff) {
      addr = 0x2000 + (addr - 0x3000) % 0xF00;
    } else if(0x2000 <= addr && addr < 0x23C0) {
      // nametable[0]
      nametable[(addr - 0x2000) / NAMETABLE_X][(addr - 0x2000) % NAMETABLE_X] = num;
    } else if(0x2400 <= addr && addr < 0x27C0) {
      // nametable[1]
      nametable[(addr - 0x2400) / NAMETABLE_X][NAMETABLE_X + (addr - 0x2400) % NAMETABLE_X] = num;
    } else if(0x2800 <= addr && addr < 0x2BC0) {
      nametable[NAMETABLE_Y + (addr - 0x2800) / NAMETABLE_X][(addr - 0x2800) % NAMETABLE_X] = num;
    } else if(0x2800 <= addr && addr < 0x2BC0) {
      // nametable[3]
      nametable[NAMETABLE_Y + (addr - 0x2C00) / NAMETABLE_X][NAMETABLE_X + (addr - 0x2C00) % NAMETABLE_X] = num;
    } else if(0x3f20 <= addr && addr <= 0x3fff) {
      addr = 0x3f00 + ((addr - 0x3f20) % 0x20);
    }
    vram[addr] = num;
  }

  Byte Bus::readSpriteRAM(Address addr) { return spriteram[addr]; }

  void Bus::writeSpriteRAM(Address addr, Byte num) {
    Logger::logWrite("SpriteRAM", addr, num);

    spriteram[addr] = num;
  }

  std::vector<Byte> Bus::readCharacterROM(Address addr) { return characterRom[addr]; }

  void Bus::writeCharacterROM(Address addr, std::vector<Byte> data) { characterRom[addr] = data; }

  Byte Bus::getScrollX() { return scroll_x; }
  Byte Bus::getScrollY() { return scroll_y; }

  Byte Bus::getNametable(int y, int x) {
    if(mirrorring == HORIZONAL) {
      x %= NAMETABLE_X;
    } else {
      y %= NAMETABLE_Y;
    }

    return nametable[y % (NAMETABLE_Y * 2)][x % (NAMETABLE_X * 2)];
  }

  void Bus::setReadCallback(Address addr, std::function<Byte(void)> func) { readCallbacks[addr] = func; }

  void Bus::setWriteCallback(Address addr, std::function<void(Byte)> func) { writeCallbacks[addr] = func; }
} // namespace NES