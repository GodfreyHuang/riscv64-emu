#include "cpu.h"
#include "dram.h"
#include "isa_decode.h"
#include "opcode.h"

#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

// print operation for DEBUG
void print_op(char *s)
{
    printf("%s%s%s", ANSI_BLUE, s, ANSI_RESET);
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

void exec_SLTU(CPU *cpu, uint32_t inst)
{
    print_op("sltu\n");
}

void exec_SLTIU(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] < imm) ? 1 : 0;
    print_op("sltiu\n");
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

// Shift Right Logical Operation
void exec_SRL(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        cpu->regs[rs1(inst)] >> (int64_t) cpu->regs[rs2(inst)];
    print_op("srl\n");
}

void exec_SRLI(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] >> shamt(inst);
    print_op("srli\n");
}