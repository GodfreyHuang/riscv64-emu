#include "cpu.h"
#include "csr.h"
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
    //cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] + cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] =
        (uint64_t) ((int64_t)cpu->regs[rs1(inst)] + (int64_t)cpu->regs[rs2(inst)]);
    print_op("add\n");
}

void exec_ADDI(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] + (int64_t) imm;
    print_op("addi\n");
}

void exec_ADDW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (int64_t) (int32_t) (cpu->regs[rs1(inst)] +
                                               (int64_t) cpu->regs[rs2(inst)]);
    print_op("addw\n");
}

void exec_ADDIW(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] + (int64_t) imm;
    print_op("addiw\n");
}

// SUB Operation
void exec_SUBW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (int64_t) (int32_t) (cpu->regs[rs1(inst)] -
                                               (int64_t) cpu->regs[rs2(inst)]);
    print_op("subw\n");
}

// MUL Operation
void exec_MULW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (int64_t) (int32_t) (cpu->regs[rs1(inst)] *
                                               (int64_t) cpu->regs[rs2(inst)]);
    print_op("mulw\n");
}

// DIV Operation
void exec_DIVW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (int64_t) (int32_t) (cpu->regs[rs1(inst)] /
                                               (int64_t) cpu->regs[rs2(inst)]);
    print_op("divw\n");
}

void exec_DIVUW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] / (int64_t) cpu->regs[rs2(inst)];
    print_op("divuw\n");
}

// Remainder Operation
void exec_REMW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (int64_t) (int32_t) (cpu->regs[rs1(inst)] %
                                               (int64_t) cpu->regs[rs2(inst)]);
    print_op("remw\n");
}

void exec_REMUW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] % (int64_t) cpu->regs[rs2(inst)];
    print_op("remuw\n");
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

void exec_SRAW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = (int64_t) (int32_t) (cpu->regs[rs1(inst)] >>
                                               (uint64_t) (int64_t) (int32_t)
                                                   cpu->regs[rs2(inst)]);
    print_op("sraw\n");
}

void exec_SRAIW(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    cpu->regs[rd(inst)] =
        (int64_t) (int32_t) (cpu->regs[rs1(inst)] >>
                             (uint64_t) (int64_t) (int32_t) imm);
    print_op("sraiw\n");
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

void exec_SLLW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        (int64_t) (int32_t) (cpu->regs[rs1(inst)] << cpu->regs[rs2(inst)]);
    print_op("sllw\n");
}

void exec_SLLIW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        (int64_t) (int32_t) (cpu->regs[rs1(inst)] << shamt(inst));
    print_op("slliw\n");
}

// Shift Right Logical Operation
void exec_SRL(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] >> cpu->regs[rs2(inst)];
    print_op("srl\n");
}

void exec_SRLI(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] >> shamt(inst);
    print_op("srli\n");
}

void exec_SRLW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        (int64_t) (int32_t) (cpu->regs[rs1(inst)] >> cpu->regs[rs2(inst)]);
    print_op("srlw\n");
}

void exec_SRLIW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] =
        (int64_t) (int32_t) cpu->regs[rs1(inst)] >> shamt(inst);
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

// Load Operation
void exec_LB(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rs1(inst)] = (int64_t) (int8_t) cpu_load(cpu, addr, 8);
    print_op("lb\n");
}

void exec_LH(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rs1(inst)] = (int64_t) (int16_t) cpu_load(cpu, addr, 16);
    print_op("lh\n");
}

void exec_LW(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rd(inst)] = (int64_t)(int32_t) cpu_load(cpu, addr, 32);
    //uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    //cpu->regs[rs1(inst)] = (int64_t) (int32_t) cpu_load(cpu, addr, 32);
    print_op("lw\n");
}

void exec_LD(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rs1(inst)] = (int64_t) cpu_load(cpu, addr, 64);
    print_op("ld\n");
}

// unsigned LB
void exec_LBU(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rs1(inst)] = cpu_load(cpu, addr, 8);
    print_op("lb\n");
}

