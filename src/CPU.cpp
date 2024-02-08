#include "header/CPU.hpp"
#include "header/defs.hpp"
#include "header/operationlist.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <time.h>

namespace NES {

  void hex(int n) {
    // cout << hex << setw(4) << setfill('0') << n << endl;
  }

  // テスト用のoptionを記述
  void CPU::test_option() {
    // 常にVBRANK状態にする。
    mem[0x2002] = 0b10000000 | mem[0x2002];
  }

  // リセット
  void CPU::reset() {
    Address pc_lowByte = read(0xFFFC);
    Address pc_highByte = read(0xFFFD);
    r_PC = (pc_highByte << 8) | pc_lowByte;
    push_status_registers();
    r_status["interrupt"] = true;
    // cout << r_PC << endl;
  }

  // Non-Markable-Interrupt
  void CPU::NMI() {
    int pc_lowByte = read(0xFFFA);
    int pc_highByte = read(0xFFFB);
    r_PC = (pc_highByte << 8) | pc_lowByte;
    push_status_registers();
    r_status["interrupt"] = true;
    // cout << r_PC << endl;
  }

  // メモリ読み込み
  Byte CPU::read(Address addr) {
    Byte res;
    // ミラー領域に対する読み込み
    if(addr >= 0x0800 && addr < 0x2000) {
      res = mem[addr - 0x0800];
    }
    // PPUレジスタに対する読み込み
    else if(addr >= 0x2000 && addr < 0x4000) {
      addr = 0x2000 + (addr - 0x2000) % 8;
      if(addr == 0x2002) {
        // 読み込みで0x2005をクリア？
        mem[0x2005] = 0;
      } else if(addr == 0x2007) {
        // PPUメモリアドレスを自動で加算
        mem[0x2006] += 1;
      }
      res = mem[addr];
    } else {
      res = mem[addr];
    }
    // cout << "read(" << hex << setw(4) << setfill('0') << addr << ")=" << hex
    // << setw(4) << setfill('0') << res << " ";
    return res;
  }

  // メモリ書き込み
  void CPU::write(Address addr, Byte data) {
    // cout << "write(" << hex << setw(4) << setfill('0') << addr << "," << hex
    // << setw(4) << setfill('0') << num << ") ";

    // WRAMのミラー領域に対する書き込み
    if(addr >= 0x0800 && addr < 0x2000) {
      mem[addr % 0x0800] = data;
    }
    // PPUレジスタに対する書き込み
    else if(addr >= 0x2000 && addr < 0x4000) {
      addr = 0x2000 + (addr - 0x2000) % 8;
      if(addr == 0x2004) {
        // スプライトメモリアドレス(0x2003)で指定されたアドレスへデータを書き込む。
        // 書き込む度にスプライトメモリアドレスはインクリメントされる
        writeVRAM(mem[0x2003], data);
        mem[0x2003]++;
      } else if(addr == 0x2004) {
        writeSpriteRAM(mem[0x2003], data);
        mem[0x2003]++;
      } else if(addr == 0x2006) {
        // 書き込み回数が偶数回ならアドレスの上位8bit,奇数なら下位8bitを書き込む
        if(ppuAddrState == 0) {
          ppu_write_addr &= 0x00ff;
          ppu_write_addr |= static_cast<Address>(data) << 8;
          mem[0x2006] = data;
        } else {
          ppu_write_addr &= 0xff00;
          ppu_write_addr |= data;
          mem[0x2006] = data;
        }
        ppuAddrState ^= 1;
      } else if(addr == 0x2007) {
        // PPUDATAレジスタに書き込むことで、PPUADDRレジスタから参照したアドレスへ、VRAMに間接的に書き込む
        writeVRAM(ppu_write_addr, data);
        if(mem[0x2000] & (1 << 2)) {
          ppu_write_addr += 32;
        } else {
          ppu_write_addr++;
        }
      } else {
        mem[addr] = data;
      }
    } else {
      mem[addr] = data;
    }
  }

  // PCレジスタをインクリメントし、そのアドレスのデータを返す
  Byte CPU::fetch() {
    Byte res = read(r_PC++);
    return res;
  }

  // スタックに値をプッシュ
  void CPU::push_stack(Byte data) {
    mem[STACK_START + r_SP] = data;
    r_SP--;
  }

  // スタックからpop
  Byte CPU::pop_stack() {
    Byte res = mem[STACK_START + r_SP + 1];
    r_SP++;
    // cout << "pop ";
    hex(res);
    return res;
  }

