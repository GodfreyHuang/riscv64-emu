#ifndef DRAM_H
#define DRAM_H
// DRAM
// The setting of DRAM using in RISC-V emulator
#include <stdint.h>

#define DRAM_SIZE 1024 * 1024 * 1  // 1 MiB DRAM
#define DRAM_BASE 0x80000000

typedef struct DRAM {
    uint8_t mem[DRAM_SIZE];  // Dram memory of DRAM_SIZE
} DRAM;

// load the dram data with the address and the data size, it will return the
// storage value
uint64_t dram_load(DRAM *dram, uint64_t addr, uint64_t size);

// store the value into the dram
void dram_store(DRAM *dram, uint64_t addr, uint64_t size, uint64_t value);

#endif