#include "CPU.h"

/**
    Class Constructor
**/
Z80::Z80()
{
    resetCPU(); //Reset, yo!
}

/**
    Class Deconstructor
**/
Z80::~Z80()
{

}

/**
    Our CPU's cycle function! It executes the current instruction according the address the program counter is
    pointing at and returns the number of cycles each instruction takes to execute
**/
uint8_t Z80::cycle()
{
    uint8_t opcode = ram[pc]; //FETCH!

    //DECODE!
    switch(opcode)
    {
    case 0x00: //NOP
        incrementProgramCounter();
        return 4; //4 Cycles required
        break;
    case 0x01: //16-bit load to BC
        writeReg16(B, C, memRead16(pc++));
        setProgramCounter(getProgramCounter() + 3); //Next instruction
        return 12;
        break;
    case 0x02: //Store A into address pointed to by BC
        memWrite(readRegister16(B, C), readRegister(A));
        incrementProgramCounter();
        return 8;
        break;
    case 0x03: //INC BC
        {
            uint16_t regpair = readRegister16(B, C);
            regpair++;
            writeReg16(B, C, regpair);
            return 4;
            break;
        }
    case 0x04: //INC B
        {
            uint8_t b = readRegister(B);

            if(!(b & 0xFF)) //Is our answer zero??
                setFlagBit(ZERO_BIT);

            unsetFlagBit(SUB_BIT);

            //TODO: How to check for a half-carry
            return 4;
            break;
        }
    case 0x05: //DEC B
        {
            uint8_t b = readRegister(B);
            --b;

            if(!(b & 0xFF)) //Is our answer zero??
                setFlagBit(ZERO_BIT);

            if(b > (b & 0xF))
                setFlagBit(HC_BIT);

            setFlagBit(SUB_BIT); //We performed a subtraction! Set sub bit

            writeReg(B, b);
        }
        return 4;
        break;
    case 0x06:
        writeReg(B, getProgramCounter() + 1);
        incrementProgramCounter();
        return 8;
        break;
    case 0x07: //Rotate Left Through Carry...
        {
            //All Bits of FLAGS except carry are rest
            unsetFlagBit(ZERO_BIT);
            unsetFlagBit(HC_BIT);
            unsetFlagBit(SUB_BIT);
        }
        return 4;
        break;
    case 0x08:
        return 4;
        break;
    case 0x09: //ADD HL, BC
        {
            uint16_t hl = readRegister16(H, L);
            uint16_t bc = readRegister16(B, C);

            if((hl + bc) > 0xFF)
                setFlagBit(CARRY_BIT);

        }
        return 8;
        break;
    case 0x0A:
        return 4;
        break;
    case 0x0B:
        return 4;
        break;
    case 0x0C:
        return 4;
        break;
    case 0x0D:
        return 4;
        break;
    case 0x0E:
        return 4;
        break;
    case 0x0F:
        return 4;
        break;
    case 0x10:
        return 4;
        break;
    case 0x11:
        return 4;
        break;
    case 0x12:
        return 4;
        break;
    case 0x13:
        return 4;
        break;
    case 0x14:
        return 4;
        break;

    default:
        {
            std::cerr << "INVALID OPCODE: " << ram[pc];
            exit(INVALID_OPCODE_EXIT);
            break;
        }
    }
    return 4;
}

/**
    Reset the CPU to it's initial initialisation state (outline in the CPU manual, pg.64)
**/
void Z80::resetCPU()
{
    //Reset our PC and SP to their defaults
    pc = 0x0100;
    sp = 0xFFFE;
    memset(ram, 0x00, 0xFFFF); //Zero our RAM
}

void Z80::incrementProgramCounter()
{
    ++pc;
}

void Z80::decrementProgramCounter()
{
    --pc;
}

void Z80::setProgramCounter(uint16_t value)
{
    pc = value;
}

uint16_t Z80::getProgramCounter()
{
    return pc & 0xFFFF;
}

void Z80::incrementStackPointer()
{
    ++sp;
}

void Z80::decrementStackPointer()
{
    --sp;
}

uint16_t Z80::getStackPointer()
{
    return sp & 0xFFFF;
}

/**
    Write an 8-bit value to GameBoy memory
**/
void Z80::memWrite(uint16_t addr, uint8_t value)
{
    ram[addr] = value & 0xFF;
}

/**
    Read an 8-bit value from memory and align it 16-bit
**/
uint8_t Z80::memRead(uint16_t addr)
{
    uint16_t ret = ram[addr] & 0xFF; //If we're using an 8-bit load, why the fuck 0xFFFF????

    return ret;
}

void Z80::memWrite16(uint16_t addr, uint16_t value)
{
    uint8_t val_hi = (value >> 8) & 0xFF;
    uint8_t val_lo = value & 0xFF;

    //TODO: How are these values then written to the memory???
}

uint16_t Z80::memRead16(uint16_t addr)
{
    uint16_t ret = ((ram[addr] & 0xFF) << 8) + (ram[addr + 1] & 0xFF);

    return ret;
}

//Read a register pair
uint16_t Z80::readRegister16(uint8_t reg1, uint8_t reg2)
{
    uint16_t ret = (reg1 << 8) | reg2;

    return ret;
}

//Thank you, /g/...
void Z80::updateFlags()
{
    //Update our flags booleans by determining what bits are switched on in the FLAGS register.
    zero = (bool)(regs[F] & ZERO_FLAG_MASK);
    sub = (bool)(regs[F] & SUB_FLAG_MASK);
    hc = (bool)(regs[F] & HC_FLAG_MASK);
    carry = (bool)(regs[F] & CARRY_FLAG_MASK);
}

uint8_t Z80::readRegister(uint8_t reg)
{
    return regs[reg];
}

//Write "value" to register "reg"
void Z80::writeReg(uint8_t reg, uint8_t value)
{
    regs[reg] = value;
}

//Write a 16 bit value to a register pair
void Z80::writeReg16(uint8_t reghi, uint8_t reglo, uint16_t value)
{
    //Seperate the value into it's low and high byte
    //GameBoy is apparently Little Endian, so store the LSB into the hi register in the pair
    regs[reghi] = value & 0xFF;
    regs[reglo] = value >> 8;
}

//Set bits in the FLAG register
void Z80::setFlagBit(uint8_t bit)
{
    uint8_t flags = readRegister(F);
    flags |= 1 << bit;
    writeReg(F, flags);
    updateFlags();
}

//Unset bit in the FLAG register
void Z80::unsetFlagBit(uint8_t bit)
{
    uint8_t flags = readRegister(F);
    flags &= ~(1 << bit);
    writeReg(F, flags);
    updateFlags();
}

//Dump EVERYTHING from the CPU to a nice table in console :)
void Z80::logEverything()
{
    std::cout << "A    |" << "F    |" << "B    |" << "C    |" << "D    |" << "E    |" << "H    |" << "L    |" << "SP    |" << "PC" << std::endl;
    //cout doesn't like uint8_t (prints out the char value, I can't be stuffed casting...), so we're using printf!
    printf("%x   %x    %x    %x    %x    %x    %x    %x    %x    %x", regs[A], regs[F], regs[B], regs[C], regs[D], regs[E], regs[H], regs[L], this->getStackPointer(), this->getProgramCounter());
}
