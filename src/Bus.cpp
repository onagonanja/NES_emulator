#include "header/Bus.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

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
        (data[4] *
         0x4000); // ヘッダー16バイト+プログラムデータのページ数*ページ数当たりのバイト数
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
    // cout << "read(" << hex << setw(4) << setfill('0') << addr << ")=" << hex
    // << setw(4) << setfill('0') << res << " ";
    return res;
  }
} // namespace NES