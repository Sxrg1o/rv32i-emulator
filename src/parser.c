#include "parser.h"

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
    // TODO: Just Fence, Fencei, Ecall & Ebreak left... these need the whole instruction
    return handle_undefined;
}

InstructionStructure get_structure(uint32_t instruction) {
    InstructionStructure structure;
    structure.instruction = instruction;

    structure.opcode    =   instruction & OPCODE_MASK;
    structure.rd        =   (instruction >> 7) & 0x1F;
    structure.funct3    =   (instruction >> 12) & 0x07;
    structure.rs1       =   (instruction >> 15) & 0x07;
    structure.rs2       =   (instruction >> 20) & 0x1F;
    structure.funct7    =   (instruction >> 25) & 0x7F;

    // TODO: We are double checking, find a way to optimise this :)))))))))))))))
    structure.type = get_type(structure.opcode, structure.funct3);

    switch(structure.type) {
        case RTYPE: break;
        case UTYPE:
            structure.imm = instruction & 0xFFFFF000;
            break;
        // TODO: Missing types to get immediates
        // Check ISA-unprivileged chapter 2
        // Remember to handle pred, succ from fence...
        // Maybe union in InstructionStructure? idk this way is easier xdxdx

        default: break;
    }

    structure.handler = get_handler(structure.instruction, structure.opcode, structure.funct3, structure.funct7);

    return structure;
}
