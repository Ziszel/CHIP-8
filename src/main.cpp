#include "../include/cpu.hpp"
#include "../include/opcodes.hpp"
#include "raylib.h"
#include "updateInput.hpp"
#include <iostream>

typedef uint8_t BYTE;

int main(void)
{
    // standard raylib setup
    const int screenWidth = 640;
    const int screenHeight = 320;
    const int drawingScale = 10;

    InitWindow(screenWidth, screenHeight, "CHIP-8 Emulator | C++ with Raylib");
    InitAudioDevice();

    Sound beep = LoadSound("res/beep.wav");

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

    // screen refresh
    const float fps = 60.0f;
    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {

        // update keyPress
        updateInput(keys);

        decodeOpcode(*cpu, keys);

        // update timers
        if (cpu->DT > 0)
        {
            if (!IsSoundPlaying(beep))
            {
                PlaySound(beep);
            }
            cpu->DT--;
        }
        if (cpu->ST > 0)
        {
            if (!IsSoundPlaying(beep))
            {
                PlaySound(beep);
            }
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

    UnloadSound(beep);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}