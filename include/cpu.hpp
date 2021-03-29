#include <iostream>
#include <string.h>
#include <vector>
#include <array>
#pragma once

typedef uint8_t BYTE;
typedef uint16_t WORD;

class Cpu

{
public:

    // 8bit registers
    BYTE registers[16];
    // timers
    BYTE DT;
    BYTE ST;
    // 16bit register
    WORD I;
    WORD PC;
    std::vector<WORD> stack;
    // RAM
    BYTE memory[0xFFF];
    BYTE screenData[64][32];
    // fontData
    const std::array<BYTE, 80> fontData = {
        /* 0 */ 0xF0,0x90,0x90,0x90,0xF0,
        /* 1 */ 0x20,0x60,0x20,0x20,0x70,
        /* 2 */ 0xF0,0x10,0xF0,0x80,0xF0,
        /* 3 */ 0xF0,0x10,0xF0,0x10,0xF0,
        /* 4 */ 0x90,0x90,0xF0,0x10,0x10,
        /* 5 */ 0xF0,0x80,0xF0,0x10,0xF0,
        /* 6 */ 0xF0,0x80,0xF0,0x90,0xF0,
        /* 7 */ 0xF0,0x10,0x20,0x40,0x40,
        /* 8 */ 0xF0,0x90,0xF0,0x90,0xF0,
        /* 9 */ 0xF0,0x90,0xF0,0x10,0xF0,
        /* A */ 0xF0,0x90,0xF0,0x90,0x90,
        /* B */ 0xE0,0x90,0xE0,0x90,0xE0,
        /* C */ 0xF0,0x80,0x80,0x80,0xF0,
        /* D */ 0xE0,0x90,0x90,0x90,0xE0,
        /* E */ 0xF0,0x80,0xF0,0x80,0xF0,
        /* F */ 0xF0,0x80,0xF0,0x80,0x80 };

    Cpu();
    void LoadROM();
    void LoadFontDataToMemory();

};