  // ステータスレジスタをスタックに退避
  void CPU::push_status_registers() {
    Byte data = r_status["negative"] << 7 | r_status["overflow"] << 6 |
                r_status["reserved"] << 5 | r_status["break"] << 4 |
                r_status["decimal"] << 3 | r_status["interrupt"] << 2 |
                r_status["zero"] << 1 | r_status["carry"];
    push_stack(data);
  }

  // スタックからステータスレジスタを復帰
  void CPU::pop_status_registers() {
    Byte data = pop_stack();
    r_status["carry"] = data;
    r_status["zero"] = data >> 1 & 1;
    r_status["interrupt"] = data >> 2 & 1;
    r_status["decimal"] = data >> 3 & 1;
    r_status["break"] = data >> 4 & 1;
    r_status["reserved"] = data >> 5 & 1;
    r_status["overflow"] = data >> 6 & 1;
    r_status["negative"] = data >> 7 & 1;
  }

  // 実行
  void CPU::run() {
    // cout << hex << setw(4) << setfill('0') << count++ << " ";
    test_option();
    Byte code = fetch();
    map<std::string, std::string> ope = opelist[code];
    std::string opeName = ope["baseName"];
    std::string addressing = ope["mode"];
    // cout << opeName << " ";
    ope_appeared.insert(opeName);
    Address data = fetchOperand(addressing);
    exec(opeName, data, addressing);
    // cout << "\n";
  }

  void CPU::print_appeared_opelist() {
    // cout << "---------------------Appeared
    // Opelations----------------------------" << endl;
    for(auto itr = ope_appeared.begin(); itr != ope_appeared.end(); ++itr) {
      // cout << *itr << endl;
    }
    // cout <<
    // "--------------------------------------------------------------------" <<
    // endl;
  };

  void CPU::readROM() {
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
    CharacterRom.assign(data[5] * 0x2000, std::vector<Byte>(64));

    // プログラムをメモリにセット
    for(int i = 0; i < CharacterRomStart - 0x10; i++) {
      mem[i + 0x8000] = data[i + 0x10];
    }

    // キャラクターROMのデータをセット
    for(int i = 0; i * 16 < data[5] * 0x2000;
        i++) { // 1スプライトごとに長さ64の配列にする
      for(int j = 0; j < 8; j++) {
        int first = data[CharacterRomStart + i * 16 + j];
        int second = data[CharacterRomStart + i * 16 + 8 + j];
        for(int h = 0; h < 8; h++) {
          CharacterRom[i][j * 8 + h] =
              !!(first & 1 << (7 - h)) + !!(second & 1 << (7 - h)) * 2;
        }
      }
    }
  }

  // レジスタをセット
  void CPU::setRegisters(Byte num) {
    r_status["negative"] = !!(num & (1 << 7));
    r_status["zero"] = (num == 0);
  }

  // アドレッシングモードを判別し、データを取得
  Address CPU::fetchOperand(std::string addr) {
    if(addr == "accumulator") {
      return -1;
    } else if(addr == "implied") {
      return -1;
    } else if(addr == "immediate") {
      return fetch();
    } else if(addr == "zeroPage") {
      return fetch();
    } else if(addr == "zeroPageX") {
      Address add = fetch();
      return add + r_X;
    } else if(addr == "zeroPageY") {
      Address add = fetch();
      return add + r_Y;
    } else if(addr == "absolute") {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2;
    } else if(addr == "absoluteX") {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2 + r_X;
    } else if(addr == "absoluteY") {
      Address add1 = fetch();
      Address add2 = static_cast<Address>(fetch()) << 8;
      return add1 + add2 + r_Y;
    } else if(addr == "preIndexedIndirect") {
      Address addradd = fetch() + r_X;
      Address addr1 = mem[addradd];
      Address addr2 = static_cast<Address>(mem[addradd + 1]) << 8;
      return addr1 + addr2;
    } else if(addr == "postIndexedIndirect") {
      Address addradd = fetch();
      Address addr1 = mem[addradd];
      Address addr2 = static_cast<Address>(mem[addradd + 1]) << 8;
      Address offset = r_Y;
      return addr1 + addr2 + offset;
    } else if(addr == "indirectAbsolute") {
      Address addradd_low = fetch();
      Address addradd_high = fetch();
      Address addr1 = mem[addradd_low + (addradd_high << 8)];
      Address addr2 = mem[(addradd_low + 1) & 0xff + (addradd_high) << 8];
      return addr1 + addr2;
    } else if(addr == "relative") {
      Address addr = static_cast<Address>(fetch());
      // 8bitのaddrを16bitの２の補数表現に拡張
      if((addr & (1 << 7))) {
        addr = 0b1111111100000000 | addr;
      }
      return addr + r_PC;
    }
    return -1;
  }

