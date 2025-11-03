#include "instructions.h"
#include "state.h"
#include "parser.h"

void handle_lui(struct InstructionStructure* inst) {}
void handle_auipc(struct InstructionStructure* inst) {}
void handle_jal(struct InstructionStructure* inst) {}
void handle_jalr(struct InstructionStructure* inst) {}
void handle_beq(struct InstructionStructure* inst) {}
void handle_bne(struct InstructionStructure* inst) {}
void handle_blt(struct InstructionStructure* inst) {}
void handle_bge(struct InstructionStructure* inst) {}
void handle_bltu(struct InstructionStructure* inst) {}
void handle_bgeu(struct InstructionStructure* inst) {}
void handle_lb(struct InstructionStructure* inst) {}
void handle_lh(struct InstructionStructure* inst) {}
void handle_lw(struct InstructionStructure* inst) {}
void handle_lbu(struct InstructionStructure* inst) {}
void handle_lhu(struct InstructionStructure* inst) {}
void handle_sb(struct InstructionStructure* inst) {}
void handle_sh(struct InstructionStructure* inst) {}
void handle_sw(struct InstructionStructure* inst) {}
void handle_addi(struct InstructionStructure* inst) {}
void handle_slti(struct InstructionStructure* inst) {}
void handle_sltiu(struct InstructionStructure* inst) {}
void handle_xori(struct InstructionStructure* inst) {}
void handle_ori(struct InstructionStructure* inst) {}
void handle_andi(struct InstructionStructure* inst) {}
void handle_slli(struct InstructionStructure* inst) {}
void handle_srli(struct InstructionStructure* inst) {}
void handle_srai(struct InstructionStructure* inst) {}

void handle_add(struct InstructionStructure* inst) {
    registers[inst->rd] = registers[inst->rs1] + registers[inst->rs2];
}

void handle_sub(struct InstructionStructure* inst) {}
void handle_sll(struct InstructionStructure* inst) {}
void handle_slt(struct InstructionStructure* inst) {}
void handle_sltu(struct InstructionStructure* inst) {}
void handle_xor(struct InstructionStructure* inst) {}
void handle_srl(struct InstructionStructure* inst) {}
void handle_sra(struct InstructionStructure* inst) {}
void handle_or(struct InstructionStructure* inst) {}
void handle_and(struct InstructionStructure* inst) {}
void handle_fence(struct InstructionStructure* inst) {}
void handle_fencei(struct InstructionStructure* inst) {}
void handle_ecall(struct InstructionStructure* inst) {}
void handle_ebreak(struct InstructionStructure* inst) {}
void handle_undefined(struct InstructionStructure* inst) {}
