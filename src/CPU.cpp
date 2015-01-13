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

            if(BC.hi > 0xF)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x05: //Decrement B
        {
            BC.hi--;

            setFlagBit(SUB);

            if(!(BC.hi & 0xFF))
                setFlagBit(ZERO);

            if(BC.hi > 0xF)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x06: //Load immediate 8-bit value into B
        {
            BC.hi = memRead(PC.Word++);

            PC.Word += 2;
            return 8;
            break;
        }
    case 0x07: //Rotate A left once. Old Bit 7 to Carry and Bit 0
        {
            resetFlagBit(SUB);
            resetFlagBit(HALF_CARRY);

            AF.lo |= ((AF.hi & 0x80) >> 3); //This can be done with an "if" statement
            AF.hi <<= 1; //Do the shift
            AF.hi |= ((AF.lo & 0x10) >> 4); //Move bit 7 to bit 0 in A

            if(!(AF.hi & 0xFF))
                setFlagBit(ZERO);

            PC.Word++;

            return 4;
            break;
        }
    case 0x08: //Load immediate 16-bit value into the stack pointer
        {
            SP.Word = memRead16(PC.Word++);

            PC.Word += 3;
            return 20;
            break;
        }
    case 0x09: //Add BC to HL
        {
            HL.Word += BC.Word;

            resetFlagBit(SUB);

            if(HL.Word > 0xFF)
                setFlagBit(CARRY);

            if(HL.Word > 0xF)
                setFlagBit(HALF_CARRY);

            return 8;
            break;
        }
    case 0x0A: //Load value pointed to by BC into A
        {
            AF.hi = memRead16(BC.Word);

            return 8;
            break;
        }
    case 0x0B: //Decrement BC
    {
        BC.Word--;

        return 8;
        break;
    }
    case 0x0C: //Increment C
        {
            BC.lo++;

            resetFlagBit(SUB);

            if(!(BC.lo & 0xFF))
                setFlagBit(ZERO);

            if(BC.lo > 0xF)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x0D: //Decrement C
        {
            BC.lo--;

            setFlagBit(SUB);

            if(!(BC.lo & 0xFF))
                setFlagBit(ZERO);

            if(BC.lo > 0xF)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x0E: //Load immediate 8-bit value into C
        {
            BC.lo = memRead(PC.Word++);

            PC.Word += 2;
            return 8;

        }
    case 0x0F: //Rotate A right once. Old Bit 0 to Carry and Bit 7
        {
            resetFlagBit(SUB);
            resetFlagBit(HALF_CARRY);

            AF.lo |= ((AF.hi & 0x01) << 4); //This can be done with an "if" statement
            AF.hi >>= 1; //Do the shift
            AF.hi |= ((AF.lo & 10) << 3); //Move bit 0 to bit 7 in A

            if(!(AF.hi & 0xFF))
                setFlagBit(ZERO);

            PC.Word++;
            return 4;
            break;
        }
    case 0x10: //STOP
        {

        }
    case 0x11: //Load immediate 16-bit value into DE
        {
            DE.Word = memRead16(PC.Word++);

            PC.Word += 3;
            return 12;
            break;
        }
    case 0x12: //Load A into address pointed to by DE
        {
            memWrite(DE.Word, AF.hi);

            PC.Word++;
            return 8;
            break;
        }
    case 0x13: //Increment DE
        {
            DE.Word++;

            PC.Word++;
            return 8;
            break;
        }
    default:
        std::cerr << "Invalid opcode: " << (uint16_t)opcode << std::endl; //Cast as 16-bit so we don't print the
        dumpCPU(); //Dump ALL CPU values to console
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

/**
    Dump everything from the CPU
**/
void Z80::dumpCPU()
{
    std::cerr << "*DEBUG DUMP*:" << std::endl;

    std::cout << "A: " << std::hex << static_cast<int>(AF.hi) << std::endl;
    std::cout << "F: " << std::hex << static_cast<int>(AF.lo) << std::endl;
    std::cout << "B: " << std::hex << static_cast<int>(BC.hi) << std::endl;
    std::cout << "C: " << std::hex << static_cast<int>(BC.hi) << std::endl;
    std::cout << "D: " << std::hex << static_cast<int>(DE.hi) << std::endl;
    std::cout << "E: " << std::hex << static_cast<int>(DE.hi) << std::endl;
    std::cout << "H: " << std::hex << static_cast<int>(HL.hi) << std::endl;
    std::cout << "L: " << std::hex << static_cast<int>(HL.hi) << std::endl;

    std::cout << "AF: " << std::hex << AF.Word << std::endl;
    std::cout << "BC: " << std::hex << BC.Word << std::endl;
    std::cout << "DE: " << std::hex << DE.Word << std::endl;
    std::cout << "HL: " << std::hex << HL.Word << std::endl;
    std::cout << "PC: " << std::hex << PC.Word << std::endl;
    std::cout << "SP: " << std::hex << SP.Word << std::endl;
}