// unsigned LH
void exec_LHU(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rs1(inst)] = cpu_load(cpu, addr, 16);
    print_op("lh\n");
}

void exec_LWU(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    uint64_t addr = cpu->regs[rs1(inst)] + (int64_t) imm;
    cpu->regs[rs1(inst)] = cpu_load(cpu, addr, 32);
    print_op("lw\n");
}

// B-Type Operation
// The Operation of Branch
void exec_BEQ(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    if ((int64_t) cpu->regs[rs1(inst)] == (int64_t) cpu->regs[rs2(inst)])
        cpu->pc = cpu->pc + (int64_t) imm - 4;
    print_op("beq\n");
}

void exec_BNE(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    if ((int64_t) cpu->regs[rs1(inst)] != (int64_t) cpu->regs[rs2(inst)])
        cpu->pc = (cpu->pc + (int64_t) imm - 4);
    //if ((int64_t) cpu->regs[rs1(inst)] != (int64_t) cpu->regs[rs2(inst)])
    //    cpu->pc = cpu->pc + (int64_t) imm - 4;
    print_op("bne\n");
}

void exec_BLT(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    if ((int64_t) cpu->regs[rs1(inst)] < (int64_t) cpu->regs[rs2(inst)])
        cpu->pc = cpu->pc + (int64_t) imm - 4;
    print_op("blt\n");
}

void exec_BGE(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    if ((int64_t) cpu->regs[rs1(inst)] >= (int64_t) cpu->regs[rs2(inst)])
        cpu->pc = cpu->pc + (int64_t) imm - 4;
    print_op("bge\n");
}

void exec_BLTU(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    if (cpu->regs[rs1(inst)] < cpu->regs[rs2(inst)])
        cpu->pc = cpu->pc + (int64_t) imm - 4;
    print_op("bltu\n");
}

void exec_BGEU(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_S(inst);
    if (cpu->regs[rs1(inst)] >= cpu->regs[rs2(inst)])
        cpu->pc = (int64_t) cpu->pc + (int64_t) imm - 4;
    print_op("bge\n");
}

uint64_t csr(uint32_t inst)
{
    // csr[11:0] = inst[31:20]
    return ((inst & 0xfff00000) >> 20);
}

//=====================================================================================
//   Instruction Execution Functions
//=====================================================================================

// Load Upper Immediate
void exec_LUI(CPU *cpu, uint32_t inst)
{
    // LUI places upper 20 bits of U-immediate value to rd
    cpu->regs[rd(inst)] = (uint64_t) (int64_t) (int32_t) (inst & 0xfffff000);
    print_op("lui\n");
}

void exec_AUIPC(CPU *cpu, uint32_t inst)
{
    // AUIPC forms a 32-bit offset from the 20 upper bits
    // of the U-immediate
    uint64_t imm = imm_U(inst);
    cpu->regs[rd(inst)] = ((int64_t) cpu->pc + (int64_t) imm) - 4;
    print_op("auipc\n");
}

void exec_JAL(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_J(inst);
    cpu->regs[rd(inst)] = cpu->pc;
    /*print_op("JAL-> rd:%ld, pc:%lx\n", rd(inst), cpu->pc);*/
    cpu->pc = cpu->pc + (int64_t) imm - 4;
    print_op("jal\n");
    if (ADDR_MISALIGNED(cpu->pc)) {
        fprintf(stderr, "JAL pc address misalligned");
        exit(0);
    }
}

void exec_JALR(CPU *cpu, uint32_t inst)
{
    uint64_t imm = imm_I(inst);
    uint64_t tmp = cpu->pc;
    cpu->pc = (cpu->regs[rs1(inst)] + (int64_t) imm) & 0xfffffffe;
    cpu->regs[rd(inst)] = tmp;
    /*print_op("NEXT -> %#lx, imm:%#lx\n", cpu->pc, imm);*/
    print_op("jalr\n");
    if (ADDR_MISALIGNED(cpu->pc)) {
        fprintf(stderr, "JAL pc address misalligned");
        exit(0);
    }
}

