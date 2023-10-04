#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

void read_file(CPU *cpu, char *filename)
{
    FILE *file;
    uint8_t *buffer;
    unsigned long fileLen;

    // Open file
    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", filename);
    }
    // Get file length
    fseek(file, 0, SEEK_END);
    fileLen = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory
    buffer = (uint8_t *) malloc(fileLen + 1);
    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
    }
    // Read file contents into buffer
    fread(buffer, fileLen, 1, file);
    fclose(file);
    // Print file contents in hex
    for (int i = 0; i < fileLen; i += 2) {
        if (i % 16 == 0)
            printf("\n%.8x: ", i);
        printf("%02x%02x ", *(buffer + i), *(buffer + i + 1));
    }
    printf("\n");

    // copy the bin executable to dram
    memcpy(cpu->bus.dram.mem, buffer, fileLen * sizeof(uint8_t));
    free(buffer);
}

int main(int argc, char **argv)
{
    // Initialize cpu, registers and program counter
    CPU cpu;
    cpu_init(&cpu);
    // Read input file
    read_file(&cpu, argv[1]);

    // cpu loop
    while (1) {
        // fetch
        uint32_t inst = cpu_fetch(&cpu);
        // Increment the program counter
        cpu.pc += 4;
        // execute
        if (!cpu_execute(&cpu, inst))
            break;
        dump_registers(&cpu);
        if (cpu.pc == 0)
            break;
    }

    // printf("hello world\n");
    return 0;
}