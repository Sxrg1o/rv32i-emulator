# RISC-V System Specification (RV32I, M/S/U Architecture)

This project implements a *vertical slice* of a 32-bit RISC-V computer system, consisting of:

1. **Hardware Emulator**: a functional RV32I CPU with Sv32 virtual memory and a minimal device model.
2. **Machine-Mode Firmware (SBI Subset)**: an OpenSBI-inspired runtime providing boot services and M-mode trap handling.
3. **Supervisor-Mode Kernel**: a monolithic UNIX-like OS with paging, scheduling, and syscalls.
4. **User-Space Runtime**: a custom syscall ABI and minimal C standard library subset.

---

# Project Phases

---

## Phase 1 Bare-Metal Execution (RV32I, M-Mode Only)

**Objective:** Implement the core CPU pipeline and enough of the Machine-level environment to run bare-metal ELF binaries.

### Hardware Emulator Specification

- **ISA Support:**
  - RV32I Base Integer ISA only.
  - No M-extension, no C-extension.

- **Privilege Model:**
  - M-Mode only (no S/U mode).

- **Physical Memory Map:**
  - `0x8000_0000` DRAM base  
  - 64 MiB DRAM  
  - `0x1000_0000` reserved MMIO base

- **CSRs Implemented (Minimal):**
  - Identity: `mhartid`, `mvendorid`, `marchid`, `mimpid`
  - ISA/Status: `misa` (RV32I), `mstatus` (MIE/MPIE)
  - Trap: `mtvec`, `mepc`, `mcause`, `mtval`, `mscratch`

- **Exceptions Required:**
  - Illegal Instruction  
  - Instruction Address Misaligned  
  - Load/Store Address Misaligned  
  - `ecall` (always traps to M-Mode)

- **ELF Loader:**  
  Emulator loads ELF segments to DRAM and sets PC to entry point.

**Deliverable:** A correct fetch–decode–execute CPU that runs simple C programs compiled for rv32i-ilp32 in M-Mode.

---

## Phase 2 Privilege Separation & SBI Firmware

**Objective:** Introduce S-Mode and a minimal SBI layer in M-Mode that boots the kernel and mediates access to hardware.

### Hardware Enhancements

- Add S-Mode and U-Mode.
- Implement `medeleg` and `mideleg`.
- Add CLINT-style timer: `mtime`, `mtimecmp`.

### Firmware (M-Mode)

#### Boot Sequence
1. Initialize UART + Timer.
2. Set `mtvec` → M-mode trap handler.
3. Delegate traps (ECALL from U, page faults) to S-Mode.
4. Configure PMP to allow S-Mode RAM access.
5. Load kernel ELF into DRAM.
6. Set `mepc` = kernel entry and `mstatus.MPP = S`.
7. `mret` → enter S-Mode.

#### SBI Subset
- **0x01** — Console Putchar  
- **0x00** — Set Timer  
- **0x08** — System Shutdown  

S-Mode calls → `ecall` → M-Mode trap → firmware handler.

### Early S-Mode Kernel

- Set `stvec` to kernel trap vector.
- Test SBI by printing to UART.
- Handle Supervisor Timer Interrupts if delegated.

**Deliverable:** A kernel boot message printed via SBI, timer interrupt firing, and controlled M→S privilege transition.

---

## Phase 3 Virtual Memory (Sv32) & User Mode

**Objective:** Implement address translation, page faults, syscalls, and first user-mode process.

### Hardware Enhancements

- Full Sv32 MMU (2-level page tables, 4 KiB pages).
- Implement `satp` (Mode = 1, PPN = root PT).
- Simple TLB (invalidate-on-write acceptable).
- Page faults on violations.

### Kernel (S-Mode)

#### Memory Management
- Physical frame allocator.
- Kernel page table identity-mapping kernel + MMIO.

