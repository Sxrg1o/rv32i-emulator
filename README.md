# RISC - V Emulator

RV32I emulator that loads and executes N 'processes' in User Mode (U-Mode) managing concurrency and virtual memory mapping.

---

## Scope (To be implemented)

* **MVP**
    * Complete base ISA implementation
    * x0-x31 general purpose registers and program counter
    * Full M-mode
    * Load ELF binary into memory
 * **Privileges & Memory Architecture**
    * Sync traps for `ecall`, misaligned access and page faults
    * Control and Status Registers for trap handling and memory management (`mstatus`, `mepc`, `mcause`, `mtvec`, `satp`)
    * MMU implementation (including TLB)
    * Implement privilege transition (use `mret`)
* **Concurrency**
    * Minimal CLINT for timed interrupts
    * MTI for async traps
    * Include Control and Status Registers for machine interruptions (`mie` & `mip`)
    * RR Scheduler implementation using the MTI trap handler
    * Handle context switch using a PCB and the `satp` register

---

## Performance scope (To be checked)

* **Pipeline:** Use a 5-stage pipeline. Forwarding logic between pipeline stages is needed. Then, we need pipeline stalling and flushing (maybe branch prediction?).

* **Memory hierarchy:**
    * **Cache:** Tag, index, offset. Cache policies.
    * **TLB:** Just a MMU cache. We need flushing logic when context switching.

---

## Sources and testing (Important)

We need strict testing at the lowest level possible. So, we need to check [this](https://github.com/riscv-software-src/riscv-tests/tree/master/isa). Each instruction must be validated before its integration into the pipeline model. If not, debugging will be impossible :(

All information needed is at:

* [RISC-V Unprivileged Architecture](https://docs.riscv.org/reference/isa/_attachments/riscv-unprivileged.pdf)
* [RISC-V Privileged Architecture](https://docs.riscv.org/reference/isa/_attachments/riscv-privileged.pdf)
* [Opcode Information](https://www.cs.unh.edu/~pjh/courses/cs520/15spr/riscv-rv32i-instructions.pdf)
---

## Out of Scope

* ISA Extensions
* Remember this is a 32 bit risc-v emulator
* Supervisor mode, we'll just use M and U mode at its simplest

---

## Extra? (To be discussed)

Maybe a simple visor to see register status, pipeline stages, interrupts, memory hierarchy and mapping, scheduling, etc.
