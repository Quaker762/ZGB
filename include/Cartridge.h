#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <fstream>
#include <iostream>
#include <string>

class Cartridge
{
public:
    enum MBCTypes
    {
        CART_ROM,
        CART_MBC1,
        CART_MBC2,
        CART_MBC3,
        CART_MBC5,
        CART_UNDEF
    };

public:
    Cartridge(char* fname);
    ~Cartridge();
    void loadCart(char* fname);
    void getROMHeaderInfo();
    uint8_t* GameRom();
    std::string getROMName();

private:
    bool romLoaded = false;

    uint8_t m_MBCType = 0; //What MBC chip does the cart have?      @0x0147
    uint8_t m_ROMSize = 0; //What size is our ROM?                  @0x0148
    uint8_t m_RAMSize = 0; //How many ram Banks do we have?         @0x0149
    uint8_t m_region  = 0; //0 = Japanese; 1 = Non-Japanese         @0x014A
    uint64_t m_cartSize = 0; //The size of the ROM file
    uint8_t rom0[0x4000]; //The actual ROM (16kB large, fixed ROM, no MBC :( )
    std::string m_romName; //16 Character game title read from 0x134 - 0x142
};



#endif // CARTRIDGE_H
