#include "../include/Cartridge.h"

Cartridge::Cartridge()
{

}

Cartridge::~Cartridge()
{

}

//Load our cartridge into the ROM array
void Cartridge::loadCart(char* fname)
{
    std::ifstream romfile(fname, std::ios::in | std::ios::binary);

    if(romfile.is_open())
    {
        romfile.read((char*)rom0, sizeof(romfile));
        romfile.close();
    }

    romLoaded = true;
    getROMHeaderInfo();
}

void Cartridge::getROMHeaderInfo()
{
    //Make sure our ROM is actually loaded first...
    if(romLoaded)
    {
        ROMHeaderInfo.MBCType = readRom(0x0147);
        ROMHeaderInfo.ROMSize = readRom(0x0148);
        ROMHeaderInfo.RAMSize = readRom(0x0149);
        ROMHeaderInfo.region  = readRom(0x014A);
        printf("MBC: 0x%x\nROM Size: 0x%x\nRAM Size: 0x%x\nRegion: 0x%x\n",
            ROMHeaderInfo.MBCType, ROMHeaderInfo.ROMSize, ROMHeaderInfo.RAMSize,
            ROMHeaderInfo.region);
    }
    else
    {
        std::cerr << "Attempting to read header when ROM hasn't been loaded...";
    }
}

//Read a value from the ROM
uint8_t Cartridge::readRom(uint16_t addr)
{
    return rom0[addr];
}

//Return a pointer to the ROM
uint8_t* Cartridge::GameRom()
{
    return rom0;
}
