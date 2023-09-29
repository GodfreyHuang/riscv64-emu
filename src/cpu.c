#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cpu.h"
#include "dram.h"
#include "opcode.h"

#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

// print operation for DEBUG
void print_op(char *s)
{
    printf("%s%s%s", ANSI_BLUE, s, ANSI_RESET);
}

// ---------- Initialize ----------

void cpu_init(CPU *cpu)
{
    cpu->regs[0] = 0x00;
    cpu->regs[2] = DRAM_BASE + DRAM_SIZE;  // The pointer of stack, which init
                                           // to the top address of the memory
    cpu->pc =
        DRAM_BASE;  // The program counter points to the start of the memory
}

// ---------- Instruction Decode ----------
// the address of destination register
uint64_t rd(uint32_t inst)
{
    return (inst >> 7) & 0x1f;  // rd in bits 11..7
}

// the address of source register 1
uint64_t rs1(uint32_t inst)
{
    return (inst >> 15) & 0x1f;  // rs1 in bits 19..15
}

// the address of source register 2
uint64_t rs2(uint32_t inst)
{
    return (inst >> 20) & 0x1f;  // rs2 in bits 24..20
}

// A value which gives the address of destination register
// I-Type: Immediate type instructions
uint64_t imm_I(uint32_t inst)
{
    // imm[11:0] = inst[31:20]
    return ((int64_t) (int32_t) (inst & 0xfff00000)) >> 20;
}

// S-Type: Store type instructions
uint64_t imm_S(uint32_t inst)
{
    // imm[11:5] = inst[31:25], imm[4:0] = inst[11:7]
    return ((int64_t) (int32_t) (inst & 0xfe000000) >> 20) |
           ((inst >> 7) & 0x1f);
}

// B-Type: Break type instructions
uint64_t imm_B(uint32_t inst)
{
    // imm[12|10:5|4:1|11] = inst[31|30:25|11:8|7]
    return ((int64_t) (int32_t) (inst & 0x80000000) >> 19) |
           ((inst & 0x80) << 4)      // imm[11]
           | ((inst >> 20) & 0x7e0)  // imm[10:5]
           | ((inst >> 7) & 0x1e);   // imm[4:1]
}

// U-Type: Register type instructions
uint64_t imm_U(uint32_t inst)
{
    // imm[31:12] = inst[31:12]
    return (int64_t) (int32_t) (inst & 0xfffff999);
}

// J-Type: Jump type instructions
uint64_t imm_J(uint32_t inst)
{
    // imm[20|10:1|11|19:12] = inst[31|30:21|20|19:12]
    return (uint64_t) ((int64_t) (int32_t) (inst & 0x80000000) >> 11) |
           (inst & 0xff000)           // imm[19:12]
           | ((inst >> 9) & 0x800)    // imm[11]
           | ((inst >> 20) & 0x7fe);  // imm[10:1]
}

// the shift amount
uint32_t shamt(uint32_t inst)
{
    // shamt(shift amount) only required for immediate shift instructions
    // shamt[4:5] = imm[5:0]
    return (uint32_t) (imm_I(inst) & 0x1f);  // TODO: 0x1f / 0x3f ?
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
            exec_SLTIU(cpu, inst);
            break;
        case XORI:
            exec_XORI(cpu, inst);  // finish
            break;
        case SRI:
            switch (funct7) {
            case SRLI:
                exec_SRLI(cpu, inst);
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
        default:;
        }
        break;

    default:
        fprintf(stderr, "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct3:0x%x\n",
                opcode, funct3, funct7);
        return 0;
        /*exit(1);*/
    }
}

// ADD Operation
void exec_ADD(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] + cpu->regs[rs2(inst)];
    print_op("add\n");
}

void exec_ADDI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] + (int64_t) imm;
    print_op("addi\n");
}

// SLT Operation
void exec_SLT(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        (cpu->regs[rs1(inst)] < (int64_t) cpu->regs[rs2(inst)]) ? 1 : 0;
    print_op("slt\n");
}

void exec_SLTI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] < (int64_t) imm) ? 1 : 0;
    print_op("slti\n");
}

// SRA Operation
void exec_SRA(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        (int32_t) cpu->regs[rs1(inst)] >> (int64_t) cpu->regs[rs2(inst)];
    print_op("sra\n");
}

void exec_SRAI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = (int32_t) cpu->regs[rs1(inst)] >> imm;
    print_op("srai\n");
}

// OR Operation
void exec_OR(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] | cpu->regs[rs2(inst)];
    print_op("or\n");
}

void exec_ORI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] | imm;
    print_op("ori\n");
}

// AND Operation
void exec_AND(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] & cpu->regs[rs2(inst)];
    print_op("and\n");
}

void exec_ANDI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] & imm;
    print_op("andi\n");
}

// XOR Operation
void exec_XOR(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] ^ cpu->regs[rs2(inst)];
    print_op("xor\n");
}

void exec_XORI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] ^ imm;
    print_op("xori\n");
}

// Shift Left Logical Operation
void exec_SLL(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)]
                          << (int64_t) cpu->regs[rs2(inst)];
    print_op("sll\n");
}

void exec_SLLI(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] << shamt(inst);
    print_op("slli\n");
}

void dump_registers(CPU *cpu) {}