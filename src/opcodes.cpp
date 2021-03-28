#include "../include/opcodes.hpp"

WORD getNextOpcode(Cpu &cpu)
{
    WORD res = 0;
    res = (cpu.memory[cpu.PC] << 8 | cpu.memory[cpu.PC + 1]);
    cpu.PC += 2;
    return res;
}

int CurrentKeyPressed(std::array<BYTE, 16> &keys)
{
    int result = -1;

    for (int i = 0; i < 16; i++)
    {
        if (keys[i] > 0)
            return i;
    }
    return result;
}

void opcode00E0(WORD opcode, Cpu &cpu)
{
    // clear the screen by setting all bits in screendata to 0
    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            cpu.screenData[x][y] = 0;
        }
    }
}

void opcode00EE(WORD opcode, Cpu &cpu)
{
    // returns PC from a subroutine
    // cleans the stack
    cpu.PC = cpu.stack.back();
    cpu.stack.pop_back();
}

void opcode1NNN(WORD opcode, Cpu &cpu)
{
    // NNN is actually the address, by masking 0x0FFF we can get only the address
    // out of the opcode and set the PC equal to it.
    cpu.PC = (opcode & 0x0FFF);
}

void opcode2NNN(WORD opcode, Cpu &cpu)
{
    cpu.stack.push_back(cpu.PC);
    cpu.PC = (opcode & 0x0FFF);
}

void opcode3XNN(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    // Each hex digit can be represented as 4 binary bits.
    // shifting to the right by 8 will give 0x0X(the number in 0x0F00 of opcode);
    regX >>= 8;

    if (cpu.registers[regX] == (opcode & 0x00FF))
    {
        cpu.PC += 2;
    }
}

void opcode4XNN(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    if (cpu.registers[regX] != (opcode & 0x00FF))
    {
        cpu.PC += 2;
    }
}

void opcode5XY0(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    if (cpu.registers[regX] == cpu.registers[regY])
    {
        cpu.PC += 2;
    }
}

void opcode6XNN(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    cpu.registers[regX] = (opcode & 0x00FF);
}

void opcode7XNN(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    cpu.registers[regX] += (opcode & 0x00FF);
}

void opcode8XY0(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    cpu.registers[regX] = cpu.registers[regY];
}

void opcode8XY1(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    cpu.registers[regX] |= cpu.registers[regY];
}

void opcode8XY2(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    cpu.registers[regX] &= cpu.registers[regY];
}

void opcode8XY3(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    cpu.registers[regX] ^= cpu.registers[regY];
}

void opcode8XY4(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    // VF set to 1 if there's a carry, 0 if there is not.
    // checks if value of regx+regy is greater than 255 (max number of bits in a byte)
    // if so, it must have a carry and thus VF is set.
    int value = cpu.registers[regX] + cpu.registers[regY];

    if (value > 0xFF)
    {
        cpu.registers[0xF] = 1;
    }
    else
    {
        cpu.registers[0xF] = 0;
    }

    cpu.registers[regX] += cpu.registers[regY];
}

void opcode8XY5(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    if (cpu.registers[regX] < cpu.registers[regY])
    {
        cpu.registers[0xF] = 0;
    }
    else
    {
        cpu.registers[0xF] = 1;
    }

    cpu.registers[regX] -= cpu.registers[regY];
}

void opcode8XY6(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00) >> 8;
    // (regX & 0x1) gets the LSB of regX and stores as the carry in VF.
    cpu.registers[0xF] = cpu.registers[regX] & 0x1;
    // Shift regX right by one. (the LSB is lost from regX but stored in VF as the carry)
    cpu.registers[regX] >>= 1;
}

// Sets VX to VY minus VX. VF is set to 0 when there's a borrow,
// and 1 when there isn't.
void opcode8XY7(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    if (cpu.registers[regY] < cpu.registers[regX])
    {
        cpu.registers[0xF] = 0;
    }
    else
    {
        cpu.registers[0xF] = 1;
    }

    cpu.registers[regX] = cpu.registers[regY] - cpu.registers[regX];
}

