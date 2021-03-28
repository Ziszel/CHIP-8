#include "../include/cpu.hpp"

Cpu::Cpu()
{
    // initialises the CPU and memory.
    I = 0;
    PC = 0x200;
    memset(registers, 0, sizeof(registers));
    memset(memory, 0, sizeof(memory));
    DT = 0;
    ST = 0;

    LoadFontDataToMemory();

}

void Cpu::LoadROM()
{
    // Load in the game
    FILE *file;
    file = fopen("roms/INVADERS", "r");
    // where to read to, the size of the array, the size of each object, the input stream.
    // the documentation for fread did not match up with what worked for me.
    if (file == 0)
    {
        std::cout << "no ROM found." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    fread(&memory[0x200], 0xfff, 1, file);
    fclose(file);
}

void Cpu::LoadFontDataToMemory()
{
    for (int i = 0; i < fontData.size(); i++)
    {
        memory[i] = fontData.at(i);
    }
}