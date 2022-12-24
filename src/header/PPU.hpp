class CPU;

class PPU{
    public:

    //VRAM
    int mem[0x4000];

    //CPUクラスへのポインタ
    CPU* cpu;

    //VRAMを読み込む
    int readRam(int addr);

    //VRAMに書き込む
    void writeRam(int addr, int num);

    void connectCPU(CPU* Cpu);
};