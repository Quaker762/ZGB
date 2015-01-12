#include <iostream>
#include <stdio.h>

#include "../include/CPU.h"

Z80 cpu;

int main()
{
    cpu.logEverything();

    cpu.tick();


}
