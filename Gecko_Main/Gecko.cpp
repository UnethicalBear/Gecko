#define GECKO_APP
//#define GECKO_DEBUG
#include "Gecko.hpp"

class myGecko : public Gecko {
public:
    using Gecko::Gecko;

    bool setup() override {

        this->config(       // this can also be called after the setup function in main()
            GECKO_USE_UNSIGNED_INTS,
            GECKO_REG_USE_STR_ID,
            false,
            GECKO_APP_INVALID_CODE_ADD_NULL
        );

        this->ACC_REGISTER_CONFIG = GECKO_REG_USE_STR_ID;
        this->GENERAL_PURPOSE_REGISTER_CONFIG = GECKO_REG_USE_STR_ID;

        this->STATUS_REGISTER_CONFIG = {           // ALU status / processing registers
            {GECKO_REG_ACC_IS_NON_ZERO, 0},
            {GECKO_REG_PROCESSOR_OK,    0},
            {GECKO_REG_ACC_IS_ODD,      0},
            {GECKO_ARITHMETIC_CARRY,    0}
        };
        // Program Counter and Accumulator are automatically instantiated. They are 0 and 1 respectively. When using strings they are PC and ACC
        // Carry and borrow are 2 and 3 respectively, under the codes ACC_CARRY and ACC_BORROW respectively. 
        // When combined, reference either since their value will be tied together.
        this->SetupStringRegisters({
                "OUTC",
                "OUTD",
                "GP0",
                "GP1"
            });
        return true;
    }
    void interpretOpcodeOperandPair(int opcode, int operand) override { 
        std::cout << opcode << " " << operand << std::endl;
        switch (opcode) {
        //case 0:
            // NOP - Nothing happens
            //break;
        case 1:
            // LDA 
            this->__internal_stringRegisters["ACC"] = this->__internal_RAM[operand];
            break;
        case 2:
            // STO
            this->__internal_RAM[operand] = __internal_stringRegisters["ACC"];
            break;
        case 3:
            // ADD 
            this->__internal_stringRegisters["ACC"] = this->__internal_stringRegisters["ACC"] + operand;
            break;
        case 4:
            // ADD 
            this->__internal_stringRegisters["ACC"] = this->__internal_stringRegisters["ACC"] + operand;
            break;
        }   
    }

};

int main() {
    myGecko g(1024, 8, 4, 16, 8, false); // You can also hard code these in your custom class if you want, and then have an empty constuctor.
    g.setup();
    g.executeBootloader("input.txt");
    g.execute();

    // Breakpoint so I can actually debug this mess.
    int i;
    std::cin >> i;
}

