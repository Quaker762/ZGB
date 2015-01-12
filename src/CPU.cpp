#include "../include/CPU.h"

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
        {
            ++pc;
            return 4; //4 Cycles required
            break;
        }
    case 0x01: //16-bit load to BC
        {
            writeReg16(B, C, memRead16(pc++));

            pc += 3;
            return 12;
            break;
        }
    case 0x02: //Store A into address pointed to by BC
        {
            memWrite(readRegister16(B, C), readRegister(A));

            ++pc;
            return 8;
            break;
        }
    case 0x03: //INC BC
        {
            uint16_t regpair = readRegister16(B, C);
            regpair++;
            writeReg16(B, C, regpair);

            ++pc;
            return 4;
            break;
        }
    case 0x04: //INC B
        {
            uint8_t b = readRegister(B);
            ++b;

            unsetFlagBit(SUB_BIT);

            if(!(b & 0xFF)) //Is our answer zero??
                setFlagBit(ZERO_BIT);

            if(b > 0xF)
                setFlagBit(HC_BIT);

            ++pc;
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

            ++pc;
            return 4;
            break;
        }
    case 0x06: //Load immediate 8-bit value into B
        {
            writeReg(B, getProgramCounter() + 1);

            pc += 2;
            return 8;
            break;
        }
    case 0x07: //Rotate A Left Through Carry...
        {
            uint8_t a = readRegister(A);
            uint8_t bit7 = a & 0x80;

            unsetFlagBit(ZERO_BIT);
            unsetFlagBit(SUB_BIT);
            unsetFlagBit(HC_BIT);

            if(bit7)
                setFlagBit(CARRY_BIT);

            a <<= 1; //Perform the shift
            a |= bit7 >> 7;

            if(!(a & 0xFF)) //Do a zero check after the shift
                setFlagBit(ZERO_BIT);

            writeReg(A, a); //Write the register back to the array

            return 4;
            break;
        }
    case 0x08: //Load SP into 16-bit immediate address
        {
            memWrite16(sp,  memRead16(pc + 1));

            pc += 3;
            return 20;
            break;
        }

    case 0x09: //ADD HL, BC
        {
            uint16_t hl = readRegister16(H, L);
            uint16_t bc = readRegister16(B, C);

            unsetFlagBit(SUB_BIT);

            if((hl + bc) > 0xFF)
                setFlagBit(CARRY_BIT);

            if((hl & 0xF) + (bc & 0xF) > 0xF);
                setFlagBit(HC_BIT);

            writeReg16(H, L, (hl + bc));

            incrementProgramCounter();

            return 8;
            break;
        }
    case 0x0A: //Load value at BC to register A
        {
            uint16_t addr = readRegister16(B, C);
            writeReg(A, memRead(addr));

            ++pc;
            return 8;
            break;
        }
    case 0x0B: //DEC BC
        {
            uint16_t bc = readRegister16(B, C);
            --bc;

            writeReg16(B, C, bc);

            ++pc;
            return 8;
            break;
        }
    case 0x0C: //Increment C
        {
            uint8_t c = readRegister(C);
            ++c;

            unsetFlagBit(SUB_BIT);

            if(!(c & 0xFF))
                setFlagBit(ZERO_BIT);

            if(c > 0xF)
                setFlagBit(HC_BIT);

            ++pc;
            return 4;
            break;
        }
    case 0x0D: //Decrement C
        {
            uint8_t c = readRegister(C);
            --c;

            setFlagBit(SUB_BIT); //We performed a subtraction

            if(!(c & 0xFF))
                setFlagBit(ZERO_BIT);

            if(c > 0xF)
                setFlagBit(HC_BIT);

            ++pc;
            return 4;
            break;
        }
    case 0x0E: //Load immediate 8-bit value into C
        {
            writeReg(C, pc + 1);

            pc += 2;
            return 8;
            break;
        }
    case 0x0F: //Rotate A right through carry
        {
            uint8_t a = readRegister(A);
            uint8_t bit0 = a & 0x01;

            unsetFlagBit(ZERO_BIT);
            unsetFlagBit(SUB_BIT);
            unsetFlagBit(HC_BIT);

            if(bit0) //If bit0 == 1, set the carry bit to 1 as well
                setFlagBit(CARRY_BIT);

            a >>= 1; //Perform the shift
            a |= bit0 << 7;

            if(!(a & 0xFF)) //Do a zero check after the shift
                setFlagBit(ZERO_BIT);

            writeReg(A, a);

            return 4;
            break;
        }
    case 0x10:
    {
        //TODO: How to interpret this opcode
        pc += 2;
        return 4;
        break;
    }
    case 0x11: //Load immediate 16-bit value into DE
        {
            writeReg16(D, E, memRead16(pc++));

            pc += 3;
            return 12;
            break;
        }
    case 0x12: //Load A into address pointed to by DE
        {
            uint16_t addr = readRegister16(D, E);
            memWrite(addr, readRegister(A));

            ++pc;
            return 8;
            break;
        }
    case 0x13:
        {
           uint16_t de = readRegister16(D, E);
           ++de;

           writeReg16(D, E, de);

           ++pc;
           return 8;
           break;
        }
    case 0x14: //Increment D
        {
            uint8_t d = readRegister(D);
            ++d;

            unsetFlagBit(SUB_BIT);

            if(!(d & 0xFF))
                setFlagBit(ZERO_BIT);

            if(d > 0xF)
                setFlagBit(HC_BIT);

            ++pc;
            return 4;
            break;
        }
    case 0x15: //Decrement D
        {
            uint8_t d = readRegister(D);
            --d;

            setFlagBit(SUB_BIT);

            if(!(d & 0xFF))
                setFlagBit(ZERO_BIT);

            if(d > 0xF)
                setFlagBit(HC_BIT);

            ++pc;
            return 4;
            break;
        }
    case 0x16: //Load immediate 8-bit value into D
        {
            writeReg(D, pc++);

            pc += 2;
            return 8;
            break;
        }
    case 0x17: //Rotate A left once. Bit 7 to carry flag and carry bit to bit 0 of A
        {
            uint8_t a = readRegister(A);
            uint8_t bit7 = a & 0x80;

            unsetFlagBit(ZERO_BIT);
            unsetFlagBit(SUB_BIT);
            unsetFlagBit(HC_BIT);

            if(bit7)
                setFlagBit(CARRY_BIT);

            uint8_t cbit = readRegister(F) & CARRY_FLAG_MASK;

            a <<= 1; //Perform the shift
            a |= cbit >> 4;

            if(!(a & 0xFF)) //Do a zero check after the shift
                setFlagBit(ZERO_BIT);

            writeReg(A, a); //Write the register back to the array

            ++pc;
            return 4;
            break;
        }
    case 0x18: //Add 8-bit immediate value to PC and jump to that address
        {
            uint16_t addr = pc + (++pc)

            pc = addr;
            return 8;
            break;
        }
    case 0x19: //Add DE to HL
        {
            uint16_t de = readRegister16(D, E);
            uint16_t hl = readRegister16(H, L);

            if((hl + de) > 0xFF)
                setFlagBit(CARRY_BIT);

            if((hl & 0xF) + (de & 0xF) > 0xF);
                setFlagBit(HC_BIT);

            writeReg16(H, L, (hl + de));

            ++pc;
            return 8;
            break;
        }
    case 0x1A: //Load value at Address in DE into reg A
        {
            uint8_t a = readRegister(A);
            uint16_t addr = readRegister16(D, E);

            a = memRead16(addr); //Read address in DE

            writeReg(A, a);
            ++pc;
            return 8;
            breakl
        }
    case 0x1B: //Decrement DE
        {
            uint16_t de = readRegister16(D, E);

            --de;

            ++pc;
            return 8;
            break;
        }
    case 0x1C:
        {
            uint8_t e = readRegister(E);

            ++e;

            unsetFlagBit(SUB_BIT);

            if(!(e & 0xFF))
                setFlagBit(ZERO_BIT);

            if(e > 0xF)
                setFlagBit(HC_BIT);
        }
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

//The GameBoy is Little-Endian, right???
void Z80::memWrite16(uint16_t addr, uint16_t value)
{
    uint8_t val_hi = value & 0xFF;
    uint8_t val_lo = (value >> 8) & 0xFF;

    memWrite(addr, val_hi);
    memWrite(addr, val_lo);
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
