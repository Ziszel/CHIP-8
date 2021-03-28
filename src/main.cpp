#include "../include/cpu.hpp"
#include "../include/opcodes.hpp"
#include "raylib.h"
#include <iostream>

typedef uint8_t BYTE;

void updateInput(std::array<BYTE, 16> &keys)
{
    if (IsKeyDown(KEY_ONE))
    {
        std::cout << "key 1" << std::endl;
        keys[0x0] = 1;
    }
    else if (IsKeyUp(KEY_ONE))
    {
        keys[0x0] = 0;
    }
    if (IsKeyDown(KEY_TWO))
    {
        std::cout << "key 2" << std::endl;
        keys[0x1] = 1;
    }
    else if (IsKeyUp(KEY_TWO))
    {
        keys[0x1] = 0;
    }
    if (IsKeyDown(KEY_THREE))
    {
        std::cout << "key 3" << std::endl;
        keys[0x2] = 1;
    }
    else if (IsKeyUp(KEY_THREE))
    {
        keys[0x2] = 0;
    }
    if (IsKeyDown(KEY_FOUR))
    {
        std::cout << "key 4" << std::endl;
        keys[0x3] = 1;
    }
    else if (IsKeyUp(KEY_FOUR))
    {
        keys[0x3] = 0;
    }
    if (IsKeyDown(KEY_Q))
    {
        std::cout << "key q" << std::endl;
        keys[0x4] = 1;
    }
    else if (IsKeyUp(KEY_Q))
    {
        keys[0x4] = 0;
    }
    if (IsKeyDown(KEY_W))
    {
        std::cout << "key w" << std::endl;
        keys[0x5] = 1;
    }
    else if (IsKeyUp(KEY_W))
    {
        keys[0x5] = 0;
    }
    if (IsKeyDown(KEY_E))
    {
        std::cout << "key e" << std::endl;
        keys[0x6] = 1;
    }
    else if (IsKeyUp(KEY_E))
    {
        keys[0x6] = 0;
    }
    if (IsKeyDown(KEY_R))
    {
        std::cout << "key r" << std::endl;
        keys[0x7] = 1;
    }
    else if (IsKeyUp(KEY_R))
    {
        keys[0x7] = 0;
    }
    if (IsKeyDown(KEY_A))
    {
        std::cout << "key a" << std::endl;
        keys[0x8] = 1;
    }
    else if (IsKeyUp(KEY_A))
    {
        keys[0x8] = 0;
    }
    if (IsKeyDown(KEY_S))
    {
        std::cout << "key s" << std::endl;
        keys[0x9] = 1;
    }
    else if (IsKeyUp(KEY_S))
    {
        keys[0x9] = 0;
    }
    if (IsKeyDown(KEY_D))
    {
        std::cout << "key d" << std::endl;
        keys[0xA] = 1;
    }
    else if (IsKeyUp(KEY_D))
    {
        keys[0xA] = 0;
    }
    if (IsKeyDown(KEY_F))
    {
        std::cout << "key f" << std::endl;
        keys[0xB] = 1;
    }
    else if (IsKeyUp(KEY_F))
    {
        keys[0xB] = 0;
    }
    if (IsKeyDown(KEY_Z))
    {
        std::cout << "key z" << std::endl;
        keys[0xC] = 1;
    }
    else if (IsKeyUp(KEY_Z))
    {
        keys[0xC] = 0;
    }
    if (IsKeyDown(KEY_X))
    {
        std::cout << "key x" << std::endl;
        keys[0xD] = 1;
    }
    else if (IsKeyUp(KEY_X))
    {
        keys[0xD] = 0;
    }
    if (IsKeyDown(KEY_C))
    {
        std::cout << "key c" << std::endl;
        keys[0xE] = 1;
    }
    else if (IsKeyUp(KEY_C))
    {
        keys[0xE] = 0;
    }
    if (IsKeyDown(KEY_V))
    {
        std::cout << "key v" << std::endl;
        keys[0xF] = 1;
    }
    else if (IsKeyUp(KEY_V))
    {
        keys[0xF] = 0;
    }
}

int main(void)
{
    // standard raylib setup
    const int screenWidth = 640;
    const int screenHeight = 320;
    const int drawingScale = 10;

    InitWindow(screenWidth, screenHeight, "CHIP-8 Emulator | C++ with Raylib");
    SetWindowPosition(800, 800);

    // setup objects
    Cpu *cpu = new Cpu();
    cpu->LoadROM();

    // keys
    std::array<BYTE, 16> keys = {0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0};

    // high but makes games more playable.
    const float fps = 120.0f;

    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {

        // update keyPress
        updateInput(keys);

        decodeOpcode(*cpu, keys);

        // update timers
        if (cpu->DT > 0)
        {
            cpu->DT--;
        }
        if (cpu->ST > 0)
        {
            cpu->ST--;
        }

        // draw the program.
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        for (int x = 0; x < 64; x++)
        {
            for (int y = 0; y < 32; y++)
            {
                if (cpu->screenData[x][y])
                {
                    //std::cout << "value is non zero" << "\n";
                    DrawRectangle(x * drawingScale,
                                  y * drawingScale,
                                  drawingScale,
                                  drawingScale,
                                  BLACK);
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}