#### Process Abstraction
- Single user process.
- ELF loader maps segments into user VA space.
- Allocate user stack at top of address space (e.g. high pages).

#### Trap Handler (stvec)
- Decode `scause`.
- On `ecall` from U → syscall handler.
- On page fault → kill process (Phase 3).

### Syscall ABI & User Runtime

- **ABI:** System V RISC-V ABI (a0–a7).
- **Syscall Interface:**
  - `ecall`  
  - syscall number in `a7`  
  - arguments in `a0–a5`  
  - return value in `a0`

- **Syscalls Implemented:**
  - `sys_write`
  - `sys_exit`
  - `sys_yield`

- **crt0.S:**
  - Set `gp`, `sp`.
  - Jump to `main`.
  - On return → `sys_exit`.
  
**Deliverable:** A user program running in U-Mode under Sv32, performing syscalls via kernel, printing to console via SBI.

---

## Phase 4 Multiprogramming & Preemption

**Objective:** Implement multi-process system with time-slicing via timer interrupts.

### Kernel Specification

#### Scheduler
- Round-Robin scheduling.
- M-mode timer interrupt sets `sip.STIP` to inject S-Mode timer interrupt.
- S-Mode trap handler calls `schedule()`.

#### PCB (Process Control Block)
Contains:
- Saved GPRs (x1–x31)
- Saved `sepc`
- Saved `sstatus`
- `satp` (page table root)
- Process state: `READY`, `RUNNING`, `ZOMBIE`

#### Context Switch
- Save current trap frame.
- Load next trap frame.
- Switch `satp`.
- `sret` back to U-mode.

#### Syscalls
- `sys_fork`: deep copy of address space.
- `sys_exec`: replace current address space with new ELF image.
- `sys_wait`: block until child process exits.

**Deliverable:** Two or more user processes executing simultaneously, with timer-driven preemption, IPC via wait/exit semantics, and correct address-space isolation.

---

## References

* RISC-V References:
    * [Unprivileged Architecture](https://docs.riscv.org/reference/isa/_attachments/riscv-unprivileged.pdf)
    * [Privileged Architecture](https://docs.riscv.org/reference/isa/_attachments/riscv-privileged.pdf)
* Testing hardware emulation correctness:
    * [RISC-V Testing Suite](https://github.com/riscv-software-src/riscv-tests/tree/master/isa)
* Computer architecture book based on RISC-V
    * [Computer Organization and Design RISC-V Edition](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/books/HandP_RISCV.pdf&ved=2ahUKEwjyoJj_2qeRAxUeBbkGHcCcKPwQFnoECBsQAQ&usg=AOvVaw0SK-qYKXSJjBIy2DH__Efk)
* Concise reference on RISC-V ISA
    * [The RISC-V Reader: An Open Architecture Atlas](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/books/rvbook.pdf&ved=2ahUKEwiJ6MngpKeRAxXaBbkGHfLUGtUQFnoECAwQAQ&usg=AOvVaw07MpRtM_9xK4AyWnw9xmij)
* On building/implement an OS
    * [xv6: a simple, Unix-like teaching operating system](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://pdos.csail.mit.edu/6.828/2023/xv6/book-riscv-rev3.pdf&ved=2ahUKEwjX1M7AqaeRAxVqLrkGHZWrOfkQFnoECAwQAQ&usg=AOvVaw3MhKpOXQZJMEQPjdYJkISa) 
* Other
   * [Linkers and loaders](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://1380295634-files.gitbook.io/~/files/v0/b/gitbook-legacy-files/o/assets%252F-MV9vJFv4kmvRLgEog6g%252F-MZ6b4zb2hA1BGZxesCv%252F-MZ6bMP71xXn3a9tNaxf%252FLinkers%2520and%2520Loaders%2520by%2520John%2520R.%2520Levine.pdf%3Falt%3Dmedia%26token%3Da7f35f32-efe8-43d6-8cec-b8b231bdf189) 
