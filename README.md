# Gecko
 Emulator Engine Library

## What is Gecko?
Gecko is a library that allows you to easily create custom emulators, such as for retro consoles, or homebrew CPUs or embedded systems.

## How does Gecko work?
Gecko works in a simple manner. It handles most of the tough work for you. All you need to do is define one function which interprets each opcode and operand pair. Everything else can be removed from your view.

## Gecko Example

```C++

#include "Gecko_Main.h"
#include <iostream>


class myGecko : public Gecko {
public:
	using Gecko::Gecko;

	void interpretOpcodeOperandPair(int opcode, int operand) override {
			/* Do your calculations here!

			...
			...
			...

			*/
	}
};

int main(){
	myGecko emulator(
		1024,	/* size of RAM */
		8,		/* size of one WORD in RAM */
		4,		/* how many bits are used for the OPCODE. Operand is calculated as RAM_WORD - OPCODE */
		16,		/* size of cache memory */
		8		/* the bit width of one CACHE WORD */
	);


	myGecko.readRAM("RAM_file.txt");
	myGecko.execute();
}
```


## Docs

### class Gecko

### macros

#### Status Registers

- GECKO_STATUS_REG_ACC_IS_EVEN

Whether the accumulator's value is even.

- GECKO_STATUS_REG_ACC_IS_EVEN

Whether the acucumulator's value is odd.

- GECKO_STATUS_REG_ACC_IS_EVEN

- GECKO_STATUS_REG_ACC_IS_EVEN

- GECKO_STATUS_REG_ACC_IS_EVEN