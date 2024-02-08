#include "header/Bus.hpp"
#include "header/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

namespace NES {

  void Bus::readROM() {
    // std::string filename = "./rom/sample1.dat";
    // std::string filename = "./rom/roulette.nes";
    std::string filename = "./rom/NEStress.NES";
    // std::string filename = "./rom/masmix.nes";
    // std::string filename = "./rom/TK20NTSC.NES";
    // std::string filename = "./rom/hello.nes";
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
    int CharacterRomStart =
        0x0010 +
        (data[4] * 0x4000); // ヘッダー16バイト + プログラムデータのページ数 *
                            // ページ数当たりのバイト数
    int CharacterRomEnd = CharacterRomStart + (data[5] * 0x2000);
    characterRom.assign(data[5] * 0x2000, std::vector<Byte>(64));

    // プログラムをメモリにセット
    for(int i = 0; i < CharacterRomStart - 0x10; i++) {
      ram[i + 0x8000] = data[i + 0x10];
    }

    // キャラクターROMのデータをセット
    for(int i = 0; i * 16 < data[5] * 0x2000;
        i++) { // 1スプライトごとに長さ64の配列にする
      for(int j = 0; j < 8; j++) {
        int first = data[CharacterRomStart + i * 16 + j];
        int second = data[CharacterRomStart + i * 16 + 8 + j];
        for(int h = 0; h < 8; h++) {
          characterRom[i][j * 8 + h] =
              !!(first & 1 << (7 - h)) + !!(second & 1 << (7 - h)) * 2;
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
      if(addr == 0x2002) {
        // 読み込みで0x2005をクリア？
        ram[0x2005] = 0;
      } else if(addr == 0x2007) {
        // PPUメモリアドレスを自動で加算
        ram[0x2006] += 1;
      }
      res = ram[addr];
    } else {
      res = ram[addr];
    }

    Logger::logRead("RAM", addr, res);
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
      addr = 0x2000 + (addr - 0x2000) % 8;
      if(addr == 0x2004) {
        // スプライトメモリアドレス(0x2003)で指定されたアドレスへデータを書き込む。
        // 書き込む度にスプライトメモリアドレスはインクリメントされる
        writeVRAM(ram[0x2003], data);
        ram[0x2003]++;
      } else if(addr == 0x2004) {
        writeSpriteRAM(ram[0x2003], data);
        ram[0x2003]++;
      } else if(addr == 0x2006) {
        // 書き込み回数が偶数回ならアドレスの上位8bit,奇数なら下位8bitを書き込む
        if(vram_addr_state == 0) {
          vram_write_addr &= 0x00ff;
          vram_write_addr |= static_cast<Address>(data) << 8;
          ram[0x2006] = data;
        } else {
          vram_write_addr &= 0xff00;
          vram_write_addr |= data;
          ram[0x2006] = data;
        }
        vram_addr_state ^= 1;
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
    } else {
      ram[addr] = data;
    }
  }

  // VRAMを読み込む
  Byte Bus::readVRAM(Address addr) {
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
  void Bus::writeVRAM(Address addr, Byte num) {
    Logger::logWrite("VRAM", addr, num);

    if(0x3000 <= addr && addr <= 0x3eff) {
      addr = 0x2000 + (addr - 0x3000) % 0xF00;
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

  std::vector<Byte> Bus::readCharacterROM(Address addr) {
    return characterRom[addr];
  }

  void Bus::writeCharacterROM(Address addr, std::vector<Byte> data) {
    characterRom[addr] = data;
  }
} // namespace NES