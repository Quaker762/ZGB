#include <iostream>
#include <stdio.h>

#include "../include/CPU.h"
#include "../include/Cartridge.h"

Z80 cpu;
Cartridge* cart = new Cartridge();

int main()
{
    cpu.dumpCPU();

    cart->loadCart("roms/tetris.gb");

    std::cout << (uint16_t)cart->GameRom()[0x0001];
}

