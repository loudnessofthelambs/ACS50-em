#include <vector>
#include <array>
#include <stdint.h>
typedef int8_t BYTE;
int registers[16];
BYTE flags; // bits go from left to right. Bit 1 is Z, bit 2 is S, bit 3 is C, bit 4 is O
BYTE RAM[4294967295];
int main(int argc, char **argv) {

}
void execute(int programStart, int programEnd) {
    BYTE op_size;
}
void execute_instruction(std::vector<BYTE> instruction, int& ip) {

}
void execute_alu(int& a, int& b, BYTE opcode) {
    int result;
    switch (opcode) {
        case 0b0000:
            result = a + b;
            if ((unsigned int)result < a) {
                flags |= 0b00100000;
            }
            if ((a > 0 && b > 0 && result <= 0) || (a < 0 && b < 0 && result >= 0)) {
                flags |= 0b1000;
            }
            a = result;
            break;
        case 0b0001:
            result = a - b;
            if ((unsigned)a > (unsigned)b) {
                flags |= 0b00100000;
            }
            int nB = ~b;
            int result = a - (nB+1);
            if ((result >> 31 != nB >> 31 && nB >> 31 == a >> 31 )) {
                flags |= 0b1000;
            }
        
    }
    if (result == 0) {
        flags |= 0b10000000;
    }
    if (result < 0) {
        flags |= 0b01000000;
    }
}
