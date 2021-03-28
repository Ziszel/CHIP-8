#include "../include/cpu.hpp"
#include <limits>
#include <stdlib.h>
#include <raylib.h>
#include <array>
#pragma once

typedef uint8_t BYTE;
typedef uint16_t WORD;

WORD getNextOpcode(Cpu &cpu);

int CurrentKeyPressed(std::array<BYTE, 16> &keys);

// List of functions which handle each opcode.
void opcode00E0(WORD opcode, Cpu &cpu);

void opcode00EE(WORD opcode, Cpu &cpu);

void opcode1NNN(WORD opcode, Cpu &cpu);

void opcode2NNN(WORD opcode, Cpu &cpu);

void opcode3XNN(WORD opcode, Cpu &cpu);

void opcode4XNN(WORD opcode, Cpu &cpu);

void opcode5XY0(WORD opcode, Cpu &cpu);

void opcode6XNN(WORD opcode, Cpu &cpu);

void opcode7XNN(WORD opcode, Cpu &cpu);

void opcode8XY0(WORD opcode, Cpu &cpu);

void opcode8XY1(WORD opcode, Cpu &cpu);

void opcode8XY2(WORD opcode, Cpu &cpu);

void opcode8XY3(WORD opcode, Cpu &cpu);

void opcode8XY4(WORD opcode, Cpu &cpu);

void opcode8XY5(WORD opcode, Cpu &cpu);

void opcode8XY6(WORD opcode, Cpu &cpu);

void opcode8XY7(WORD opcode, Cpu &cpu);

void opcode8XYE(WORD opcode, Cpu &cpu);

void opcode9XY0(WORD opcode, Cpu &cpu);

void opcodeANNN(WORD opcode, Cpu &cpu);

void opcodeBNNN(WORD opcode, Cpu &cpu);

void opcodeCXNN(WORD opcode, Cpu &cpu);

void opcodeDXYN(WORD opcode, Cpu &cpu);

void opcodeEX9E(WORD opcode, Cpu &cpu, std::array<BYTE, 16> &keys);

void opcodeEXA1(WORD opcode, Cpu &cpu, std::array<BYTE, 16> &keys);

void opcodeFX07(WORD opcode, Cpu &cpu);

void opcodeFX0A(WORD opcode, Cpu &cpu, std::array<BYTE, 16> &keys);

void opcodeFX15(WORD opcode, Cpu &cpu);

void opcodeFX18(WORD opcode, Cpu &cpu);

void opcodeFX1E(WORD opcode, Cpu &cpu);

void opcodeFX29(WORD opcode, Cpu &cpu);

void opcodeFX33(WORD opcode, Cpu &cpu);

void opcodeFX55(WORD opcode, Cpu &cpu);

void opcodeFX65(WORD opcode, Cpu &cpu);

void decodeOpcode(Cpu &cpu, std::array<BYTE, 16> &keys);