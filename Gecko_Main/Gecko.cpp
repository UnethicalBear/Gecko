#include "Gecko.hpp"
#define GECKO_DEBUG

class Gecko {
// =============== START OF PROTECTED METHODS ============= //
protected:
    int __config_RAM_word_width;
    int __config_RAM_SIZE_MAX;
    int __config_operandBits;
    int __config_opcodeBits;
    int* __config_cacheMemorySize;      // list of cache sizes by level, e.g. [16,128,256]
    int* __config_cacheMemoryWidth;     // list of cache widths by level, e.g. [8,8,8]

    bool    __internal_useSignedIntegers = false;
    int     __internal_bootloaderFailSafe;
    bool    __internal_registerCombineCarryBorrow;
    int     __internal_bootloaderInputBase = 2;

    bool __config_alwaysUseBinary = false;

    std::vector<int> __internal_RAM = {};
    std::vector<std::vector<int>> __internal_Cache = {};
    std::map<std::string, int> __internal_stringRegisters = {};
    int* __internal_intRegisters;

    std::vector<int> STATUS_REGISTER_CONFIG = {};
    std::vector<int> GENERAL_PURPOSE_REGISTERS = {};

    int GENERAL_PURPOSE_REGISTER_CONFIG = 0;
    int ACC_REGISTER_CONFIG = 0;

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
        return this->toBinaryString(input, isSigned).c_str(); // NOQA
    }

    // Cache Operations

    void writeCache(int cacheLevel, int cacheAddr, int newValue){

    }
    void writeCache(int cacheLevel, int cacheAddr, char* newValue){
    
    }
    void writeCache(int cacheLevel, int cacheAddr, std::string newValue){
    
    }

    int readCache(int cacheLevel, int cacheAddr) {
        return this->__internal_Cache[cacheLevel][cacheAddr];
    }

    std::string readCache(std::string cacheLevel, std::string cacheAddr) {
        return toBinaryString(
            readCache(
                stoi(cacheLevel, 0, 2), 
                stoi(cacheAddr, 0, 2)
           ), 
           this->__internal_useSignedIntegers
        );
    }

    const char* readCacheBinaryArray(const char* cacheLevel, const char* cacheAddr) {
        return this->readCache(std::string(cacheLevel), std::string(cacheAddr)).c_str();
    }

    void SetupStringRegisters(std::vector<std::string>s) {
        for (std::string _register : s) {
            this->__internal_stringRegisters.insert({ _register,0 });
        }
        this->__internal_stringRegisters.insert({ "ACC",0 });
        this->__internal_stringRegisters.insert({ "PC",0 });
    }


    void SetupIntegerRegisters(int noOfRegisters) {
        // This doesnt include PC and ACC registers (0 and 1 respectively).
        noOfRegisters += 2;
        this->__internal_intRegisters = new int[noOfRegisters];
        for (int i = 0; i < noOfRegisters; ++i) {
            __internal_intRegisters[i] = 0;
        }
    }

    // ================= END OF PROTECTED METHODS ===================== //
    // ================= START OF PUBLIC METHODS  ===================== //
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
        this->__config_cacheMemorySize = new int { cacheMemorySize };
        this->__config_cacheMemoryWidth = new int{ cacheMemoryWordWidth };

        this->__config_opcodeBits = opcodeBits;
        this->__config_operandBits = RAMwordSize - opcodeBits;
    }
    Gecko(int RAMAddrSize, int RAMwordSize, int opcodeBits, int* cacheMemorySize, int* cacheMemoryWordWidth, bool alwaysUseBinary = false) {
        this->__config_RAM_SIZE_MAX = RAMAddrSize;
        this->__config_RAM_word_width = RAMwordSize;
        this->__config_alwaysUseBinary = alwaysUseBinary;
        this->__config_cacheMemorySize = cacheMemorySize;
        this->__config_cacheMemoryWidth = cacheMemoryWordWidth;

        this->__config_opcodeBits = opcodeBits;
        this->__config_operandBits = RAMwordSize - opcodeBits;
    }

    bool config(bool useSignedIntegers = false, bool useRegisterStringIDs = true, bool combineCarryBorrowReg = true, int bootloaderErrorCodeHandler = 0){
        this->__internal_useSignedIntegers = useSignedIntegers;
        //
        this->__internal_registerCombineCarryBorrow = combineCarryBorrowReg;
        this->__internal_bootloaderFailSafe = bootloaderErrorCodeHandler;

        return true;
    }
    
    bool executeBootloader(std::string inputFile) {
        std::ifstream fileHandler(inputFile);
        std::string words;
        char ch;
        std::string tmpWORD = "";
        int ctr = 0;
        while (fileHandler.get(ch)){
            if (isspace(ch)) {
                continue;
            }
            tmpWORD += ch;
            ctr++;
            if (ctr == this->__config_RAM_word_width){
                try {
                    this->__internal_RAM.push_back(stoi(tmpWORD));
                }
                catch(std::invalid_argument){
                    #ifdef GECKO_DEBUG
                    std::cout << "Invalid character sequence detected!";
                    #endif

                    switch (this->__internal_bootloaderFailSafe) {
                    case GECKO_APP_INVALID_CODE_ADD_NULL:
                        this->__internal_RAM.push_back(0);
                        break;
                    
                    case GECKO_APP_INVALID_CODE_TERMINATE:
                        throw std::invalid_argument("Invalid code loaded from bootloader: " + tmpWORD);
                        break;

                    case GECKO_APP_INVALID_CODE_CUSTOM:
                        this->returnCustomBootloaderSafeCode(tmpWORD);
                    }

                }
                ctr=0;
                tmpWORD = "";
            }
        }
        #ifdef GECKO_DEBUG
        for(int x : this->__internal_RAM){
            /*int y = x / pow(10, __config_RAM_word_width-this->__config_opcodeBits);*/
            //int z = x % (int)pow(10, __config_RAM_word_width-this->__config_opcodeBits);
            std::string w = std::to_string(x);
            if (w.size() == 7) { w = "0" + w; }
            std::string y = w.substr(0, this->__config_opcodeBits);
            std::string z = w.substr(this->__config_opcodeBits);
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

    // ================ VIRTUAL FUNCTIONS ================ //

    // this turns Gecko into an abstract class so it cannot be directly instantiated.
    virtual void interpretOpcodeOperandPair(int opcode, int operand) = 0;   
    virtual bool setup() = 0;
       
    int returnCustomBootloaderSafeCode(std::string codeRead) { return 0; }   // Overrided by the user if needed.

    // ================= QUICK EXEC ====================== //

    bool quickSetup(std::string inputFile) {
        if (this->setup()) {
            return this->executeBootloader(inputFile);
        }
        return false;
    }

    bool quickRun(std::string inputFile) {
        if (this->setup()){
            if (this->executeBootloader(inputFile)) {
                return this->execute();
            }
        }
        return false;
    }

    // ===================== DESTRUCTORS =================== //

    ~Gecko() {
        
    }
};

// ------------------------- Custom implementation ------------------------ //

class myGecko : public Gecko {
public:
    using Gecko::Gecko;
    void interpretOpcodeOperandPair(int opcode, int operand) override {}
    
    bool setup() override {
           
        this->ACC_REGISTER_CONFIG = GECKO_REG_USE_INT_ID;
        this->GENERAL_PURPOSE_REGISTER_CONFIG = GECKO_REG_USE_INT_ID;

        this->STATUS_REGISTER_CONFIG = {           // ALU status / processing registers
            GECKO_REG_ACC_IS_NON_ZERO,
            GECKO_REG_PROCESSOR_OK,
            GECKO_REG_ACC_IS_ODD,
            GECKO_ARITHMETIC_CARRY
        };
        // Program Counter and Accumulator are automatically instantiated. They are 0 and 1 respectively. When using strings they are PC and ACC
        // if using INT IDs, just call this->SetupIntegerRegisters(noOfRegisters);

       /* this->SetupStringRegisters({
                "MDR",
                "MAR",
                "OUTC",
                "OUTD",
                "GP0",
                "GP1"
        });*/

        this->SetupIntegerRegisters(6);
        return true;
    }
};

int main() {
    myGecko g(1024, 8, 4, 16, 8, false); // You can also hard code these in your custom class if you want, and then have an empty constuctor.
    g.config(GECKO_USE_UNSIGNED_INTS, GECKO_REG_USE_STR_ID, false, GECKO_APP_INVALID_CODE_TERMINATE);
    g.setup();
    g.executeBootloader("input.txt");
    g.execute();

    // Breakpoint so I can actually debug this mess.
    int i;
    std::cin >> i;
}

