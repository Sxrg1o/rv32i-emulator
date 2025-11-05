#include "parser.h"
#include "instructions.h"

#define OPCODE_MASK 0x0000007F

enum OpcodeType get_type(uint8_t opcode, uint8_t funct3) {
    switch (opcode) {
        case 0b0110011: return RTYPE;
        case 0b0100011: return STYPE;
        case 0b1100011: return BTYPE;
        case 0b0000011: return ITYPE;
        case 0b0001111: return FENCE;
        case 0b0010011:
            if(funct3 == 0x1 || funct3 == 0x5) {
                return RTYPE;
            } else return ITYPE;
            break;
        case 0b1110011: return ENV;
        case 0b0110111: return UTYPE;
        case 0b0010111: return UTYPE;
        case 0b1101111: return JTYPE;
        case 0b1100111: return ITYPE;
        default: return UNDEF;
    }
}

static InstructionHandler get_handler(uint32_t instruction, uint8_t opcode, uint8_t funct3, uint8_t funct7) {
    switch (opcode) {
        case 0b0110011:
            switch(funct3) {
                case 0x0:
                    if(funct7 == 0x00) return handle_add;
                    if(funct7 == 0x20) return handle_sub;
                    break;
                case 0x1:
                    if(funct7 == 0x00) return handle_sll;
                    break;
                case 0x2:
                    if(funct7 == 0x00) return handle_slt;
                    break;
                case 0x3:
                    if(funct7 == 0x00) return handle_sltu;
                    break;
                case 0x4:
                    if(funct7 == 0x00) return handle_xor;
                    break;
                case 0x5:
                    if(funct7 == 0x00) return handle_srl;
                    if(funct7 == 0x20) return handle_sra;
                    break;
                case 0x6:
                    if(funct7 == 0x00) return handle_or;
                    break;
                case 0x7:
                    if(funct7 == 0x00) return handle_and;
                    break;
            }
            break;
        case 0b0010011:
            switch(funct3) {
                case 0x0: return handle_addi;
                case 0x1:
                    if(funct7 == 0x00) return handle_slli;
                    break;
                case 0x2: return handle_slti;
                case 0x3: return handle_sltiu;
                case 0x4: return handle_xori;
                case 0x5:
                    if(funct7 == 0x00) return handle_srli;
                    if(funct7 == 0x20) return handle_srai;
                    break;
                case 0x6: return handle_ori;
                case 0x7: return handle_andi;
            }
        case 0b0000011:
            switch(funct3) {
                case 0x0: return handle_lb;
                case 0x1: return handle_lh;
                case 0x2: return handle_lw;
                case 0x4: return handle_lbu;
                case 0x5: return handle_lhu;
            }
        case 0b0100011:
            switch(funct3) {
                case 0x0: return handle_sb;
                case 0x1: return handle_sh;
                case 0x2: return handle_sw;
            }
        case 0b1100011:
            switch(funct3) {
                case 0x0: return handle_beq;
                case 0x1: return handle_bne;
                case 0x4: return handle_blt;
                case 0x5: return handle_bge;
                case 0x6: return handle_bltu;
                case 0x7: return handle_bgeu;
            }
        case 0b1100111:
            if(funct3 == 0x0) return handle_jalr;
            break;
        case 0b1101111: return handle_jal;
        case 0b0010111: return handle_auipc;
        case 0b0110111: return handle_lui;
    }
    if(instruction == 0x00100073) return handle_ebreak;
    if(instruction == 0x00000073) return handle_ecall;
    if(instruction == 0x0000100F) return handle_fencei;
    if((instruction & 0xF00FFFFF) == 0x0000000F) return handle_fence;
    return handle_undefined;
}

InstructionStructure get_structure(uint32_t instruction) {
    InstructionStructure structure;
    structure.instruction = instruction;

    structure.opcode    =   instruction & OPCODE_MASK;
    structure.funct3    =   (instruction >> 12) & 0x07;         // Most of them have this so ig its better this way?

    structure.type = get_type(structure.opcode, structure.funct3);

    switch(structure.type) {
        case RTYPE:
            structure.rd        =   (instruction >> 7) & 0x1F;
            structure.rs1       =   (instruction >> 15) & 0x1F;
            structure.rs2       =   (instruction >> 20) & 0x1F;
            structure.funct7    =   (instruction >> 25) & 0x7F;
            break;
        case UTYPE:
            structure.rd        =   (instruction >> 7) & 0x1F;
            structure.imm       =   instruction & 0xFFFFF000;
            break;
        case ITYPE:
            structure.rd        =   (instruction >> 7) & 0x1F;
            structure.rs1       =   (instruction >> 15) & 0x1F;
            structure.imm       =   (int32_t)(instruction & 0xFFF00000) >> 20;
            break;
        case STYPE:
            structure.rs1       =   (instruction >> 15) & 0x1F;
            structure.rs2       =   (instruction >> 20) & 0x1F;
            uint32_t im_11_5    =   (instruction & 0xFE000000) >> 20;
            uint32_t im_4_0     =   (instruction & 0x00000F80) >> 7;
            uint32_t imm_s      =   im_11_5 | im_4_0;
            structure.imm       =   (int32_t)(imm_s << 20) >> 20;
            break;
        case BTYPE:
            structure.rs1       =   (instruction >> 15) & 0x1F;
            structure.rs2       =   (instruction >> 20) & 0x1F;
            uint32_t im_12      =   (instruction & 0x80000000) >> 19;
            uint32_t im_11      =   (instruction & 0x00000080) << 4;
            uint32_t im_10_5    =   (instruction & 0x7E000000) >> 20;
            uint32_t im_4_1     =   (instruction & 0x00000F00) >> 7;
            uint32_t imm_b      =   im_12 | im_11 | im_10_5 | im_4_1;
            structure.imm       =   (int32_t)(imm_b << 19) >> 19;
            break;
        case JTYPE:
            structure.rd        =   (instruction >> 7) & 0x1F;
            uint32_t im_20      =   (instruction & 0x80000000) >> 11;
            uint32_t im_10_1    =   (instruction & 0x7FE00000) >> 20;
            uint32_t im_11_j    =   (instruction & 0x00100000) >> 9;
            uint32_t im_19_12   =   (instruction & 0x000FF000);
            uint32_t imm_j      =   im_20 | im_19_12 | im_11_j | im_10_1;
            structure.imm       =   (int32_t)(imm_j << 11) >> 11;
            break;
        case FENCE:
            structure.pred      =   (instruction & 0x0F000000) >> 24;
            structure.succ      =   (instruction & 0x00F00000) >> 20;
            break;
        default: break;
    }

    // TODO: We are double checking, find a way to optimise this :)))))))))))))))
    structure.handler = get_handler(structure.instruction, structure.opcode, structure.funct3, structure.funct7);

    return structure;
}
