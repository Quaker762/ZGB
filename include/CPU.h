#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stdint.h>
#include <stdio.h>

#include "Global.h"

//REGISTER INDEXES
#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7

//FLAG bits
#define ZERO_BIT        7
#define SUB_BIT         6
#define HC_BIT          5
#define CARRY_BIT       4

//FLAG bitmasks
#define ZERO_FLAG_MASK  0x80
#define SUB_FLAG_MASK   0x40
#define HC_FLAG_MASK    0x20
#define CARRY_FLAG_MASK 0x10

class Z80
{
public:

    Z80();
    ~Z80();

    uint8_t     readRegister(uint8_t reg);
    uint16_t    readRegister16(uint8_t reg1, uint8_t reg2); //Pair two registers together and return the value (TODO: How are we storing registers??)
    void        updateFlags(); //Update the FLAGS booleans.
    void        writeReg(uint8_t reg, uint8_t value); //Write to a regigster
    void        writeReg16(uint8_t reghi, uint8_t reglo, uint16_t value);

    void        setFlagBit(uint8_t bit);
    void        unsetFlagBit(uint8_t bit);

    //CPU functions
    void        incrementStackPointer();
    void        decrementStackPointer();
    uint16_t    getStackPointer();
    void        incrementProgramCounter();
    void        decrementProgramCounter();
    void        setProgramCounter(uint16_t value);
    uint16_t    getProgramCounter();
    void        resetCPU();

    void        memWrite(uint16_t addr, uint8_t value);
    uint8_t     memRead(uint16_t addr);
    void        memWrite16(uint16_t addr, uint16_t value);
    uint16_t    memRead16(uint16_t addr);


    //Our main CPU function
    uint8_t cycle();


    void logEverything();

private:
    /**
        Our 8, 8-bit CPU Registers; A, F(lags), B, C, D, E, H, L

        These registers can be paired to create a 16-bit register combo
    **/
    uint8_t regs[8];

    /**
        CPU program counter. Stores current instruction to be performed in memory

        When the GameBoy is switched on, the Program Counter is intialised to 0x100, and the
        instruction found at this location is executed
    **/
    uint16_t pc;

    /**
        CPU stack pointer. Stores current location in our stack.

        When the GameBoy is switched on, the Stack Pointer is initialised to 0xFFFE.
    **/
    uint16_t sp;

    /**
        Our GameBoy memory! It can contains a maximum of 65535 (0xFFFF in hex) 8-bit values. It is mapped as follows:


    **/
    uint8_t ram[0xFFFF];

    /**
        Our FLAG Booleans. These are used to check what
    **/

    bool zero = false; //Result of math operation is zero
    bool sub = false; //Subtraction was performed last math instruction
    bool hc = false; //Carry occurred from lower nibble in last math operation
    bool carry = false; //Carry occurred last math operation, or REG A is smaller than value when CP instruction is executed
};


#endif // CPU_H_INCLUDED
