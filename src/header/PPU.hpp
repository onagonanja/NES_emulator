class CPU;

class PPU{
    public:

    //VRAM
    int vram[0x4000];

    //VRAMを読み込む
    int readRam(int addr);

    //VRAMに書き込む
    void writeRam(int addr, int num);
};