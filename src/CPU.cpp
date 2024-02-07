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
using namespace std;

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
  int pc_lowByte = read(0xFFFC);
  int pc_highByte = read(0xFFFD);
  registers["PC"] = (pc_highByte << 8) | pc_lowByte;
  push_status_registers();
  registers["interrupt"] = true;
  // cout << registers["PC"] << endl;
}

// Non-Markable-Interrupt
void CPU::NMI() {
  int pc_lowByte = read(0xFFFA);
  int pc_highByte = read(0xFFFB);
  registers["PC"] = (pc_highByte << 8) | pc_lowByte;
  push_status_registers();
  registers["interrupt"] = true;
  // cout << registers["PC"] << endl;
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
  // cout << "read(" << hex << setw(4) << setfill('0') << addr << ")=" << hex << setw(4) << setfill('0') << res << " ";
  return res;
}

// メモリ書き込み
void CPU::write(Address addr, Byte data) {
  // cout << "write(" << hex << setw(4) << setfill('0') << addr << "," << hex << setw(4) << setfill('0') << num << ") ";

  // WRAMのミラー領域に対する書き込み
  if(addr >= 0x0800 && addr < 0x2000) {
    mem[addr % 0x0800] = data;
  }
  // PPUレジスタに対する書き込み
  else if(addr >= 0x2000 && addr < 0x4000) {
    addr = 0x2000 + (addr - 0x2000) % 8;
    if(addr == 0x2004) {
      // スプライトメモリアドレス(0x2003)で指定されたアドレスへデータを書き込む。 書き込む度にスプライトメモリアドレスはインクリメントされる
      writeVRAM(mem[0x2003], data);
      mem[0x2003]++;
    } else if(addr == 0x2004) {
      writeSpriteRAM(mem[0x2003], data);
      mem[0x2003]++;
    } else if(addr == 0x2006) {
      // 書き込み回数が偶数回ならアドレスの上位8bit,奇数なら下位8bitを書き込む
      if(ppuaddrCnt == 0) {
        ppu_write_addr &= 0x00ff;
        ppu_write_addr |= static_cast<Address>(data) << 8;
        mem[0x2006] = data;
      } else {
        ppu_write_addr &= 0xff00;
        ppu_write_addr |= data;
        mem[0x2006] = data;
      }
      ppuaddrCnt ^= 1;
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
  Byte res = read(registers["PC"]++);
  return res;
}

// スタックに値をプッシュ
void CPU::push_stack(Byte data) {
  mem[STACK_START + registers["S"]] = data;
  registers["S"]--;
}

// スタックからpop
Byte CPU::pop_stack() {
  Byte res = mem[STACK_START + registers["S"] + 1];
  registers["S"]++;
  // cout << "pop ";
  hex(res);
  return res;
}

// ステータスレジスタをスタックに退避
void CPU::push_status_registers() {
  push_stack(registers["negative"]);
  push_stack(registers["overflow"]);
  push_stack(registers["reserved"]);
  push_stack(registers["break"]);
  push_stack(registers["decimal"]);
  push_stack(registers["interrupt"]);
  push_stack(registers["zero"]);
  push_stack(registers["carry"]);
}

// スタックからステータスレジスタを復帰
void CPU::pop_status_registers() {
  registers["carry"] = pop_stack();
  registers["zero"] = pop_stack();
  registers["interrupt"] = pop_stack();
  registers["decimal"] = pop_stack();
  registers["break"] = pop_stack();
  registers["reserved"] = pop_stack();
  registers["overflow"] = pop_stack();
  registers["negative"] = pop_stack();
}

// 実行
void CPU::run() {
  // cout << hex << setw(4) << setfill('0') << count++ << " ";
  test_option();
  Byte code = fetch();
  map<string, string> ope = opelist[code];
  string opeName = ope["baseName"];
  string addressing = ope["mode"];
  // cout << opeName << " ";
  ope_appeared.insert(opeName);
  Address data = fetchOperand(addressing);
  exec(opeName, data, addressing);
  // cout << "\n";
}

void CPU::print_appeared_opelist() {
  // cout << "---------------------Appeared Opelations----------------------------" << endl;
  for(auto itr = ope_appeared.begin(); itr != ope_appeared.end(); ++itr) {
    // cout << *itr << endl;
  }
  // cout << "--------------------------------------------------------------------" << endl;
};

void CPU::readROM() {
  // string filename = "./rom/sample1.dat";
  // string filename = "./rom/roulette.nes";
  string filename = "./rom/NEStress.NES";
  // string filename = "./rom/masmix.nes";
  // string filename = "./rom/TK20NTSC.NES";
  // string filename = "./rom/hello.nes";
  // string filename = "./rom/firedemo.nes";
  ifstream ifs(filename, ios::in | ios::binary);
  if(!ifs) {
    // cout << "ファイルが開けません";
    exit(1);
  }
  // cout << filename << endl;
  ifs.seekg(0, ios::end);
  size_t size = ifs.tellg();
  ifs.seekg(0);
  unsigned char *data = new unsigned char[size];
  ifs.read((char *)data, size);

  // iNESヘッダーからプログラムROMとキャラクターROMの範囲を割り出す(バイト単位)
  int CharacterRomStart = 0x0010 + (data[4] * 0x4000); // ヘッダー16バイト+プログラムデータのページ数*ページ数当たりのバイト数
  int CharacterRomEnd = CharacterRomStart + (data[5] * 0x2000);
  CharacterRom.assign(data[5] * 0x2000, vector<Byte>(64));

  // プログラムをメモリにセット
  for(int i = 0; i < CharacterRomStart - 0x10; i++) {
    mem[i + 0x8000] = data[i + 0x10];
  }

  // キャラクターROMのデータをセット
  for(int i = 0; i * 16 < data[5] * 0x2000; i++) { // 1スプライトごとに長さ64の配列にする
    for(int j = 0; j < 8; j++) {
      int first = data[CharacterRomStart + i * 16 + j];
      int second = data[CharacterRomStart + i * 16 + 8 + j];
      for(int h = 0; h < 8; h++) {
        CharacterRom[i][j * 8 + h] = !!(first & 1 << (7 - h)) + !!(second & 1 << (7 - h)) * 2;
      }
    }
  }
}

// レジスタをセット
void CPU::setRegisters(Byte num) {
  registers["negative"] = !!(num & (1 << 7));
  registers["zero"] = (num == 0);
}

// アドレッシングモードを判別し、データを取得
Address CPU::fetchOperand(string addr) {
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
    return add + registers["X"];
  } else if(addr == "zeroPageY") {
    Address add = fetch();
    return add + registers["Y"];
  } else if(addr == "absolute") {
    Address add1 = fetch();
    Address add2 = static_cast<Address>(fetch()) << 8;
    return add1 + add2;
  } else if(addr == "absoluteX") {
    Address add1 = fetch();
    Address add2 = static_cast<Address>(fetch()) << 8;
    return add1 + add2 + registers["X"];
  } else if(addr == "absoluteY") {
    Address add1 = fetch();
    Address add2 = static_cast<Address>(fetch()) << 8;
    return add1 + add2 + registers["Y"];
  } else if(addr == "preIndexedIndirect") {
    Address addradd = fetch() + registers["X"];
    Address addr1 = mem[addradd];
    Address addr2 = static_cast<Address>(mem[addradd + 1]) << 8;
    return addr1 + addr2;
  } else if(addr == "postIndexedIndirect") {
    Address addradd = fetch();
    Address addr1 = mem[addradd];
    Address addr2 = static_cast<Address>(mem[addradd + 1]) << 8;
    Address offset = registers["Y"];
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
    return addr + registers["PC"];
  }
  return -1;
}

// 命令コード、データから命令を実行
void CPU::exec(string opeName, Address data, string mode) {
  if(opeName == "LDA") { // データをレジスタにロード
    if(mode == "immediate") {
      registers["A"] = data;
    } else {
      registers["A"] = read(data);
    }
    setRegisters(registers["A"]);
  } else if(opeName == "LDX") {
    if(mode == "immediate") {
      registers["X"] = data;
    } else {
      registers["X"] = read(data);
    }
    setRegisters(registers["X"]);
  } else if(opeName == "LDY") {
    if(mode == "immediate") {
      registers["Y"] = data;
    } else {
      registers["Y"] = read(data);
    }
    setRegisters(registers["Y"]);
  } else if(opeName == "STA") { // レジスタのデータをメモリにストア
    write(data, registers["A"]);
  } else if(opeName == "STX") {
    write(data, registers["X"]);
  } else if(opeName == "STY") {
    write(data, registers["Y"]);
  } else if(opeName == "TAX") { // レジスタの値を別レジスタにコピー
    registers["X"] = registers["A"];
    setRegisters(registers["X"]);
  } else if(opeName == "TAY") {
    registers["Y"] = registers["A"];
    setRegisters(registers["Y"]);
  } else if(opeName == "TSX") {
    registers["X"] = registers["S"];
    setRegisters(registers["X"]);
  } else if(opeName == "TXA") {
    registers["A"] = registers["X"];
    setRegisters(registers["X"]);
  } else if(opeName == "TXS") {
    registers["S"] = registers["X"];
    setRegisters(registers["S"]);
  } else if(opeName == "TYA") {
    registers["A"] = registers["Y"];
    setRegisters(registers["A"]);
  } else if(opeName == "ADC") { // 演算命令いろいろ
    int calc;
    if(mode == "immediate") {
      calc = data + registers["A"] + registers["carry"];
    } else {
      calc = read(data) + registers["A"] + registers["carry"];
    }
    registers["carry"] = !!(calc > 0xff);
    setRegisters(calc);
    registers["A"] = calc & 0xff;
  } else if(opeName == "AND") {
    int calc;
    if(mode == "immediate") {
      calc = data & registers["A"];
    } else {
      calc = read(data) & registers["A"];
    }
    setRegisters(calc);
    registers["A"] = calc & 0xff;
  } else if(opeName == "ASL") {
    if(data == -1) {
      registers["carry"] = !!(registers["A"] & (1 << 7));
      registers["A"] = (registers["A"] << 1) & 0xff; // Aレジスタを左シフト(8bit以上は切り捨て) 7bit目をcarryフラグに代入
      setRegisters(registers["A"]);
    } else {
      registers["carry"] = !!(read(data) & (1 << 7));
      write(data, (read(data) << 1) & 0xff); // 指定されたアドレスのbitを左シフト(8bit以上は切り捨て)
      setRegisters(read(data));
    }
  } else if(opeName == "BIT") {
    registers["zero"] = !!(registers["A"] & read(data)); // Aレジスタとメモリデータのand演算の結果をZレジスタに格納
    registers["overflow"] = !!(read(data) & (1 << 6));   // メモリデータの6bit目をVレジスタに格納
    registers["negative"] = !!(read(data) & (1 << 7));   // メモリデータの7bit目をNレジスタに格納
  } else if(opeName == "CMP") {
    data = (mode == "immediate") ? data : read(data);
    int diff = registers["A"] - data;
    registers["carry"] = !!(diff >= 0);
    setRegisters(diff);
  } else if(opeName == "CPX") {
    data = (mode == "immediate") ? data : read(data);
    int diff = registers["X"] - data;
    registers["carry"] = !!(diff >= 0);
    setRegisters(diff);
  } else if(opeName == "CPY") {
    data = (mode == "immediate") ? data : read(data);
    int diff = registers["Y"] - data;
    registers["carry"] = !!(diff >= 0);
    setRegisters(diff);
  } else if(opeName == "DEC") {
    write(data, (read(data) + 0xff) & 0xff);
    setRegisters(read(data));
  } else if(opeName == "DEX") {
    registers["X"] = (registers["X"] + 0xff) & 0xff;
    setRegisters(registers["X"]);
    hex(registers["X"]);
  } else if(opeName == "DEY") {
    registers["Y"] = (registers["Y"] + 0xff) & 0xff;
    setRegisters(registers["Y"]);
    hex(registers["Y"]);
  } else if(opeName == "EOR") {
    data = (mode == "immediate" ? data : read(data));
    registers["A"] = registers["A"] ^ data;
    setRegisters(registers["A"]);
  } else if(opeName == "INC") {
    write(data, (read(data) + 1) & 0xff);
    setRegisters(read(data));
  } else if(opeName == "INX") {
    registers["X"] = (registers["X"] + 1) & 0xff;
    setRegisters(registers["X"]);
  } else if(opeName == "INY") {
    registers["Y"] = (registers["Y"] + 1) & 0xff;
    hex(registers["Y"]);
    setRegisters(registers["Y"]);
  } else if(opeName == "LSR") {
    if(data == -1) {
      registers["carry"] = (registers["A"] & 0b1);
      registers["A"] = (registers["A"] >> 1) & 0xff;
      setRegisters(registers["A"]);
    } else {
      registers["carry"] = (read(data) & 0b1);
      write(data, (read(data) >> 1) & 0xff);
      setRegisters(read(data));
    }
  } else if(opeName == "ORA") {
    data = (mode == "immediate") ? data : read(data);
    registers["A"] = registers["A"] | data;
    setRegisters(registers["A"]);
  } else if(opeName == "ROL") {
    if(data == -1) {
      int tmp = registers["A"];
      registers["A"] = (registers["A"] << 1) | registers["carry"];
      registers["carry"] = !!(tmp & 1 << 7);
      setRegisters(registers["A"]);
      registers["A"] = registers["A"] & 0xff;
    } else {
      int tmp = read(data);
      write(data, read(data) << 1 | registers["carry"]);
      registers["carry"] = !!(tmp & 1 << 7);
      setRegisters(read(data));
      write(data, read(data) & 0xff);
    }
  } else if(opeName == "ROR") {
    if(data == -1) {
      int tmp = registers["A"];
      registers["A"] = ((registers["A"] >> 1) | (registers["carry"] << 7)) & 0xff;
      registers["carry"] = !!(tmp & 0b1);
      setRegisters(registers["A"]);
    } else {
      int tmp = read(data);
      write(data, (read(data) >> 1 | (registers["carry"] << 7)) & 0xff);
      registers["carry"] = !!(tmp & 0b1);
      setRegisters(read(data));
    }
  } else if(opeName == "SBC") {
    data = (mode == "immediate") ? data : read(data);
    registers["A"] = registers["A"] + (~data + 1) + (0x100 - !registers["carry"]);
    registers["carry"] = !!(registers["A"] > 0xff);
    setRegisters(registers["A"]);
    registers["A"] = registers["A"] & 0xff;
  } else if(opeName == "PHA") {
    push_stack(registers["A"]);
  } else if(opeName == "PHP") {
    push_stack(registers["P"]);
  } else if(opeName == "PLA") {
    registers["A"] = pop_stack();
  } else if(opeName == "PLP") {
    registers["P"] = pop_stack();
  } else if(opeName == "JMP") {
    registers["PC"] = data;
  } else if(opeName == "JSR") {
    int lowByte = (registers["PC"] - 1) & 0xff;
    int highByte = ((registers["PC"] - 1) & 0xff00) >> 8;
    push_stack(highByte);
    push_stack(lowByte);
    registers["PC"] = data;
  } else if(opeName == "RTS") {
    int lowByte = pop_stack();
    int highByte = pop_stack() << 8;
    registers["PC"] = highByte + lowByte + 1;
  } else if(opeName == "RTI") {
    // 割り込みから復帰
    pop_status_registers();
    int pc_lowByte = pop_stack();
    int pc_highByte = pop_stack();
    registers["PC"] = (pc_highByte << 8) | pc_lowByte;
  } else if(opeName == "BCC") {
    if(!registers["carry"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BCS") {
    if(registers["carry"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BEQ") {
    if(registers["zero"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BMI") {
    if(registers["negative"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BNE") {
    if(!registers["zero"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BPL") {
    if(!registers["negative"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BVC") {
    if(!registers["overflow"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "BVS") {
    if(registers["overflow"]) {
      registers["PC"] = data;
    }
  } else if(opeName == "CLC") {
    registers["carry"] = false;
  } else if(opeName == "CLD") {
    registers["decimal"] = false;
  } else if(opeName == "CLI") {
    registers["interrupt"] = false;
  } else if(opeName == "CLV") {
    registers["overflow"] = false;
  } else if(opeName == "SEC") {
    registers["carry"] = true;
  } else if(opeName == "SED") {
    registers["decimal"] = true;
  } else if(opeName == "SEI") {
    registers["interrupt"] = true;
  }
  // 非公式
  else if(opeName == "NOPD") {
    registers["PC"]++;
  }
}