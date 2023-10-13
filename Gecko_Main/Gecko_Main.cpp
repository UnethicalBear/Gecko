#include "Gecko_Main.hpp"

#define GECKO_DEBUG

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <math.h>

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
            catch(std::invalid_argument){
                #ifdef GECKO_DEBUG
                std::cout << "Invalid character detected - adding NULL WORD";
                #endif
            }
            ctr=0;
            tmpWORD = "";
            }
        }
        #ifdef GECKO_DEBUG
        for(int x : this->__internal_RAM){
            int y = x / pow(10,word-this->__config_opcodeBits);
            int z = x % (int)pow(10,word-this->__config_opcodeBits);
            std::cout << y << "|" << z << std::endl;
        }
        #endif
    }

    void execute(){
        for(int RAMWORD : this->__internal_RAM){
            int _opcode = RAMWORD / pow(10,this->__config_RAM_word_width-this->__config_opcodeBits);
            int _operand = RAMWORD % (int)pow(10,__config_RAM_word_width-this->__config_opcodeBits);
            interpretOpcodeOperandPair(_opcode,_operand);
        }
    }

    virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;   
    // this turns gecko into an abstract class so it cannot be directly instantiated.

    
    int readCache(int cacheAddr){
        return this->__internal_Cache[cacheAddr];
    }

    std::string readCacheBinaryString(int cacheAddr){
        return "";
        // int tmp = this->readCache(cacheAddr);
        // return this->toBinaryString(tmp);
    }

    char* readCacheBinaryArray(int cacheAddr){
        return NULL;
        // int tmp = this->readCache(cacheAddr);
        // reutrn this->toBinaryArray(tmp);
    }
    
    void writeCache(int cacheAddr, int newValue){}
    void writeCache(int cacheAddr, char* newValue){}
    void writeCache(int cacheAddr, std::string newValue){}

    ~Gecko() {
        
    }
};


class myGecko : public Gecko {
public:
    using Gecko::Gecko;
    void interpretOpcodeOperandPair(int opcode, int operand) override {

    }
};
   

int main() {
    //Gecko(int RAMAddrSize, int RAMwordSize, int opcodeBits, int cacheMemorySize, int cacheMemoryWordWidth, bool alwaysUseBinary = false) {
    std::cout << "Hello World!\n";
    myGecko g(1024, 8, 4, 16, 8, false);
    g.readRAM("input.txt");
}