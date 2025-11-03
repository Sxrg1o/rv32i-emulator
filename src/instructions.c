#include "instructions.h"
#include "state.h"
#include "parser.h"

void handle_add(InstructionStructure structure) {
    registers[structure.rd] = registers[structure.rs1] + registers[structure.rs2];
}
