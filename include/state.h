#pragma once

#include <stdint.h>

/*  All rv32i state goes here  */

extern uint32_t registers[32];      // General purpose registers
extern uint32_t program_counter;
extern uint8_t* ram;
