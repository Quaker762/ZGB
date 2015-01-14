/**
    Our Memory Management Unit. The GameBoy's(DMG) memory map is as follows:

  Interrupt Enable Register

 --------------------------- FFFF

  Internal RAM

 --------------------------- FF80

  Empty but unusable for I/O

 --------------------------- FF4C

  I/O ports

 --------------------------- FF00

  Empty but unusable for I/O

 --------------------------- FEA0

  Sprite Attrib Memory (OAM)

 --------------------------- FE00

  Echo of 8kB Internal RAM

 --------------------------- E000

  8kB Internal RAM

 --------------------------- C000

  8kB switchable RAM bank

 --------------------------- A000

  8kB Video RAM

 --------------------------- 8000

  32kB Cartridge

 --------------------------- 0000

 Also note that the GameBoy's memory can be banked using an external Memory Bank Controller on the actual cart.
 I have no fucking idea how these work, but they allow more RAM and a larger ROM size to be used.

**/

#include "../include/MMU.h"

/**
    Class Constructor
**/
MMU::MMU()
{

}

/**
    Class Deconstructor
**/
MMU::~MMU()
{

}

/**
    Read an 8-bit value from memory
**/
uint8_t MMU::memRead(uint16_t addr)
{
    return m_ram[addr];
}

/**
    Read a 16-bit value from memory
**/
uint16_t MMU::memRead16(uint16_t addr)
{
    return (m_ram[addr + 1] << 8) | m_ram[addr];
}

/**
    Write an 8-bit value to memory
**/
void MMU::memWrite(uint16_t addr, uint8_t value)
{
    m_ram[addr] = value;
}

/**
    Write a 16-bit value to memory
**/
void MMU::memWrite16(uint16_t addr, uint16_t value)
{
    m_ram[addr] = value & 0xFF;
    m_ram[addr] = (value >> 8) & 0xFF;
}
