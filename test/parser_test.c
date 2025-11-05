#include "instructions.h"
#include "parser.h"
#include "test_utils.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Test R-Type: add x5, x10, x20
 * Bin: 0000000 10100 01010 000 00101 0110011
 * Hex: 0x014502B3
 */
bool test_rtype_add() {
    uint32_t instruction_hex = 0x014502B3;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == RTYPE);
    ASSERT(structure.opcode == 0b0110011);
    ASSERT(structure.rd == 5);
    ASSERT(structure.funct3 == 0x0);
    ASSERT(structure.rs1 == 10);
    ASSERT(structure.rs2 == 20);
    ASSERT(structure.funct7 == 0x00);
    ASSERT(structure.handler == handle_add);

    return true;
}

/**
 * Test I-Type with signed imm: addi x5, x10, -50
 * imm = -50 = 0xFFFFFFCE -> 12 bits: 0xFCE
 * Bin: 111111001110 01010 000 00101 0010011
 * Hex: 0xFCE50293
 */
bool test_itype_addi() {
    uint32_t instruction_hex = 0xFCE50293;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == ITYPE);
    ASSERT(structure.opcode == 0b0010011);
    ASSERT(structure.rd == 5);
    ASSERT(structure.funct3 == 0x0);
    ASSERT(structure.rs1 == 10);
    ASSERT(structure.imm == -50);
    ASSERT(structure.handler == handle_addi);

    return true;
}

/**
 * Test SLLI (shamt, but R-TYPE): slli x5, x10, 4
 * Bin: 0000000 00100 01010 001 00101 0010011
 * Hex: 0x00451293
 */
bool test_itype_shift_slli() {
    uint32_t instruction_hex = 0x00451293;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == RTYPE);
    ASSERT(structure.opcode == 0b0010011);
    ASSERT(structure.rd == 5);
    ASSERT(structure.funct3 == 0x1);
    ASSERT(structure.rs1 == 10);
    ASSERT(structure.rs2 == 4);
    ASSERT(structure.funct7 == 0x00);
    ASSERT(structure.handler == handle_slli);

    return true;
}

/**
 * @brief Test S-TYPE with signed imm: sw x20, -12(x10)
 * imm = -12 -> imm[11:5] = 0b1111111, imm[4:0] = 0b10100
 * Bin: 1111111 10100 01010 010 10100 0100011
 * Hex: 0xFF452A23
 */
bool test_stype_sw() {
    uint32_t instruction_hex = 0xFF452A23;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == STYPE);
    ASSERT(structure.opcode == 0b0100011);
    ASSERT(structure.funct3 == 0x2);
    ASSERT(structure.rs1 == 10);
    ASSERT(structure.rs2 == 20);
    ASSERT(structure.handler == handle_sw);
    ASSERT(structure.imm == -12);

    return true;
}

/**
 * Test B-Type with signed imm: beq x10, x20, -16
 * imm = -16 -> imm[12]=1, imm[11]=1, imm[10:5]=111111, imm[4:1]=1000
 * Bin: 1 111111 10100 01010 000 1000 1 1100011
 * Hex: 0xFF4508E3
 */
bool test_btype_beq() {
    uint32_t instruction_hex = 0xFF4508E3;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == BTYPE);
    ASSERT(structure.opcode == 0b1100011);
    ASSERT(structure.funct3 == 0x0);
    ASSERT(structure.rs1 == 10);
    ASSERT(structure.rs2 == 20);
    ASSERT(structure.handler == handle_beq);
    ASSERT(structure.imm == -16);

    return true;
}

/**
 * Test U-Type: lui x5, 0xABCDE
 * imm = 0xABCDE000
 * Bin: 10101011110011011110 00101 0110111
 * Hex: 0xABCDE2B7
 */
bool test_utype_lui() {
    uint32_t instruction_hex = 0xABCDE2B7;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == UTYPE);
    ASSERT(structure.opcode == 0b0110111);
    ASSERT(structure.rd == 5);
    ASSERT(structure.imm == 0xABCDE000);
    ASSERT(structure.handler == handle_lui);

    return true;
}

/**
 * Test J-Type with signed imm: jal x5, -32
 * imm = -32 -> imm[20]=1, imm[19:12]=11111111, imm[11]=1, imm[10:1]=11100
 * Bin: 1 1110000000 1 11111111 00101 1101111
 * Hex: 0xFE1FF2EF
 */
bool test_jtype_jal() {
    uint32_t instruction_hex = 0xFE1FF2EF;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == JTYPE);
    ASSERT(structure.opcode == 0b1101111);
    ASSERT(structure.rd == 5);
    ASSERT(structure.handler == handle_jal);
    ASSERT(structure.imm == -32);

    return true;
}

/**
 * Test env call
 * Hex: 0x00100073
 */
bool test_special_ebreak() {
    uint32_t instruction_hex = 0x00100073;
    InstructionStructure structure = get_structure(instruction_hex);

    ASSERT(structure.type == ENV);
    ASSERT(structure.handler == handle_ebreak);

    return true;
}

/**
 * Test undefined
 * Bin: 1010 1011 1100 1101 1110 1111 0 0010010
 */
 bool test_undefined() {
     uint32_t instruction_hex = 0xABCDEF12;
     InstructionStructure structure = get_structure(instruction_hex);

     ASSERT(structure.type == UNDEF);
     ASSERT(structure.handler == handle_undefined);

     return true;
 }

int test_parser() {
    printf("Starting tests for parser\n\n");
    INIT_TEST_MODULE(parser);

    RUN_TEST(parser, test_rtype_add);
    RUN_TEST(parser, test_itype_addi);
    RUN_TEST(parser, test_itype_shift_slli);
    RUN_TEST(parser, test_utype_lui);
    RUN_TEST(parser, test_stype_sw);
    RUN_TEST(parser, test_btype_beq);
    RUN_TEST(parser, test_jtype_jal);
    RUN_TEST(parser, test_special_ebreak);
    RUN_TEST(parser, test_undefined);

    PRINT_TEST_SUMMARY(parser);
    return parser_total_tests - parser_successful_tests;
}
