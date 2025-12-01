#pragma once

#include <stdint.h>

// As we are emulating 32 bits, we just use 32 bits ELF
typedef struct {
    uint32_t ei_mag;        // Magic number [7F 45 4C 46]
    uint8_t ei_class;       // 1 or 2 (Always 1 bc 32 bits)
    uint8_t ei_data;        // 1 or 2 (1 Little endian)
    uint8_t ei_version;     // Just 1
    uint8_t ei_osabi;       // Target OS (System V so 0x00)
    uint8_t ei_abiversion;  // Ignore for now ig
    uint8_t padding[7];
    uint16_t e_type;        // Ig executable file so 0x02
    uint16_t e_machine;     // Must check if 0xF3
    uint32_t e_version;     // Just 1
    uint32_t e_entry;       // Entry point memory address
    uint32_t e_phoff;       // Start of program header table
    uint32_t e_shoff;       // Start of section header table
    uint32_t e_flags;       // TODO: Check this field for RISC-V
    uint16_t e_ehsize;      // Size of the header (52 bytes)
    uint16_t e_phentsize;   // Size of PH table entry 
    uint16_t e_phnum;       // Number of entries in PH table
    uint16_t e_shentsize;   // Size of SH table entry 
    uint16_t e_shnum;       // Number of entries in SH table
    uint16_t e_shstrndx;    // Index of SH table entry with sections names
} ELFHeader;

typedef struct {
    uint32_t p_type;        // For now, just use 0x01 (Loadable segments)
    uint32_t p_offset;      // Offset of the segment
    uint32_t p_vaddr;       // Virtual address of segment
    uint32_t p_paddr;       // Physical address of segment
    uint32_t p_filesz;      // Size of segment in file
    uint32_t p_memz;        // Size of segment in memory
    uint32_t p_flags;       // 0x1 Executable, 0x2 Writeable, 0x4 Readable
    uint32_t p_align;       // TODO: Check this, idk what is this :)
} ProgramHeader;

// Check this link: https://refspecs.linuxbase.org/elf/gabi4+/ch5.pheader.html

int load_elf(const char *);