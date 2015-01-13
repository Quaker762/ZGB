#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stdint.h>
#include <stdio.h>

#include "Global.h"

//REGISTER INDEXES
//These can be enums and use the actual bits so you can & them.
#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7

//FLAG bitmasks
#define ZERO_FLAG_MASK  0x80
#define SUB_FLAG_MASK   0x40
#define HC_FLAG_MASK    0x20
#define CARRY_FLAG_MASK 0x10

class Z80
{
	//Register type
	//Unions allow you to read and write the same region of memory in different ways
	// Word is the full 16 bytes, Low and High will allow you to access the low and
	//    high bytes.
	// Could possibly do this for each and every register so you can access variables
	//    individually such as the Flag bits of the AF register.
	typedef union
	{
		struct
		{
			uint8_t lo;
			uint8_t hi;
		};
		uint16_t Word;
	} Register16;

	//FLAG bits
	enum Flags
	{
		ZERO = 0x80,	// Zero Flag
		SUB = 0x40,	// Add/Subtract
		HALF_CARRY = 0x20,	// Half-Carry
		CARRY = 0x10	// Carry
	};

public:

	Z80();
	~Z80();

	//void        updateFlags(); //Update the FLAGS booleans.

	void        setFlagBit(Flags flags);
	void        resetFlagBit(Flags flags);

	//CPU functions
	uint16_t    getStackPointer();
	uint16_t    getProgramCounter();
	void        resetCPU();

	void        memWrite(uint16_t addr, uint8_t value);
	uint8_t     memRead(uint16_t addr);
	void        memWrite16(uint16_t addr, uint16_t value);
	uint16_t    memRead16(uint16_t addr);

	//Our main CPU function
	uint8_t     tick();

	void        logEverything();

private:
	/**
		Our 8, 8-bit CPU Registers; A, F(lags), B, C, D, E, H, L

		These registers can be paired to create a 16-bit register combo, as is done below
		**/
	Register16 AF, BC, DE, HL;

	/**
		CPU program counter. Stores current instruction to be performed in memory

		When the GameBoy is switched on, the Program Counter is intialised to 0x100, and the
		instruction found at this location is executed
		**/
	Register16 PC;

	/**
		CPU stack pointer. Stores current location in our stack.

		When the GameBoy is switched on, the Stack Pointer is initialised to 0xFFFE.
		**/
	Register16 SP;

	/**
		Our GameBoy memory! It can contains a maximum of 65535 (0xFFFF in hex) 8-bit values. It is mapped as follows:

		**/
	// Todo: Gameboy memory is "paged" or "banked" so different chunks may be -armed- at different moments of time.
	uint8_t ram[0xFFFF];
};

#endif // CPU_H_INCLUDED
