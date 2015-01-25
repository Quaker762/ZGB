#include <iostream>
#include <stdio.h>

#include "../include/CPU.h"
#include "../include/Cartridge.h"

Z80 cpu;
Cartridge* cart = new Cartridge("roms/tetris.gb");

int main()
{
    cpu.dumpCPU();

    return 0;
}

