#pragma once

#include "instructions.h"
#include <stdint.h>

enum OpcodeType {
  RTYPE,        // Register to register
  ITYPE,        // Immediate and load
  STYPE,        // Store instructions
  BTYPE,        // Branch operations
  UTYPE,        // Upper immediate
  JTYPE,        // Unconditional jump instructions
  ENV,          // Environment calls (check ECALL, EBREAK)
  FENCE,        // Fence
  UNDEF         // Undefined
};

typedef struct InstructionStructure {
  enum OpcodeType type;
  InstructionHandler handler;
  uint32_t instruction;

  uint8_t rs1;      // 5 bits
  uint8_t rs2;      // 5 bits
  uint8_t rd;       // 5 bits
  uint32_t imm;     // at most 20 bits
  uint8_t pred;     // For fence
  uint8_t succ;     // For fence

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
 */

// Just getting the type, check up here before and refine it before implementing
enum OpcodeType get_type(uint8_t, uint8_t);

// Getting all structure (inside here we use get_type())
InstructionStructure get_structure(uint32_t);