void opcode8XYE(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00) >> 8;
    cpu.registers[0xF] = cpu.registers[regX] & 0x80;
    cpu.registers[regX] <<= 1;
}

void opcode9XY0(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int regY = (opcode & 0x00F0);
    regY >>= 4;

    if (cpu.registers[regX] != cpu.registers[regY])
    {
        cpu.PC += 2;
    }
}

void opcodeANNN(WORD opcode, Cpu &cpu)
{
    cpu.I = (opcode & 0x0FFF);
}

void opcodeBNNN(WORD opcode, Cpu &cpu)
{
    cpu.PC = cpu.registers[0] + (opcode & 0x0FFF);
}

void opcodeCXNN(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int value = (opcode & 0x00FF);

    cpu.registers[regX] = (rand() & value);
}

void opcodeDXYN(WORD opcode, Cpu &cpu)
{
    // Draws a sprite at coordinate (VX, VY)
    // that has a width of 8 pixels and a height of N+1 pixels.
    int regX = (opcode & 0x0F00) >> 8;
    int regY = (opcode & 0x00F0) >> 4;

    // n = height
    int n = (opcode & 0x000F);
    int width = 8;

    cpu.registers[0xF] = 0;

    for (int row = 0; row < n; row++)
    {
        // Get a row one of sprite data from the memory address in reg I (one byte per row)
        BYTE spriteData = (cpu.memory[cpu.I + row]);

        // for each 8 pixels/bits in this sprite row
        for (int col = 0; col < width; col++)
        {
            // if the current pixel in the sprite row is on,
            // and the screen pixel at X,Y is on, set pixel to 0, set VF to 1
            if ((spriteData & 0x80) > 0)
            {
                int x = cpu.registers[regX] + col;
                int y = cpu.registers[regY] + row;

                if ((x >= 0 && x < 64) && (y >= 0 && y < 32))
                {
                    if (cpu.screenData[x][y] == 1)
                    {
                        cpu.registers[0xF] = 1;
                    }
                    cpu.screenData[x][y] ^= 1;
                }
            }
            // Point spriteData to the next bit
            spriteData <<= 1;
        }
    }
}

void opcodeEX9E(WORD opcode, Cpu &cpu, std::array<BYTE, 16> &keys)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int key = cpu.registers[regX];

    if (keys[key] == 1)
    {
        cpu.PC += 2;
    }
}

void opcodeEXA1(WORD opcode, Cpu &cpu, std::array<BYTE, 16> &keys)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int key = cpu.registers[regX];

    if (keys[key] == 0)
    {
        cpu.PC += 2;
    }
}

void opcodeFX07(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    cpu.registers[regX] = cpu.DT;
}

// Await keypress, then store in VX ()
void opcodeFX0A(WORD opcode, Cpu &cpu, std::array<BYTE, 16> &keys)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    int keyPress = CurrentKeyPressed(keys);

    // From Wiki: All instruction halted until next key event.
    // PC-=2 allows the CPU to continue without locking to this opcode
    if (keyPress == -1)
    {
        cpu.PC -= 2;
    }
    else
    {
        cpu.registers[regX] = keyPress;
    }
}

void opcodeFX15(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    cpu.DT = cpu.registers[regX];
}

void opcodeFX18(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    cpu.ST = cpu.registers[regX];
}

void opcodeFX1E(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    cpu.I += cpu.registers[regX];
}

void opcodeFX29(WORD opcode, Cpu &cpu)
{

    // sets I to the location of the sprite for the character in VX
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    // TODO: research this
    cpu.I = cpu.registers[regX] * 5;
}

void opcodeFX33(WORD opcode, Cpu &cpu)
{
    // Stores the binary-coded decimal representation of VX,
    // with the most significant of three digits at the address in I,
    // the middle digit at I plus 1, and the least significant digit at I plus 2
    int regX = (opcode & 0x0F00);
    regX >>= 8;
    int value = cpu.registers[regX];

    int hundreds = value / 100;
    int tens = (value / 10) % 10;
    int digits = value % 10;

    cpu.memory[cpu.I] = hundreds;
    cpu.memory[cpu.I + 1] = tens;
    cpu.memory[cpu.I + 2] = digits;
}

