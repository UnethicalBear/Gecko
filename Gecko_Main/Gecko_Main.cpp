#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <math.h>

int main(){
    std::cout << "Hello World!\n";
}

class Gecko {
private:
    int __config_RAM_word_width;
    int __config_RAM_SIZE_MAX;
    int __config_operandBits;
    int __config_opcodeBits;
    int __config_cacheMemorySize;
    int __config_cacheMemoryWidth;

    int __reg_accumulator = 0;

    bool __config_alwaysUseBinary = false;

    std::vector<int> __internal_RAM = {};
    std::vector<int> __internal_Cache = {};

public:
    Gecko() {

    }
    Gecko(int RAMAddrSize, int RAMwordSize, int opcodeBits, int cacheMemorySize, int cacheMemoryWordWidth, bool alwaysUseBinary = false) {
        this->__config_RAM_SIZE_MAX = RAMAddrSize;
        this->__config_RAM_word_width = RAMwordSize;
        this->__config_alwaysUseBinary = alwaysUseBinary;
        this->__config_cacheMemorySize = cacheMemorySize;
        this->__config_cacheMemoryWidth = cacheMemoryWordWidth;

        this->__config_opcodeBits = opcodeBits;
        this->__config_operandBits = RAMwordSize - opcodeBits;
    }

    void readRAM(std::string inputFile) {
        int word = 8;
        std::ifstream fileHandler(inputFile);
        std::string words;
        char ch;
        std::string tmpWORD = "";
        int ctr = 0;
        while (fileHandler.get(ch)){
            tmpWORD += ch;
            ctr++;
            if (ctr == word){
            try {
                this->__internal_RAM.push_back(stoi(tmpWORD));
            }
            catch(...){
                #ifdef DEBUG
                std::cout << "Invalid";
                #endif
            }
            ctr=0;
            tmpWORD = "";
            }
        }
        #ifdef GECKO_DEBUG
        for(int x : RAM){
            int y = x / pow(10,word-4);
            int z = x % (int)pow(10,word-4);
            std::cout << y << " " << z << std::endl;
        }
        #endif
    }

    virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;   
    // this turns gecko into an abstract class so it cannot be directly instantiated.

    ~Gecko() {
        
    }
};

