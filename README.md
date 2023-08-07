14 32 bit general purpose registers
32 bit stack base register
32 bit stack end register
32 bit memory addresses
32 bit word size
4 flags that are changed on certain arithmetic operations (Zero flag, sign flag, overflow flag, carry flag)
Von Neumann architecture
# Register IDs

32 bit register 0 - 32 bit register 7 have IDs 0 through 13.
Stack base is id 14.
Stack end is id 15.

# Instructions

The first 2 bits determine the “mode” of the instruction:

00: Memory/arithmetic computation
01: Memory/arithmetic computation with an intermediate
10: conditional jump instruction
11: function call instruction

The next 2 bits determine the size of the instruction, with 00 meaning an instruction size of 2 and adding continues from there.

The next 4 bits usually determine the opcode, with the exception being function calls, discussed later. Here are the bit patterns for the different modes:


| First byte | Second byte | Notes | 
| ---------- | ----------- | ----------- |
| ```00 00 OOOO```     | ```RRRR RRRR```   |  This instruction is the only instruction that is fixed size and is also the only reason that I have 16 registers.
| ```01 SS OOOO```     | ```RRRR IIII``` | To have bigger intermediates, change the instruction size.
| ```10 SS OOOO```    | ``` PPPPPPPPPP``` | Size of instruction can be extended to accomodate times when you want to have code longer than 255 instructions.
| ```11 SS M PPPP``` | ```PPPPPPPP```  | Mode is 0 for call and 1 for ret. Above note still applies

With the instructions out of the way, we will now be talking about computation opcodes.
Computation opcodes are separated into the memory unit, and the arithmetic unit, indicated by the MSB of the opcode. Some but not all opcodes set the 4 flags. Unless specified, all ALU opcodes that don't specify a change to a flag will have the flags not be changed.

You might notice that some memory operations sound like arithmetic operations. This is because I ran out of space in the ALU opcode section, and am too lazy to fix my design.

| OPCODE | NAME            | Operation                          | Flags  |
|--------|-----------------|------------------------------------|--------|
| `0000` | `ADD`           | `A = A + B`                        | `ZSCO` |
| `0001` | `SUB`           | `A = A - B`                        | `ZSCO` |
| `0010` | `MUL`           | `A = A * B`                        | `ZSCO` |
| `0011` | `CMP`           | `null = A - B`                     | `ZSCO` |
| `0100` | `OR`            | `A = A \| B`                       | `ZS`   |
| `0101` | `XOR`           | `A = A ^ B`                        | `ZS`   |
| `0110` | `AND`           | `A = A & B`                        | `ZS`   |
| `0111` | `CLEAR`         | `All flags are cleared`            | `ZSCO` |
| `1000` | `MOV`           | `A = B`                            | None   |
| `1001` | `EXEC`          |`Execute code from RAM[A] to RAM[B]`| None   |
| `1010` | `LOAD`          | `A = RAM[B]`                       | None   |
| `1011` | `STORE`         | `RAM[B] = A`                       | None   |
| `1100` | `DIVMOD`        | `A = A / B, B = A % B`             | `ZS`   |
| `1101` | `MUL`           |  `A = A * B`                       | `ZSCO` |
| `1110` | `PUSH`          | `stackend += 1, add value to top`  | `ZS`   |
| `1111` | `POP`           | `stackend += 1, get value and put it in A`|`ZS`   |

# Notes about STORE

For "computer being able to do anything" purposes, the STORE instruction doesn't allow intermediates. Instead, the size bits are non-zero if you are storing to console, which acts as if the console is a big 1d array.

# Details regarding flags
The Z flag indicates that the result was zero.
The S flag indicates that the result, was negative (the MSB was 1).
The C flag describes whether an additive instruction caused a carry, or a subtractive computation caused a borrow. For example, if you have a 2 bit number, 0b1 * 0b1 would cause a carry
The O flag indicates that the operation caused overflow. This means that a loss of precision occurred for a signed integer calculation (eg. positive + positive = negative or negative + negative = positive).

# Jump opcodes

Jumps are based on the flags. The middle column represents the equivalent instruction if using the CMP instruction. If a flag is not mentioned, there is an implicit "&& !FLAG" present.

|OPCODE   | CONDITION       | Flags                              |
|---------|-----------------|------------------------------------|
| `0000`  | `==`            | `Z`                                |
| `0001`  | `!=`            | `!Z`                               |
| `0010`  | `Negative`      | `S`                                |
| `0011`  | `Not negative`  | `!S`                               |
| `0100`  | `Carry`         | `C`                                |
| `0101`  | `No carry`           | `!C`                        |
| `0110`  | `Overflow`           | `O`                        |
| `0111`  | `Not overflow`         | `!O`            |
| `1000`  | `<`           | `S != O`                            |
| `1001`  | `>=`          |`S == O`|
| `1010`  | `>`          | `!Z && (N == O)`                       |
| `1011`  | `<=`         | `Z \|\| (N != O)`                       |
| `1100`  | `Always`        | `1`             |
| `1101`  | `Never`           |  `0`                       |
| `1110`  | `unsigned >`           | `C && !Z`                       |
| `1111`  | `unsigned <=`           | `!C || Z`                       |


# Regarding reading own program memory
By default, the stack bottom register has the address of your last instruction + 1. You can deduce the start of your program from that.

There is also nothing to stop you from completely overriding your own program memory, or any other program's memory. 