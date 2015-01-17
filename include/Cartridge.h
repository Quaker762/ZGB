#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <fstream>
#include <iostream>
#include <cstring>
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
    uint8_t *rom0; //Will be dynamically allocated when we read the ROM into memory.
};



#endif // CARTRIDGE_H
