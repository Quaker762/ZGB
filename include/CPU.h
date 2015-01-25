#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <iostream>
#include <cstring>
#include <stdint.h>
#include <stdio.h>

#include "MMU.h"
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

	//Our main CPU function
	uint8_t     tick();

	void        dumpCPU();

	bool        isFlagSet(Flags flags);

private:

    MMU* mmu;
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
	//uint8_t ram[0xFFFF]; Put this in MMU.h to make everything cleaner
};

#endif // CPU_H_INCLUDED
