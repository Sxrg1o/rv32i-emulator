#include "parser.h"
#include "instructions.h"

#define OPCODE_MASK 0x0000007F

InstructionStructure get_structure(uint32_t instruction) {
    InstructionStructure structure;

    uint8_t opcode   = instruction & OPCODE_MASK;
    uint8_t rd       = (instruction >> 7) & 0x1F;
    uint8_t funct3   = (instruction >> 12) & 0x07;
    uint8_t rs1      = (instruction >> 15) & 0x1F;
    uint8_t rs2      = (instruction >> 20) & 0x1F;
    uint8_t funct7   = (instruction >> 25) & 0x7F;

    structure.type = UNDEF;
    structure.handler = handle_undefined;

    switch (opcode) {

        case 0b0110011:
            structure.type = RTYPE;
            structure.rd = rd;
            structure.rs1 = rs1;
            structure.rs2 = rs2;

            switch(funct3) {
                case 0x0:
                    if(funct7 == 0x00) structure.handler = handle_add;
                    else if(funct7 == 0x20) structure.handler = handle_sub;
                    break;
                case 0x1: if(funct7 == 0x00) structure.handler = handle_sll; break;
                case 0x2: if(funct7 == 0x00) structure.handler = handle_slt; break;
                case 0x3: if(funct7 == 0x00) structure.handler = handle_sltu; break;
                case 0x4: if(funct7 == 0x00) structure.handler = handle_xor; break;
                case 0x5:
                    if(funct7 == 0x00) structure.handler = handle_srl;
                    else if(funct7 == 0x20) structure.handler = handle_sra;
                    break;
                case 0x6: if(funct7 == 0x00) structure.handler = handle_or; break;
                case 0x7: if(funct7 == 0x00) structure.handler = handle_and; break;
            }
            break;

        case 0b0010011:
            structure.type = ITYPE;
            structure.rd = rd;
            structure.rs1 = rs1;
            structure.imm = (int32_t)(instruction & 0xFFF00000) >> 20;

            switch(funct3) {
                case 0x0: structure.handler = handle_addi; break;
                case 0x1:
                    if(funct7 == 0x00) {
                        structure.rs2 = rs2;
                        structure.handler = handle_slli;
                        structure.type = RTYPE;
                    } else {
                        structure.handler = handle_undefined;
                        structure.type = UNDEF;
                    }
                    break;
                case 0x2: structure.handler = handle_slti; break;
                case 0x3: structure.handler = handle_sltiu; break;
                case 0x4: structure.handler = handle_xori; break;
                case 0x5:
                    if(funct7 == 0x00) {
                        structure.rs2 = rs2;
                        structure.handler = handle_srli;
                        structure.type = RTYPE;
                    } else if(funct7 == 0x20) {
                        structure.rs2 = rs2;
                        structure.handler = handle_srai;
                        structure.type = RTYPE;
                    } else {
                        structure.handler = handle_undefined;
                        structure.type = UNDEF;
                    }
                    break;
                case 0x6: structure.handler = handle_ori; break;
                case 0x7: structure.handler = handle_andi; break;
            }
            break;

        case 0b0000011:
            structure.type = ITYPE;
            structure.rd = rd;
            structure.rs1 = rs1;
            structure.imm = (int32_t)(instruction & 0xFFF00000) >> 20;

            switch(funct3) {
                case 0x0: structure.handler = handle_lb; break;
                case 0x1: structure.handler = handle_lh; break;
                case 0x2: structure.handler = handle_lw; break;
                case 0x4: structure.handler = handle_lbu; break;
                case 0x5: structure.handler = handle_lhu; break;
                default:
                    structure.handler = handle_undefined;
                    structure.type = UNDEF;
                    break;
            }
            break;

        case 0b1100111:
            if(funct3 == 0x0) {
                structure.type = ITYPE;
                structure.rd = rd;
                structure.rs1 = rs1;
                structure.imm = (int32_t)(instruction & 0xFFF00000) >> 20;
                structure.handler = handle_jalr;
            }
            break;

        case 0b0100011:
            structure.type = STYPE;
            structure.rs1 = rs1;
            structure.rs2 = rs2;
            uint32_t im_11_5 = (instruction & 0xFE000000) >> 20;
            uint32_t im_4_0  = (instruction & 0x00000F80) >> 7;
            structure.imm = (int32_t)((im_11_5 | im_4_0) << 20) >> 20;

            switch(funct3) {
                case 0x0: structure.handler = handle_sb; break;
                case 0x1: structure.handler = handle_sh; break;
                case 0x2: structure.handler = handle_sw; break;
                default:
                    structure.handler = handle_undefined;
                    structure.type = UNDEF;
                    break;
            }
            break;

        case 0b1100011:
            structure.type = BTYPE;
            structure.rs1 = rs1;
            structure.rs2 = rs2;
            uint32_t im_12   = (instruction & 0x80000000) >> 19;
            uint32_t im_11   = (instruction & 0x00000080) << 4;
            uint32_t im_10_5 = (instruction & 0x7E000000) >> 20;
            uint32_t im_4_1  = (instruction & 0x00000F00) >> 7;
            structure.imm = (int32_t)((im_12 | im_11 | im_10_5 | im_4_1) << 19) >> 19;

            switch(funct3) {
                case 0x0: structure.handler = handle_beq; break;
                case 0x1: structure.handler = handle_bne; break;
                case 0x4: structure.handler = handle_blt; break;
                case 0x5: structure.handler = handle_bge; break;
                case 0x6: structure.handler = handle_bltu; break;
                case 0x7: structure.handler = handle_bgeu; break;
                default:
                    structure.handler = handle_undefined;
                    structure.type = UNDEF;
                    break;
            }
            break;

        case 0b0110111:
            structure.type = UTYPE;
            structure.rd = rd;
            structure.imm = instruction & 0xFFFFF000;
            structure.handler = handle_lui;
            break;

        case 0b0010111:
            structure.type = UTYPE;
            structure.rd = rd;
            structure.imm = instruction & 0xFFFFF000;
            structure.handler = handle_auipc;
            break;

        case 0b1101111:
            structure.type = JTYPE;
            structure.rd = rd;
            uint32_t im_20   = (instruction & 0x80000000) >> 11;
            uint32_t im_10_1 = (instruction & 0x7FE00000) >> 20;
            uint32_t im_11_j = (instruction & 0x00100000) >> 9;
            uint32_t im_19_12= (instruction & 0x000FF000);
            structure.imm = (int32_t)((im_20 | im_19_12 | im_11_j | im_10_1) << 11) >> 11;
            structure.handler = handle_jal;
            break;

        case 0b0001111:
            if(rs1 == 0 && rd == 0) {
                uint32_t imm12 = (instruction >> 20);
                if(funct3 == 0x0 && (imm12 & 0xF00) == 0x000) {
                    structure.type = FENCE;
                    structure.pred = (instruction & 0x0F000000) >> 24;
                    structure.succ = (instruction & 0x00F00000) >> 20;
                    structure.handler = handle_fence;
                } else if(funct3 == 0x1 && (imm12 & 0xFFF) == 0x000) {
                    structure.type = FENCE;
                    structure.handler = handle_fencei;
                }
            }
            break;

        case 0b1110011:
            structure.type = ENV;
            if (instruction == 0x00000073) structure.handler = handle_ecall;
            else if (instruction == 0x00100073) structure.handler = handle_ebreak;
            else structure.type = UNDEF;
            break;
    }

    return structure;
}
