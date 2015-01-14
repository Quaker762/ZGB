#ifndef CARTRIDGE_H
#define CARTRIDGE_H

class Cartridge
{
public:

private:
    uint8_t m_MBCType = 0; //What MBC chip does the cart have?      @0x0147
    uint8_t m_ROMSize = 0; //What size is our ROM?                  @0x0148
    uint8_t m_RAMSize = 0; //How many ram Banks do we have?         @0x0149
    uint8_t m_region  = 0; //0 = Japanese; 1 = Non-Japanese         @0x014A
};



#endif // CARTRIDGE_H
