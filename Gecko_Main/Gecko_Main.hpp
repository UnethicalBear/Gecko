#pragma once

// ----------------------- Basic Setup ------------------------------------- //

#define GECKO_APP
//#define GECKO_APP_DEBUG // put this in your file to turn DEBUG on.

// ----------------------- Register Congi ---------------------------------- //

#define GECKO_STATUS_REG_ACC_IS_EVEN							1
#define GECKO_STATUS_REG_ACC_IS_ODD								2
#define GECKO_STATUS_REG_ACC_IS_ZERO							3
#define GECKO_STATUS_REG_ACC_IS_NON_ZERO						4
#define GECKO_STATUS_REG_ACC_PROCESSOR_OK						5
#define GECKO_ARITHMETIC_CARRY									6
#define GECKO_ARITHMETIC_BORROW									7

// ----------------------- Integer to Binary Config ------------------------- //

#define GECKO_BINSTR_CHRn                                       0
#define GECKO_BINSTR_STDSTRING                                  1

// ----------------------- String to Integer Exception Config --------------- //

#define GECKO_APP_INVALID_CODE_TERMINATE						0
#define GECKO_APP_INVALID_CODE_ADD_NULL							1
#define GECKO_APP_INVALID_CODE_CUSTOM							2


// ------------------------ Gecko Class Defenition Template ----------------- //

class Gecko {
public:
	Gecko();
	Gecko(int RAMAddrSize, int RAMwordSize, int opcodeBits, int cacheMemorySize, int cacheMemoryWordWidth, bool alwaysUseBinary = false);

	void readRAM(std::string inputFile);

	void execute();
	virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;
	
	int readCache(int cacheAddr);
	std::string readCacheBinaryString(int cacheAddr);
	char* readCacheBinaryArray(int cacheAddr);


	void writeCache(int cacheAddr, int newValue);
	void writeCache(int cacheAddr, char* newValue);
	void writeCache(int cacheAddr, std::string newValue);





};