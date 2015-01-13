#include "../include/CPU.h"

/**
    Class Constructor
**/
Z80::Z80()
{
    resetCPU(); //Reset, yo!
}

/**
    Class Destructor
**/
Z80::~Z80()
{

}

/**
    Reset the CPU to it's initial state (outlined in the CPU manual, pg.64)
**/
void Z80::resetCPU()
{
    //Reset our PC and SP to their defaults
    PC.Word = 0x0100;
    SP.Word = 0xFFFE;
    memset(ram, 0x00, 0xFFFF); //Zero our ram
}

/**
	Our CPU's cycle function! It executes the current instruction according the address the program counter is
	pointing at and returns the number of cycles each instruction takes to execute
**/
uint8_t Z80::tick()
{
    uint8_t opcode = ram[PC.Word];

    switch(opcode)
    {
    case 0x00: //NOP
        {
            PC.Word++;
            return 4; //4 cycle instruction
            break;
        }
    case 0x01: //Load immediate 16-bit value into register BC
        {
            BC.Word = memRead16(PC.Word++);

            PC.Word += 3;
            return 12;
            break;
        }
    case 0x02: //Load A into address stored in BC
        {
            memWrite(BC.Word, AF.hi);

            PC.Word++;
            return 8;
            break;
        }
    case 0x03: //Increment BC
        {
            BC.Word++;

            PC.Word++;
            return 8;
            break;
        }
    case 0x04: //Increment B register
        {
            BC.hi++;

            resetFlagBit(SUB);

            if(!(BC.hi & 0xFF))
                setFlagBit(ZERO);
        }
    case 0x05: //Decrement B
        {

        }
    default:
        std::cerr << "Invalid opcode: " << (uint16_t)opcode << std::endl; //Cast as 16-bit so we don't print the
        exit(-1); //Exit the program with an error
    }
    return 4;
}

/**
    Get the address the program counter is currently pointing to.
**/
uint16_t Z80::getProgramCounter()
{
    return PC.Word & 0xFFFF; //For sanity, &0xFFFF to ensure we are returning a 16-bit value
}

/**
    Get the address the pstack pointer is currently pointing to.
**/
uint16_t Z80::getStackPointer()
{
    return SP.Word & 0xFFFF;
}

/*************************************************************************
*
*                           MEMORY FUNCTIONS
*
*************************************************************************/

/**
    Read an 8-bit value from memory
**/
uint8_t Z80::memRead(uint16_t addr)
{
    return ram[addr];
}

/**
    Write an 8-bit value to memory
**/
void Z80::memWrite(uint16_t addr, uint8_t value)
{
    ram[addr] = value;
}

/**
    Read a 16-bit value from memory
**/
uint16_t Z80::memRead16(uint16_t addr)
{
    uint8_t hi = ram[addr] & 0xFF;
    uint8_t lo = (ram[addr] >> 8) & 0xFF;

    return (hi << 8) | lo;
}

/**
    Write a 16-bit value to memory
**/
void Z80::memWrite16(uint16_t addr, uint16_t value)
{
    ram[addr] = value >> 8;
    ram[addr+1] = value & 0xFF;
}

void Z80::setFlagBit(Flags flags)
{
    AF.lo |= flags;
}

void Z80::resetFlagBit(Flags flags)
{
    AF.lo &= ~(flags);
}

//Dump EVERYTHING from the CPU to a nice table in console :)
void Z80::logEverything()
{
	std::cout << "A    |" << "F    |" << "B    |" << "C    |" << "D    |" << "E    |" << "H    |" << "L    |" << "SP    |" << "PC" << std::endl;
	//cout doesn't like uint8_t (prints out the char value, I can't be stuffed casting...), so we're using printf!
	printf("%x   %x    %x    %x    %x    %x    %x    %x    %x    %x", AF.hi, AF.lo, BC.hi, BC.lo, DE.hi, DE.lo, HL.hi, HL.lo, this->getStackPointer(), this->getProgramCounter());
}