void opcodeFX55(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    // Stores V0 to VX (including VX) in memory starting at address I.
    // The offset from I is increased by 1 for each value written,
    // but I itself is left unmodified.
    for (int i = 0; i <= regX; i++)
    {
        cpu.memory[cpu.I + i] = cpu.registers[i];
    }
}

void opcodeFX65(WORD opcode, Cpu &cpu)
{
    int regX = (opcode & 0x0F00);
    regX >>= 8;

    //Fills V0 to VX (including VX) with values from memory starting at
    // address I.
    // The offset from I is increased by 1 for each value written,
    // but I itself is left unmodified.
    for (int i = 0; i <= regX; i++)
    {
        cpu.registers[i] = cpu.memory[cpu.I + i];
    }
}

void decodeOpcode(Cpu &cpu, std::array<BYTE, 16> &keys)
{
    WORD opcode = getNextOpcode(cpu);

    switch (opcode & 0xF000)
    {
    case 0x0000:
    {
        switch (opcode & 0xF)
        {
        case 0x0:
            opcode00E0(opcode, cpu);
            break;
        case 0xE:
            opcode00EE(opcode, cpu);
            break;
        }
        break;
    }
    case 0x1000:
        opcode1NNN(opcode, cpu);
        break;
    case 0x2000:
        opcode2NNN(opcode, cpu);
        break;
    case 0x3000:
        opcode3XNN(opcode, cpu);
        break;
    case 0x4000:
        opcode4XNN(opcode, cpu);
        break;
    case 0x5000:
        opcode5XY0(opcode, cpu);
        break;
    case 0x6000:
        opcode6XNN(opcode, cpu);
        break;
    case 0x7000:
        opcode7XNN(opcode, cpu);
        break;
    case 0x8000:
    {
        switch (opcode & 0xF)
        {
        case 0x0:
            opcode8XY0(opcode, cpu);
            break;
        case 0x1:
            opcode8XY1(opcode, cpu);
            break;
        case 0x2:
            opcode8XY2(opcode, cpu);
            break;
        case 0x3:
            opcode8XY3(opcode, cpu);
            break;
        case 0x4:
            opcode8XY4(opcode, cpu);
            break;
        case 0x5:
            opcode8XY5(opcode, cpu);
            break;
        case 0x6:
            opcode8XY6(opcode, cpu);
            break;
        case 0x7:
            opcode8XY7(opcode, cpu);
            break;
        case 0xE:
            opcode8XYE(opcode, cpu);
            break;
        }
        break;
    }
    case 0x9000:
        opcode9XY0(opcode, cpu);
        break;
    case 0xA000:
        opcodeANNN(opcode, cpu);
        break;
    case 0xB000:
        opcodeBNNN(opcode, cpu);
        break;
    case 0xC000:
        opcodeCXNN(opcode, cpu);
        break;
    case 0xD000:
        opcodeDXYN(opcode, cpu);
        break;
    case 0xE000:
    {
        switch (opcode & 0xF)
        {
        case 0xE:
            opcodeEX9E(opcode, cpu, keys);
            break;
        case 0x1:
            opcodeEXA1(opcode, cpu, keys);
            break;
        }
        break;
    }
    case 0xF000:
    {
        switch (opcode & 0xFF)
        {
        case 0x07:
            opcodeFX07(opcode, cpu);
            break;
        case 0x0A:
            opcodeFX0A(opcode, cpu, keys);
            break;
        case 0x15:
            opcodeFX15(opcode, cpu);
            break;
        case 0x18:
            opcodeFX18(opcode, cpu);
            break;
        case 0x1E:
            opcodeFX1E(opcode, cpu);
            break;
        case 0x29:
            opcodeFX29(opcode, cpu);
            break;
        case 0x33:
            opcodeFX33(opcode, cpu);
            break;
        case 0x55:
            opcodeFX55(opcode, cpu);
            break;
        case 0x65:
            opcodeFX65(opcode, cpu);
            break;
        }
        break;
    }
    default:
        break;
    }
}