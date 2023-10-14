#include "Gecko.hpp"
#define GECKO_DEBUG

class Gecko {
protected:
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

    std::vector<int> STATUS_REGISTER_CONFIG = {};
    std::vector<int> GENERAL_PURPOSE_REGISTERS = {};

    int GENERAL_PURPOSE_REGISTER_CONFIG = 0;
    int ACC_REGISTER_CONFIG = 0;

    bool useSignedIntegers = false;


    std::string toBinaryString(int input, bool isSigned) {  // convert integer to a std::string binary representation.
        std::string bin = "";
        int num = input;
        if (num < 0 && isSigned) {
            bin += "1";
            num = -(num + 1); // Invert the number and subtract 1 for two's complement representation
        }
        else {
            bin += "0";
        }

        // Convert the number to binary representation
        for (int i = this->__config_RAM_word_width - 1; i >= 0; --i) {
            int bit = (num >> i) & 1;
            bin += std::to_string(bit);
        }
        return bin;
    }

    const char* toBinaryArray(int input, bool isSigned) {
        return this->toBinaryString(input, isSigned).c_str();
    }

    // Cache Operations

    void writeCache(int cacheLevel, int cacheAddr, int newValue){

    }
    void writeCache(int cacheLevel, int cacheAddr, char* newValue){
    
    }
    void writeCache(int cacheLevel, int cacheAddr, std::string newValue){
    
    }

    int readCache(int cacheLevel, int cacheAddr) {
        return this->__internal_Cache[cacheAddr];
    }

    std::string readCache(std::string cacheLevel, std::string cacheAddr) {
        return toBinaryString(
            readCache(
                stoi(cacheLevel, 0, 2), 
                stoi(cacheAddr, 0, 2)
           ), 
           this->useSignedIntegers
        );
    }

    char* readCacheBinaryArray(int cacheAddr) {
        return NULL;
        // int tmp = this->readCache(cacheAddr);
        // reutrn this->toBinaryArray(tmp);
    }

    void SetupGeneralPurposeRegisters(std::vector<std::string>s) {

    }
    

public:
    Gecko() {
        this->__config_RAM_SIZE_MAX = 0;
        this->__config_RAM_word_width = 0;
        this->__config_alwaysUseBinary = 0;
        this->__config_cacheMemorySize = 0;
        this->__config_cacheMemoryWidth = 0;
        this->__config_opcodeBits = 0;
        this->__config_operandBits = 0;
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
    
    bool start() {

        return true;
    }



    bool readRAM(std::string inputFile) {
        std::ifstream fileHandler(inputFile);
        std::string words;
        char ch;
        std::string tmpWORD = "";
        int ctr = 0;
        while (fileHandler.get(ch)){
            tmpWORD += ch;
            ctr++;
            if (ctr == this->__config_RAM_word_width){
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
            int y = x / pow(10, __config_RAM_word_width-this->__config_opcodeBits);
            int z = x % (int)pow(10, __config_RAM_word_width-this->__config_opcodeBits);
            std::cout << y << "|" << z << std::endl;
        }
        #endif
        return true;
    }

    bool execute(){
        for(int RAMWORD : this->__internal_RAM){
            int _opcode = RAMWORD / pow(10,this->__config_RAM_word_width-this->__config_opcodeBits);
            int _operand = RAMWORD % (int)pow(10,__config_RAM_word_width-this->__config_opcodeBits);
            interpretOpcodeOperandPair(_opcode,_operand);
        }
        return true;
    }
    // this turns gecko into an abstract class so it cannot be directly instantiated.
    virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;   
    virtual void setup() = 0;

    bool quickSetup(std::string inputFile) {
        if (this->start()) {
            return this->readRAM(inputFile);
        }
        return false;
    }

    bool quickRun(std::string inputFile) {
        if (this->start()){
            if (this->readRAM(inputFile)) {
                return this->execute();
            }
        }
        return false;
    }

    ~Gecko() {
        
    }
};

// ------------------------- Custom implementation ------------------------ //

class myGecko : public Gecko {
public:
    using Gecko::Gecko;
    void interpretOpcodeOperandPair(int opcode, int operand) override {
        std::cout << this->toBinaryString(10, this->useSignedIntegers);
    }
    void setup() override {
           
        this->useSignedIntegers = GECKO_USE_SIGNED_INTS;

        this->ACC_REGISTER_CONFIG = GECKO_REG_USE_STR_ID;
        this->GENERAL_PURPOSE_REGISTER_CONFIG = GECKO_REG_USE_STR_ID;

        this->STATUS_REGISTER_CONFIG = {           // ALU status / processing registers
            GECKO_REG_ACC_IS_NON_ZERO,
            GECKO_REG_PROCESSOR_OK,
            GECKO_REG_ACC_IS_ODD,
            GECKO_ARITHMETIC_CARRY
        };
        // Program Counter and Accumulator are automatically instantiated. They are 0 and 1 respectively. When using strings they are PC and ACC
        // if using INT IDs, just call this->autoSetupGPR(noOfRegisters);

        this->SetupGeneralPurposeRegisters({
                "MDR",
                "MAR",
                "OUTC",
                "OUTD",
                "GP0",
                "GP1"
        });
    }
};
   

int main() {
    myGecko g(1024, 8, 4, 16, 8,  false); // You can also hard code these in your custom class if you want, and then have an empty constuctor.
    g.quickRun("input.txt"); // Batch start, configure, read RAM and then execute in one function. Will not procced to the next task unless the previous one was successfull.
}

