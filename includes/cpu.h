#include <stdint.h>
#include "bus.h"

typedef struct cpu {
    uint64_t regs[32];  // 32 64-bit registers (x0-x31)
    uint64_t pc;        // 64-bit program counter
    BUS bus;            // CPU connected to BUS
} CPU;

void cpu_init(CPU *cpu);

uint32_t cpu_fetch(CPU *cpu);

int cpu_execute(CPU *cpu, uint32_t inst);

void dump_registers(CPU *cpu);