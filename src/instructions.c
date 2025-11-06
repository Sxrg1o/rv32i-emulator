#include "instructions.h"
#include "state.h"
#include "parser.h"

void handle_lui(struct InstructionStructure* inst) {
    registers[inst->rd] = inst->imm;
}

void handle_auipc(struct InstructionStructure* inst) {
    registers[inst->rd] = program_counter + inst->imm;
}

void handle_jal(struct InstructionStructure* inst) {
    registers[inst->rd] = program_counter + 4;
    program_counter += inst->imm;
}

void handle_jalr(struct InstructionStructure* inst) {
    registers[inst->rd] = program_counter + 4;
    program_counter = registers[inst->rs1] + inst->imm;
}

void handle_beq(struct InstructionStructure* inst) {
    if(registers[inst->rs1] == registers[inst->rs2]) {
        program_counter += inst->imm;
    }
}

void handle_bne(struct InstructionStructure* inst) {
    if(registers[inst->rs1] != registers[inst->rs2]) {
        program_counter += inst->imm;
    }
}

void handle_blt(struct InstructionStructure* inst) {
    if((int32_t)registers[inst->rs1] < (int32_t)registers[inst->rs2]) {
        program_counter += inst->imm;
    }
}

void handle_bge(struct InstructionStructure* inst) {
    if((int32_t)registers[inst->rs1] >= (int32_t)registers[inst->rs2]) {
        program_counter += inst->imm;
    }
}

void handle_bltu(struct InstructionStructure* inst) {
    if(registers[inst->rs1] < registers[inst->rs2]) {
        program_counter += inst->imm;
    }
}

void handle_bgeu(struct InstructionStructure* inst) {
    if(registers[inst->rs1] >= registers[inst->rs2]) {
        program_counter += inst->imm;
    }
}

// Remember memory is little-endian
void handle_lb(struct InstructionStructure* inst) {
    // We need memory defined so i commented some things
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint8_t byte; //= memory[address];

    registers[inst->rd] = (int32_t)(int8_t)byte;
}

void handle_lh(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint8_t byte0; // = memory[address];
    uint8_t byte1; // = memory[byte1];
    uint16_t halfword = (byte1 << 8) | byte0;

    registers[inst->rd] = (int32_t)(int16_t)halfword;
}

void handle_lw(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint8_t byte0; // = memory[address];
    uint8_t byte1; //= memory[address + 1];
    uint8_t byte2; //= memory[address + 2];
    uint8_t byte3; //= memory[address + 3];

    registers[inst->rd] = (byte3 << 24) | (byte2 << 16) | (byte1 << 8) | byte0;
}

void handle_lbu(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint8_t byte; //= memory[address];

    registers[inst->rd] = byte;
}

void handle_lhu(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint8_t byte0; // = memory[address];
    uint8_t byte1; // = memory[byte1];
    uint16_t halfword = (byte1 << 8) | byte0;

    registers[inst->rd] = halfword;
}

void handle_sb(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;

    //memory[address] = (uint8_t)registers[inst->rs2];
}

void handle_sh(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint16_t halfword = (uint16_t)registers[inst->rs2];

    //memory[address] = (uint8_t)(halfword & 0xFF);
    //memory[address + 1] = (uint8_t)((halfword >> 8) & 0xFF);
}

void handle_sw(struct InstructionStructure* inst) {
    uint32_t address = registers[inst->rs1] + inst->imm;
    uint32_t word = registers[inst->rs2];

    //memory[address] = (uint8_t)(word & 0xFF);
    //memory[address + 1] = (uint8_t)((word >> 8) & 0xFF);
    //memory[address + 2] = (uint8_t)((word >> 16) & 0xFF);
    //memory[address + 3] = (uint8_t)((word >> 24) & 0xFF);
}

void handle_addi(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] + inst->imm;
}

void handle_slti(struct InstructionStructure* inst) {
    registers[inst->rd] = ((int32_t)registers[inst->rs1] < (int32_t)inst->imm) ? 1 : 0;
}

void handle_sltiu(struct InstructionStructure* inst) {
    registers[inst->rd] = (registers[inst->rs1] < inst->imm) ? 1 : 0;
}

void handle_xori(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] ^ inst->imm;
}

void handle_ori(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] | inst->imm;
}

void handle_andi(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] & inst->imm;
}

void handle_slli(struct InstructionStructure* inst) {
    uint32_t shamt = inst->rs2;
    registers[inst->rd] = registers[inst->rs1] << shamt;
}

void handle_srli(struct InstructionStructure* inst) {
    uint32_t shamt = inst->rs2;
    registers[inst->rd] = registers[inst->rs1] >> shamt;
}

void handle_srai(struct InstructionStructure* inst) {
    uint32_t shamt = inst->rs2;
    registers[inst->rd] = (int32_t)registers[inst->rs1] >> shamt;
}

void handle_add(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] + registers[inst->rs2];
}

void handle_sub(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] - registers[inst->rs2];
}

void handle_sll(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] << registers[inst->rs2];
}

void handle_slt(struct InstructionStructure* inst) {
    registers[inst->rd] = ((int32_t)registers[inst->rs1] < (int32_t)registers[inst->rs2]) ? 1 : 0;
}

void handle_sltu(struct InstructionStructure* inst) {
    registers[inst->rd] = (registers[inst->rs1] < registers[inst->rs2]) ? 1 : 0;
}

void handle_xor(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] ^ registers[inst->rs2];
}

void handle_srl(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] >> registers[inst->rs2];
}

void handle_sra(struct InstructionStructure* inst) {
    registers[inst->rd] = (int32_t)registers[inst->rs1] >> (int32_t)registers[inst->rs2];
}

void handle_or(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] | registers[inst->rs2];
}

void handle_and(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] & registers[inst->rs2];
}

// To be implemented later
void handle_fence(struct InstructionStructure* inst) {}

void handle_fencei(struct InstructionStructure* inst) {}

void handle_ecall(struct InstructionStructure* inst) {}

void handle_ebreak(struct InstructionStructure* inst) {}

void handle_undefined(struct InstructionStructure* inst) {}
