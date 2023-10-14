#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <math.h>

// ----------------------- Basic Setup ------------------------------------- //

#define GECKO_APP
//#define GECKO_APP_DEBUG // put this in your file to turn DEBUG on.

// ----------------------- Register Config ---------------------------------- //

#define GECKO_REG_ACC_IS_EVEN									1
#define GECKO_REG_ACC_IS_ODD									2
#define GECKO_REG_ACC_IS_ZERO									3
#define GECKO_REG_ACC_IS_NON_ZERO								4
#define GECKO_REG_PROCESSOR_OK									5

#define GECKO_REG_USE_INT_ID									0	// use integer IDs to address registers
#define GECKO_REG_USE_STR_ID									1	// use string IDs to address registers.

// ------------------------ Arithmetic -------------------------------------- //

#define GECKO_ARITHMETIC_CARRY									6	// CARRY register from addition, can be combined with BORROW.
#define GECKO_ARITHMETIC_BORROW									7	// BORROW register from subtraction, can be combined to CARRY.

#define GECKO_USE_SIGNED_INTS									0
#define GECKO_USE_UNSIGNED_INTS									1


// ----------------------- Integer to Binary Config ------------------------- //

#define GECKO_BINSTR_CHRn                                       0
#define GECKO_BINSTR_STDSTRING                                  1

// ----------------------- String to Integer Exception Config --------------- //

#define GECKO_APP_INVALID_CODE_TERMINATE						0
#define GECKO_APP_INVALID_CODE_ADD_NULL							1
#define GECKO_APP_INVALID_CODE_CUSTOM							2


// ------------------------ Gecko Class Defenition Template ----------------- //

// for some reason uncommenting this breaks EVERYTHING

//class Gecko {
//protected:
//	std::string toBinaryString(int input, bool isSigned);
//	const char* toBinaryArray(int input, bool isSigned);
//
//	int readCache(int cacheAddr);
//	std::string readCacheBinaryString(int cacheAddr);
//	char* readCacheBinaryArray(int cacheAddr);
//
//	void writeCache(int cacheAddr, int newValue);
//	void writeCache(int cacheAddr, char* newValue);
//	void writeCache(int cacheAddr, std::string newValue);
//
//public:
//	Gecko();
//	Gecko(int RAMAddrSize, int RAMwordSize, int opcodeBits, int cacheMemorySize, int cacheMemoryWordWidth, bool alwaysUseBinary = false);
//
//	void readRAM(std::string inputFile);
//
//	void execute();
//	virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;
//};