void exec_ECALL(CPU *cpu, uint32_t inst) {}
void exec_EBREAK(CPU *cpu, uint32_t inst) {}

void exec_ECALLBREAK(CPU *cpu, uint32_t inst)
{
    if (imm_I(inst) == 0x0)
        exec_ECALL(cpu, inst);
    if (imm_I(inst) == 0x1)
        exec_EBREAK(cpu, inst);
    print_op("ecallbreak\n");
}

// CSR instructions
void exec_CSRRW(CPU *cpu, uint32_t inst)
{
    cpu->regs[rd(inst)] = csr_read(cpu, csr(inst));
    csr_write(cpu, csr(inst), cpu->regs[rs1(inst)]);
    print_op("csrrw\n");
}

void exec_CSRRS(CPU *cpu, uint32_t inst)
{
    csr_write(cpu, csr(inst), cpu->csr[csr(inst)] | cpu->regs[rs1(inst)]);
    print_op("csrrs\n");
}

void exec_CSRRC(CPU *cpu, uint32_t inst)
{
    csr_write(cpu, csr(inst), cpu->csr[csr(inst)] & !(cpu->regs[rs1(inst)]));
    print_op("csrrc\n");
}

void exec_CSRRWI(CPU *cpu, uint32_t inst)
{
    csr_write(cpu, csr(inst), rs1(inst));
    print_op("csrrwi\n");
}

void exec_CSRRSI(CPU *cpu, uint32_t inst)
{
    csr_write(cpu, csr(inst), cpu->csr[csr(inst)] | rs1(inst));
    print_op("csrrsi\n");
}

void exec_CSRRCI(CPU *cpu, uint32_t inst)
{
    csr_write(cpu, csr(inst), cpu->csr[csr(inst)] & !rs1(inst));
    print_op("csrrci\n");
}

// AMO_W
void exec_LR_W(CPU *cpu, uint32_t inst) {}
void exec_SC_W(CPU *cpu, uint32_t inst) {}
void exec_AMOSWAP_W(CPU *cpu, uint32_t inst) {}

void exec_AMOADD_W(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp + (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoadd.w\n");
}

void exec_AMOXOR_W(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp ^ (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoxor.w\n");
}

void exec_AMOAND_W(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp & (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoand.w\n");
}

void exec_AMOOR_W(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp | (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoor.w\n");
}

void exec_AMOMIN_W(CPU *cpu, uint32_t inst) {}
void exec_AMOMAX_W(CPU *cpu, uint32_t inst) {}
void exec_AMOMINU_W(CPU *cpu, uint32_t inst) {}
void exec_AMOMAXU_W(CPU *cpu, uint32_t inst) {}

// AMO_D TODO
void exec_LR_D(CPU *cpu, uint32_t inst) {}
void exec_SC_D(CPU *cpu, uint32_t inst) {}
void exec_AMOSWAP_D(CPU *cpu, uint32_t inst) {}

void exec_AMOADD_D(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp + (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoadd.w\n");
}

void exec_AMOXOR_D(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp ^ (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoxor.w\n");
}

void exec_AMOAND_D(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp & (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoand.w\n");
}

void exec_AMOOR_D(CPU *cpu, uint32_t inst)
{
    uint32_t tmp = cpu_load(cpu, cpu->regs[rs1(inst)], 32);
    uint32_t res = tmp | (uint32_t) cpu->regs[rs2(inst)];
    cpu->regs[rd(inst)] = tmp;
    cpu_store(cpu, cpu->regs[rs1(inst)], 32, res);
    print_op("amoor.w\n");
}

void exec_AMOMIN_D(CPU *cpu, uint32_t inst) {}
void exec_AMOMAX_D(CPU *cpu, uint32_t inst) {}
void exec_AMOMINU_D(CPU *cpu, uint32_t inst) {}
void exec_AMOMAXU_D(CPU *cpu, uint32_t inst) {}

void exec_FENCE(CPU *cpu, uint32_t inst)
{
    print_op("fence\n");
}