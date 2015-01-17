#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <fstream>
#include <iostream>
#include "ROMHeader.h"

class Cartridge
{
public:
    Cartridge();
    ~Cartridge();

    void loadCart(char* fname);
    void getROMHeaderInfo();

    uint8_t readRom(uint16_t addr);
    uint8_t* GameRom();


private:
    bool romLoaded = false;
    s_ROMHeaderInfo ROMHeaderInfo;
    uint8_t rom0[0x8000]; //The actual ROM (32kB large, fixed ROM, no MBC :( )
};



#endif // CARTRIDGE_H
