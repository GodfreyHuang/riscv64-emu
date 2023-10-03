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

    case R_TYPE_64:
        switch (funct3) {
        case ADDSUB:
            switch (funct7) {
            case ADDW:
                exec_ADDW(cpu, inst);  // finish
                break;
            case SUBW:
                exec_SUBW(cpu, inst);  // finish
                break;
            case MULW:
                exec_MULW(cpu, inst);  // finish
                break;
            }
            break;
        case DIVW:
            exec_DIVW(cpu, inst);  // finish
            break;
        case SLLW:
            exec_SLLW(cpu, inst);  // finish
            break;
        case SRW:
            switch (funct7) {
            case SRLW:
                exec_SRLW(cpu, inst);  // finish
                break;
            case SRAW:
                exec_SRAW(cpu, inst);  // finish
                break;
            case DIVUW:
                exec_DIVUW(cpu, inst);  // finish
                break;
            }
            break;
        case REMW:
            exec_REMW(cpu, inst);  // finish
            break;
        case REMUW:
            exec_REMUW(cpu, inst);  // finish
            break;
        default:;
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

    case I_TYPE_64:
        switch (funct3) {
        case ADDIW:
            exec_ADDIW(cpu, inst);  // finish
            break;
        case SLLIW:
            exec_SLLIW(cpu, inst);  // finish
            break;
        case SRIW:
            switch (funct7) {
            case SRLIW:
                exec_SRLIW(cpu, inst);  // finish
                break;
            case SRAIW:
                exec_SRAIW(cpu, inst);  // finish
                break;
            default:;
            }
            break;
        default:;
        }
        break;

    case S_TYPE:
        switch (funct3) {
        case SB:
            exec_SB(cpu, inst);  // finish
            break;
        case SH:
            exec_SH(cpu, inst);  // finish
            break;
        case SW:
            exec_SW(cpu, inst);  // finish
            break;
        case SD:
            exec_SD(cpu, inst);  // finish
            break;
        default:;
        }
        break;

    case LOAD:
        switch (funct3) {
        case LB:
            exec_LB(cpu, inst);  // finish
            break;
        case LH:
            exec_LH(cpu, inst);  // finish
            break;
        case LW:
            exec_LW(cpu, inst);  // finish
            break;
        case LD:
            exec_LD(cpu, inst);  // finish
            break;
        case LBU:
            exec_LBU(cpu, inst);  // finish
            break;
        case LHU:
            exec_LHU(cpu, inst);  // finish
            break;
        case LWU:
            exec_LWU(cpu, inst);  // finish
            break;
        default:;
        }
        break;

    case B_TYPE:
        switch (funct3) {
        case BEQ:
            exec_BEQ(cpu, inst);  // finish
            break;
        case BNE:
            exec_BNE(cpu, inst);  // finish
            break;
        case BLT:
            exec_BLT(cpu, inst);  // finish
            break;
        case BGE:
            exec_BGE(cpu, inst);  // finish
            break;
        case BLTU:
            exec_BLTU(cpu, inst);  // finish
            break;
        case BGEU:
            exec_BGEU(cpu, inst);  // finish
            break;
        default:;
        }
        break;

    case LUI:
        exec_LUI(cpu, inst);
        break;
    case AUIPC:
        exec_AUIPC(cpu, inst);
        break;

    case JAL:
        exec_JAL(cpu, inst);
        break;
    case JALR:
        exec_JALR(cpu, inst);
        break;

    case CSR:
        switch (funct3) {
        case ECALLBREAK:
            exec_ECALLBREAK(cpu, inst);
            break;
        case CSRRW:
            exec_CSRRW(cpu, inst);
            break;
        case CSRRS:
            exec_CSRRS(cpu, inst);
            break;
        case CSRRC:
            exec_CSRRC(cpu, inst);
            break;
        case CSRRWI:
            exec_CSRRWI(cpu, inst);
            break;
        case CSRRSI:
            exec_CSRRSI(cpu, inst);
            break;
        case CSRRCI:
            exec_CSRRCI(cpu, inst);
            break;
        default:
            fprintf(stderr,
                    "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct7:0x%x\n",
                    opcode, funct3, funct7);
            return 0;
        }
        break;

    case AMO_W:
        switch (funct7 >> 2) {  // since, funct[1:0] = aq, rl
        case LR_W:
            exec_LR_W(cpu, inst);
            break;
        case SC_W:
            exec_SC_W(cpu, inst);
            break;
        case AMOSWAP_W:
            exec_AMOSWAP_W(cpu, inst);
            break;
        case AMOADD_W:
            exec_AMOADD_W(cpu, inst);
            break;
        case AMOXOR_W:
            exec_AMOXOR_W(cpu, inst);
            break;
        case AMOAND_W:
            exec_AMOAND_W(cpu, inst);
            break;
        case AMOOR_W:
            exec_AMOOR_W(cpu, inst);
            break;
        case AMOMIN_W:
            exec_AMOMIN_W(cpu, inst);
            break;
        case AMOMAX_W:
            exec_AMOMAX_W(cpu, inst);
            break;
        case AMOMINU_W:
            exec_AMOMINU_W(cpu, inst);
            break;
        case AMOMAXU_W:
            exec_AMOMAXU_W(cpu, inst);
            break;
        default:
            fprintf(stderr,
                    "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct7:0x%x\n",
                    opcode, funct3, funct7);
            return 0;
        }
        break;

    case FENCE:
        exec_FENCE(cpu, inst);
        break;

    case 0x00:
        return 0;

    default:
        fprintf(stderr, "[-] ERROR-> opcode:0x%x, funct3:0x%x, funct3:0x%x\n",
                opcode, funct3, funct7);
        return 0;
        /*exit(1);*/
    }
}

void dump_registers(CPU *cpu) {}