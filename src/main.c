#include "state.h"
#include "pipeline.h"
#include "opparser.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    // Little test

    registers[1] = 2;
    registers[2] = 3;

    printf("Registers before: %d, %d, %d\n", registers[1], registers[2], registers[3]);

    // IF?
    uint32_t instruction = 0x002081B3;

    // ID?
    InstructionStructure decoded = get_structure(instruction);

    // EX
    execute(&decoded);

    printf("Registers after: %d, %d, %d\n", registers[1], registers[2], registers[3]);

    return 0;
}
