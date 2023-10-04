#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "bus.h"

#define ADDR_MISALIGNED(addr) (addr & 0x3)

typedef struct cpu {
    uint64_t regs[32];  // 32 64-bit registers (x0-x31)
    uint64_t pc;        // 64-bit program counter
    uint64_t csr[4069];
    BUS bus;  // CPU connected to BUS
} CPU;

void cpu_init(CPU *cpu);

uint32_t cpu_fetch(CPU *cpu);

void cpu_store(CPU *cpu, uint64_t addr, uint64_t size, uint64_t value);

uint64_t cpu_load(CPU *cpu, uint64_t addr, uint64_t size);

int cpu_execute(CPU *cpu, uint32_t inst);

void dump_registers(CPU *cpu);

#endif