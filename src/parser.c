#include "parser.h"

#define OPCODE_MASK 0x0000007F

enum OpcodeType get_type(uint8_t opcode) {
    switch (opcode) {
        case 0b0110011: return RTYPE;
        case 0b0
    }
    return RTYPE;
}

InstructionStructure get_structure(uint32_t instruction) {
    enum OpcodeType type = get_type(instruction);

    switch (type) {
        case RTYPE:

    }
}
