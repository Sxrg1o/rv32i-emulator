#pragma once

struct InstructionStructure;

typedef void (*InstructionHandler)(struct InstructionStructure*);

void handle_lui(struct InstructionStructure*);
void handle_auipc(struct InstructionStructure*);
void handle_jal(struct InstructionStructure*);
void handle_jalr(struct InstructionStructure*);
void handle_beq(struct InstructionStructure*);
void handle_bne(struct InstructionStructure*);
void handle_blt(struct InstructionStructure*);
void handle_bge(struct InstructionStructure*);
void handle_bltu(struct InstructionStructure*);
void handle_bgeu(struct InstructionStructure*);
void handle_lb(struct InstructionStructure*);
void handle_lh(struct InstructionStructure*);
void handle_lw(struct InstructionStructure*);
void handle_lbu(struct InstructionStructure*);
void handle_lhu(struct InstructionStructure*);
void handle_sb(struct InstructionStructure*);
void handle_sh(struct InstructionStructure*);
void handle_sw(struct InstructionStructure*);
void handle_addi(struct InstructionStructure*);
void handle_slti(struct InstructionStructure*);
void handle_sltiu(struct InstructionStructure*);
void handle_xori(struct InstructionStructure*);
void handle_ori(struct InstructionStructure*);
void handle_andi(struct InstructionStructure*);
void handle_slli(struct InstructionStructure*);
void handle_srli(struct InstructionStructure*);
void handle_srai(struct InstructionStructure*);
void handle_add(struct InstructionStructure*);
void handle_sub(struct InstructionStructure*);
void handle_sll(struct InstructionStructure*);
void handle_slt(struct InstructionStructure*);
void handle_sltu(struct InstructionStructure*);
void handle_xor(struct InstructionStructure*);
void handle_srl(struct InstructionStructure*);
void handle_sra(struct InstructionStructure*);
void handle_or(struct InstructionStructure*);
void handle_and(struct InstructionStructure*);
void handle_fence(struct InstructionStructure*);
void handle_fencei(struct InstructionStructure*);
void handle_ecall(struct InstructionStructure*);
void handle_ebreak(struct InstructionStructure*);
void handle_undefined(struct InstructionStructure*);
