#include "../include/CPU.h"

/**
    Class Constructor
**/
Z80::Z80()
{
    mmu = new MMU();
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
}

/**
	Our CPU's cycle function! It executes the current instruction according the address the program counter is
	pointing at and returns the number of cycles each instruction takes to execute
**/
uint8_t Z80::tick()
{
    uint8_t opcode = mmu->memRead(PC.Word);

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
            BC.Word = mmu->memRead16(PC.Word++);

            PC.Word += 3;
            return 12;
            break;
        }
    case 0x02: //Load A into address stored in BC
        {
            mmu->memWrite(BC.Word, AF.hi);

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
            BC.hi = mmu->memRead(PC.Word++);

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
            SP.Word = mmu->memRead16(PC.Word++);

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
            AF.hi = mmu->memRead16(BC.Word);

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
            BC.lo = mmu->memRead(PC.Word++);

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
            PC.Word++;
        }
    case 0x11: //Load immediate 16-bit value into DE
        {
            DE.Word = mmu->memRead16(PC.Word++);

            PC.Word += 3;
            return 12;
            break;
        }
    case 0x12: //Load A into address pointed to by DE
        {
            mmu->memWrite(DE.Word, AF.hi);

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
    case 0x14: //Increment D
        {
            DE.hi++;

            if(!(DE.hi & 0xFF))
                setFlagBit(ZERO);

            if(DE.hi > 0x0F)
                setFlagBit(HALF_CARRY);

            resetFlagBit(SUB);


            PC.Word++;
            return 4;
            break;
        }
    case 0x15: //Decrement D
        {
            DE.hi--;

            if(!(DE.hi & 0xFF))
                setFlagBit(ZERO);

            if(DE.hi > 0x0F)
                setFlagBit(HALF_CARRY);

            setFlagBit(SUB);

            PC.Word++;
            return 4;
            break;
        }
    case 0x16: //Load immediate 8-bit value into D
        {
            DE.hi = mmu->memRead(PC.Word++);

            PC.Word+=2;
            return 8;
            break;
        }
    case 0x17: //Rotate A left
        {
            resetFlagBit(ZERO);
            resetFlagBit(HALF_CARRY);
            resetFlagBit(SUB);

            uint8_t bit7 = AF.hi & 0x80;
            AF.hi <<= 1;
            AF.hi |= (AF.lo & 0x10) >> 4;

            if(bit7)
                setFlagBit(CARRY);

            PC.Word += 1;
            return 4;
            break;
        }
    case 0x18: //Add N to current address and jump to it
        {
            PC.Word += (signed)memRead(PC.Word + 1); //The immediate 8 bit value is signed

            PC.Word += 2;
            return 12;
            break;
        }
    case 0x19: //Add HL to DE
        {
            DE.Word += HL.Word;

            resetFlagBit(SUB);

            if(DE.Word + HL.Word > 0xFF)
                setFlagBit(CARRY);

            if(DE.Word > 0x0F)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 8;
            break;
        }
    case 0x1A: //Load value at address in DE into A
        {
            AF.hi = memRead16(DE.Word);

            PC.Word++;
            return 8;
            break;
        }
    case 0x1B: //Decrement DE
        {
            DE.Word--;

            PC.Word++;
            return 8;
            break;
        }
    case 0x1C: //Increment E
        {
            DE.lo++;

            resetFlagBit(ZERO);

            if(!(DE.lo & 0xFF))
                setFlagBit(ZERO);

            if(DE.lo > 0x0F)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x1D: //Decrement E
        {
            DE.lo--;

            setFlagBit(SUB);

            if(!(DE.lo & 0xFF))
                setFlagBit(ZERO);

            if(DE.lo > 0x0F)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x1E: //Load immediate 8-bit value into E
        {
            DE.lo = memRead(PC.Word++);

            PC.Word += 2;
            return 8;
            break;
        }
    case 0x1F: //Rotate A right
        {
            resetFlagBit(ZERO);
            resetFlagBit(HALF_CARRY);
            resetFlagBit(SUB);

            uint8_t bit0 = AF.hi & 0x01;
            AF.hi >>= 1;
            AF.hi |= (AF.lo & 0x10) << 4;

            if(bit0)
                setFlagBit(CARRY);

            PC.Word += 1;
            return 4;
            break;
        }
    case 0x20:
        {
            if(!isFlagSet(ZERO))
            {
                PC.Word += (signed)memRead(PC.Word + 1);
                return 12;
                break;
            }

            PC.Word++;
            return 8;
            break;
        }
    case 0x21: //Load immediate 16-bit value into DE
        {
            DE.Word = memRead16(PC.Word++);

            PC.Word+=3;
            return 12;
            break;
        }
    case 0x22: //Load A into HL, then increment
        {
            (HL.Word = AF.hi)++;

            PC.Word++;
            return 8;
            break;
        }
    case 0x23: //INcrement HL
        {
            HL.Word++;

            PC.Word++;
            return 8;
            break;
        }
    case 0x24: //Increment H
        {
            HL.hi++;

            resetFlagBit(SUB);

            if(!(HL.hi & 0xFF))
                setFlagBit(ZERO);

            if(HL.hi > 0x0F)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x25: //Decrement H
        {
            HL.hi--;

            setFlagBit(SUB);

            if(!(HL.hi & 0xFF))
                setFlagBit(ZERO);

            if(HL.hi > 0x0F)
                setFlagBit(HALF_CARRY);

            PC.Word++;
            return 4;
            break;
        }
    case 0x26: //Load immediate 8-bit value into H
        {
            HL.hi = memRead(PC.Word++);

            PC.Word += 2;
            return 8;
            break;
        }
    case 0x27: //Decimal Adjust A after addition
        {

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

void Z80::setFlagBit(Flags flags)
{
    AF.lo |= flags;
}

void Z80::resetFlagBit(Flags flags)
{
    AF.lo &= ~(flags);
}

bool Z80::isFlagSet(Flags flags)
{
    return AF.lo & flags;
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
