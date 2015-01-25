#include <cstring>
#include "../include/Cartridge.h"

Cartridge::Cartridge(char* fname)
{
    loadCart(fname);
}

Cartridge::~Cartridge()
{

}

//Load our cartridge into the ROM array
void Cartridge::loadCart(char* fname)
{
    std::ifstream romfile(fname, std::ios::in | std::ios::binary);

    if(!romfile.is_open())
    {
        std::cerr << "Cannot open ROM file, Aborting...\n";
        return;
    }

    romfile.seekg(0, std::ios::end);
    uint64_t rsize = romfile.tellg(); //Find the file size by seeking the entire file.

    romfile.seekg(0, std::ios::beg); //Go back to the beginning of the file
    romfile.read((char*)rom0, rsize / 2); //Read the first primary ROM bank (Bank 0)
    romfile.close();

    romLoaded = true;
}

void Cartridge::getROMHeaderInfo()
{
    //Make sure our ROM is actually loaded first...
    if(romLoaded)
    {
        char buffer[16];

        for(int i = 0; i <= 16; i++)
        {
            if(rom0[0x0134 + i] == 0x00)
            {
                buffer[i] = '\0';
                break;
            }

            buffer[i] = rom0[0x0134 + i];
        }

        m_MBCType = rom0[0x0147];
        m_ROMSize = rom0[0x0148];
    }
    else
    {
        std::cerr << "Attempting to read header when ROM hasn't been loaded...\n";
    }
}

//Return a pointer to the ROM
uint8_t* Cartridge::GameRom()
{
    return rom0;
}

//Our ROM's name
std::string Cartridge::getROMName()
{
    return m_romName;
}
