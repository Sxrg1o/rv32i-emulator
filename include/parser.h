#pragma once

#include <stdint.h>

// We'll need masks

enum OpcodeType {
  RTYPE,        // Register to register
  ITYPE,        // Immediate and load
  STYPE,        // Store instructions
  BTYPE,        // Branch operations
  UTYPE,        // Upper immediate
  JTYPE,        // Unconditional jump instructions
  ENV,          // Environment calls (check FENCE, ECALL, EBREAK) Check later if we need some other like RDTIME, RDCYCLE, etc
  UNDEF         // Undefined
};

typedef struct {
  enum OpcodeType type;
  uint32_t instruction;

  uint8_t rs1;      // 5 bits
  uint8_t rs2;      // 5 bits
  uint8_t rd;       // 5 bits
  uint32_t imm;     // at most 20 bits

  uint8_t funct7;   // 7 bits
  uint8_t funct3;   // 3 bits
  uint8_t opcode;   // 7 bits
} InstructionStructure;

/*
 * R-TYPE   funct7       -   rs2     -   rs1     -   funct3  -   rd       -   opcode
 * I-TYPE   imm[11:0]    -   rs1     -   funct3  -   rd      -   opcode
 * S-TYPE   imm[11:5]    -   rs2     -   rs1     -   funct3  -   imm[4:0] -   opcode
 * B-TYPE   imm[12|10:5] -   rd      -   opcode
 * U-TYPE   imm[31:12]   -   rd      -   opcode
 * J-TYPE   imm[20|10:1|11|19:12]    -   rd      -   opcode
 *
 * Remember it's little endian!!!!1 and this needs a more rigorous check for undefined opcodes it's kinda fast-ugly done xd
 *
 * 0 - 1 bits always are set
 * if bit 2 is set then:
 *      if bit 4 is set:
 *          is U-TYPE
 *      elif bit 3 is set:
 *          if bits 5 and 6 are set:
 *              is J-TYPE
 *          else:
 *              is ENV      this are fence instructions
 *      else:       not so sure about this
 *          is I-TYPE
 * elif bit 5 is set:
 *      if bit 6 is set:
 *          if bit 4 is set:
 *              is ENV      this are ecall, ebreak and other instructions that use just one register
 *          else:
 *              is B-TYPE
 *      elif bit 4 is set:
 *          is R-TYPE
 *      else:
 *          is S-TYPE
 * elif bit 4 is set:
 *      if funct3 is (001 | 101):
 *          is R-TYPE   BUT rs2 is not a register, is an immediate
 *      else:
 *          is I-TYPE
 * elif bit 3 is not set:
 *      is I-TYPE
 * else:
 *      UNDEFINED
 *
 */

// Just getting the type, check up here before and refine it before implementing
enum OpcodeType get_type(uint8_t);

// Getting all structure (inside here we use get_type())
InstructionStructure get_structure(uint32_t);
