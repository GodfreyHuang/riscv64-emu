#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cpu.h"
#include "cpu_exec.h"
#include "opcode.h"

// ---------- Initialize ----------
void cpu_init(CPU *cpu)
{
    cpu->regs[0] = 0x00;
    cpu->regs[2] = DRAM_BASE + DRAM_SIZE;  // The pointer of stack, which init
                                           // to the top address of the memory
    cpu->pc =
        DRAM_BASE;  // The program counter points to the start of the memory
}

uint32_t cpu_fetch(CPU *cpu)
{
    return bus_load(&(cpu->bus), cpu->pc, 32);
}

uint64_t cpu_load(CPU *cpu, uint64_t addr, uint64_t size)
{
    return bus_load(&(cpu->bus), addr, size);
}

void cpu_store(CPU *cpu, uint64_t addr, uint64_t size, uint64_t value)
{
    bus_store(&(cpu->bus), addr, size, value);
}

int cpu_execute(CPU *cpu, uint32_t inst)
{
    int opcode = inst & 0x7f;          // opcode in bits 6..0
    int funct3 = (inst >> 12) & 0x7;   // funct3 in bits 14..12
    int funct7 = (inst >> 25) & 0x7f;  // funct7 in bits 31..25

    cpu->regs[0] = 0;  // x0 hardwired to 0 at each cycle

    // If cpu get I Type opcode:
    switch (opcode) {
    case R_TYPE:
        switch (funct3) {
        case ADDSUB:
            switch (funct7) {
            case ADD:
                exec_ADD(cpu, inst);  // finish
                break;
            case SUB:
                exec_ADD(cpu, inst);  // finish
                break;
            default:
                break;
            }
        case SLL:
            exec_SLL(cpu, inst);  // finish
            break;
        case SLT:
            exec_SLT(cpu, inst);  // finish
            break;
        case SLTU:
            exec_SLTU(cpu, inst);  // finish
            break;
        case XOR:
            exec_XOR(cpu, inst);  // finish
            break;
        case SR:
            switch (funct7) {
            case SRL:
                exec_SRL(cpu, inst);  // finish
                break;
            case SRA:
                exec_SRA(cpu, inst);  // finish
                break;
            default:;
            }
            break;
        case OR:
            exec_OR(cpu, inst);  // finish
            break;
        case AND:
            exec_AND(cpu, inst);  // finish
            break;
        default:
            fprintf(stderr,
                    "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct7:0x%x\n",
                    opcode, funct3, funct7);
            return 0;
        }
        break;

    case I_TYPE:
        switch (funct3) {
        case ADDI:
            exec_ADDI(cpu, inst);  // finish
            break;
        case SLLI:
            exec_SLLI(cpu, inst);  // finish
            break;
        case SLTI:
            exec_SLTI(cpu, inst);  // finish
            break;
        case SLTIU:
            exec_SLTIU(cpu, inst);  // finish
            break;
        case XORI:
            exec_XORI(cpu, inst);  // finish
            break;
        case SRI:
            switch (funct7) {
            case SRLI:
                exec_SRLI(cpu, inst);  // finish
                break;
            case SRAI:
                exec_SRAI(cpu, inst);  // finish
                break;
            default:;
            }
            break;
        case ORI:
            exec_ORI(cpu, inst);  // finish
            break;
        case ANDI:
            exec_ANDI(cpu, inst);  // finish
            break;
        default:
            fprintf(stderr,
                    "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct7:0x%x\n",
                    opcode, funct3, funct7);
            return 0;
        }
        break;

    default:
        fprintf(stderr, "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct3:0x%x\n",
                opcode, funct3, funct7);
        return 0;
        /*exit(1);*/
    }
}

void dump_registers(CPU *cpu) {}