  // 命令コード、データから命令を実行
  void CPU::exec(string opeName, Address data, string mode) {
    if(opeName == "LDA") { // データをレジスタにロード
      if(mode == "immediate") {
        r_A = data;
      } else {
        r_A = read(data);
      }
      setRegisters(r_A);
    } else if(opeName == "LDX") {
      if(mode == "immediate") {
        r_X = data;
      } else {
        r_X = read(data);
      }
      setRegisters(r_X);
    } else if(opeName == "LDY") {
      if(mode == "immediate") {
        r_Y = data;
      } else {
        r_Y = read(data);
      }
      setRegisters(r_Y);
    } else if(opeName == "STA") { // レジスタのデータをメモリにストア
      write(data, r_A);
    } else if(opeName == "STX") {
      write(data, r_X);
    } else if(opeName == "STY") {
      write(data, r_Y);
    } else if(opeName == "TAX") { // レジスタの値を別レジスタにコピー
      r_X = r_A;
      setRegisters(r_X);
    } else if(opeName == "TAY") {
      r_Y = r_A;
      setRegisters(r_Y);
    } else if(opeName == "TSX") {
      r_X = r_SP;
      setRegisters(r_X);
    } else if(opeName == "TXA") {
      r_A = r_X;
      setRegisters(r_X);
    } else if(opeName == "TXS") {
      r_SP = r_X;
      setRegisters(r_SP);
    } else if(opeName == "TYA") {
      r_A = r_Y;
      setRegisters(r_A);
    } else if(opeName == "ADC") { // 演算命令いろいろ
      int calc;
      if(mode == "immediate") {
        calc = data + r_A + r_status["carry"];
      } else {
        calc = read(data) + r_A + r_status["carry"];
      }
      r_status["carry"] = !!(calc > 0xff);
      setRegisters(calc);
      r_A = calc & 0xff;
    } else if(opeName == "AND") {
      int calc;
      if(mode == "immediate") {
        calc = data & r_A;
      } else {
        calc = read(data) & r_A;
      }
      setRegisters(calc);
      r_A = calc & 0xff;
    } else if(opeName == "ASL") {
      if(data == -1) {
        r_status["carry"] = !!(r_A & (1 << 7));
        r_A = (r_A << 1) & 0xff; // Aレジスタを左シフト(8bit以上は切り捨て)
                                 // 7bit目をcarryフラグに代入
        setRegisters(r_A);
      } else {
        r_status["carry"] = !!(read(data) & (1 << 7));
        write(
            data,
            (read(data) << 1) &
                0xff); // 指定されたアドレスのbitを左シフト(8bit以上は切り捨て)
        setRegisters(read(data));
      }
    } else if(opeName == "BIT") {
      r_status["zero"] = !!(
          r_A &
          read(
              data)); // Aレジスタとメモリデータのand演算の結果をZレジスタに格納
      r_status["overflow"] =
          !!(read(data) & (1 << 6)); // メモリデータの6bit目をVレジスタに格納
      r_status["negative"] =
          !!(read(data) & (1 << 7)); // メモリデータの7bit目をNレジスタに格納
    } else if(opeName == "CMP") {
      data = (mode == "immediate") ? data : read(data);
      int diff = r_A - data;
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } else if(opeName == "CPX") {
      data = (mode == "immediate") ? data : read(data);
      int diff = r_X - data;
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } else if(opeName == "CPY") {
      data = (mode == "immediate") ? data : read(data);
      int diff = r_Y - data;
      r_status["carry"] = !!(diff >= 0);
      setRegisters(diff);
    } else if(opeName == "DEC") {
      write(data, (read(data) + 0xff) & 0xff);
      setRegisters(read(data));
    } else if(opeName == "DEX") {
      r_X = (r_X + 0xff) & 0xff;
      setRegisters(r_X);
      hex(r_X);
    } else if(opeName == "DEY") {
      r_Y = (r_Y + 0xff) & 0xff;
      setRegisters(r_Y);
      hex(r_Y);
    } else if(opeName == "EOR") {
      data = (mode == "immediate" ? data : read(data));
      r_A = r_A ^ data;
      setRegisters(r_A);
    } else if(opeName == "INC") {
      write(data, (read(data) + 1) & 0xff);
      setRegisters(read(data));
    } else if(opeName == "INX") {
      r_X = (r_X + 1) & 0xff;
      setRegisters(r_X);
    } else if(opeName == "INY") {
      r_Y = (r_Y + 1) & 0xff;
      hex(r_Y);
      setRegisters(r_Y);
    } else if(opeName == "LSR") {
      if(data == -1) {
        r_status["carry"] = (r_A & 0b1);
        r_A = (r_A >> 1) & 0xff;
        setRegisters(r_A);
      } else {
        r_status["carry"] = (read(data) & 0b1);
        write(data, (read(data) >> 1) & 0xff);
        setRegisters(read(data));
      }
    } else if(opeName == "ORA") {
      data = (mode == "immediate") ? data : read(data);
      r_A = r_A | data;
      setRegisters(r_A);
    } else if(opeName == "ROL") {
      if(data == -1) {
        int tmp = r_A;
        r_A = (r_A << 1) | r_status["carry"];
        r_status["carry"] = !!(tmp & 1 << 7);
        setRegisters(r_A);
        r_A = r_A & 0xff;
      } else {
        int tmp = read(data);
        write(data, read(data) << 1 | r_status["carry"]);
        r_status["carry"] = !!(tmp & 1 << 7);
        setRegisters(read(data));
        write(data, read(data) & 0xff);
      }
    } else if(opeName == "ROR") {
      if(data == -1) {
        int tmp = r_A;
        r_A = ((r_A >> 1) | (r_status["carry"] << 7)) & 0xff;
        r_status["carry"] = !!(tmp & 0b1);
        setRegisters(r_A);
      } else {
        int tmp = read(data);
        write(data, (read(data) >> 1 | (r_status["carry"] << 7)) & 0xff);
        r_status["carry"] = !!(tmp & 0b1);
        setRegisters(read(data));
      }
    } else if(opeName == "SBC") {
      data = (mode == "immediate") ? data : read(data);
      r_A = r_A + (~data + 1) + (0x100 - !r_status["carry"]);
      r_status["carry"] = !!(r_A > 0xff);
      setRegisters(r_A);
      r_A = r_A & 0xff;
    } else if(opeName == "PHA") {
      push_stack(r_A);
    } else if(opeName == "PHP") {
      push_status_registers();
    } else if(opeName == "PLA") {
      r_A = pop_stack();
    } else if(opeName == "PLP") {
      pop_status_registers();
    } else if(opeName == "JMP") {
      r_PC = data;
    } else if(opeName == "JSR") {
      int lowByte = (r_PC - 1) & 0xff;
      int highByte = ((r_PC - 1) & 0xff00) >> 8;
      push_stack(highByte);
      push_stack(lowByte);
      r_PC = data;
    } else if(opeName == "RTS") {
      int lowByte = pop_stack();
      int highByte = pop_stack() << 8;
      r_PC = highByte + lowByte + 1;
    } else if(opeName == "RTI") {
      // 割り込みから復帰
      pop_status_registers();
      int pc_lowByte = pop_stack();
      int pc_highByte = pop_stack();
      r_PC = (pc_highByte << 8) | pc_lowByte;
    } else if(opeName == "BCC") {
      if(!r_status["carry"]) {
        r_PC = data;
      }
    } else if(opeName == "BCS") {
      if(r_status["carry"]) {
        r_PC = data;
      }
    } else if(opeName == "BEQ") {
      if(r_status["zero"]) {
        r_PC = data;
      }
    } else if(opeName == "BMI") {
      if(r_status["negative"]) {
        r_PC = data;
      }
    } else if(opeName == "BNE") {
      if(!r_status["zero"]) {
        r_PC = data;
      }
    } else if(opeName == "BPL") {
      if(!r_status["negative"]) {
        r_PC = data;
      }
    } else if(opeName == "BVC") {
      if(!r_status["overflow"]) {
        r_PC = data;
      }
    } else if(opeName == "BVS") {
      if(r_status["overflow"]) {
        r_PC = data;
      }
    } else if(opeName == "CLC") {
      r_status["carry"] = false;
    } else if(opeName == "CLD") {
      r_status["decimal"] = false;
    } else if(opeName == "CLI") {
      r_status["interrupt"] = false;
    } else if(opeName == "CLV") {
      r_status["overflow"] = false;
    } else if(opeName == "SEC") {
      r_status["carry"] = true;
    } else if(opeName == "SED") {
      r_status["decimal"] = true;
    } else if(opeName == "SEI") {
      r_status["interrupt"] = true;
    }
    // 非公式
    else if(opeName == "NOPD") {
      r_PC++;
    }
  }
} // namespace NES