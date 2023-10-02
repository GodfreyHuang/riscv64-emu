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

// SLT in unsigned
void exec_SLTU(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] < cpu->regs[rs2(inst)]) ? 1 : 0;
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

// Store Operation: Store Byte
// M[R[rs1] + imm](7:0) = R[rs2](7:0)
void exec_SB(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);  // Get immediate in the inst
    uint64_t addr =
        cpu->regs[rs1(inst)] +
        (int64_t) imm;  // Get the address which rs1 store and add the imm value
    cpu_store(cpu, addr, 8,
              cpu->regs[rs2(inst)]);  // Store the value from rs2 into the
                                      // address. Using 8 bits because the
                                      // function is size of data is a byte
    print_op("sb\n");
}

// Store Operation: Store Halfword
// M[R[rs1] + imm](15:0) = R[rs2](15:0)
void exec_SH(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu_store(cpu, addr, 16, cpu->regs[rs2(inst)]);
    print_op("sh\n");
}

// Store Operation: Store Word
// M[R[rs1] + imm](31:0) = R[rs2](31:0)
void exec_SW(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu_store(cpu, addr, 32, cpu->regs[rs2(inst)]);
    print_op("sw\n");
}

// Store Operation: Store Doubleword
// M[R[rs1] + imm](63:0) = R[rs2](63:0)
void exec_SD(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu_store(cpu, addr, 64, cpu->regs[rs2(inst)]);
    print_op("sd\n");
}

void exec_LB(CPU *cpu, uint32_t inst)
{

    print_op("lb\n");
}

void exec_LH(CPU *cpu, uint32_t inst)
{

    print_op("lh\n");
}

void exec_LW(CPU *cpu, uint32_t inst)
{

    print_op("lw\n");
}

void exec_LD(CPU *cpu, uint32_t inst)
{

    print_op("ld\n");
}