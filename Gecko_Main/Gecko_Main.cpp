#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <map>

int main(){
    std::cout << "Hello World!\n";
}

class Gecko {
    int __config_RAM_word_width;
    int __config_RAM_SIZE_MAX;
    int __config_operandBits;
    int __config_opcodeBits;
    int __config_cacheMemorySize;
    int __config_cacheMemoryWidth;

    int __reg_accumulator = 0;

    bool __config_alwaysUseBinary = false;

public:
    Gecko() {

    }
    Gecko(int RAMAddrSize, int RAMwordSize, int opcodeBits, int cacheMemorySize, int cacheMemoryWordWidth, bool alwaysUseBinary = false) {
        this->__config_RAM_SIZE_MAX = RAMAddrSize;
        this->__config_RAM_word_width = RAMwordSize;
        this->__config_alwaysUseBinary = alwaysUseBinary;
        this->__config_cacheMemorySize = cacheMemorySize;
        this->__config_cacheMemoryWidth = cacheMemoryWordWidth;
    }

    void readRAM(std::string inputFile) {

    }

    virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;   
    // this turns gecko into an abstract class so it cannot be directly instantiated.


    ~Gecko() {

    }


};

