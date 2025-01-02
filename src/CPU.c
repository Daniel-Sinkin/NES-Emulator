#include <CPU.h>

void CPU_init(CPU *cpu, BUS *bus) {
    if (!cpu || !bus) {
        PANIC("NULL POINTER in init!");
    }

    cpu->bus = bus;

    cpu->clock_counter = 0;
    cpu->fetched = 0x00;
    cpu->addr_abs = 0x0000;
    cpu->addr_rel = 0x0000;
    cpu->opcode = 0x00;
    cpu->cycles = 0x00;

    cpu->reg.A = 0x00;
    cpu->reg.X = 0x00;
    cpu->reg.Y = 0x00;
    cpu->reg.SP = 0xFD;
    cpu->reg.PC = 0x0000;
    cpu->reg.STATUS = 0x00;
}

uint8_t CPU_read(CPU *cpu, uint16_t addr) {
    return BUS_read(cpu->bus, addr, false);
}

void CPU_write(CPU *cpu, uint16_t addr, uint8_t data) {
    BUS_write(cpu->bus, addr, data);
}

uint8_t CPU_get_flag(CPU *cpu, CPU_FLAGS flag) {
    return cpu->reg.STATUS & flag;
}

void CPU_set_flag(CPU *cpu, CPU_FLAGS flag) {
    cpu->reg.STATUS |= flag;
}

void CPU_unset_flag(CPU *cpu, CPU_FLAGS flag) {
    cpu->reg.STATUS &= ~flag;
}

void CPU_print_registers(CPU *cpu) {
    if (!cpu) {
        printf("Error: CPU pointer is NULL.\n");
        return;
    }

    printf("=== CPU Registers ===\n");
    printf(" A:     0x%02X    | Accumulator\n", cpu->reg.A);
    printf(" X:     0x%02X    | X Register\n", cpu->reg.X);
    printf(" Y:     0x%02X    | Y Register\n", cpu->reg.Y);
    printf("OP:     0x%02X    | OP Code\n", cpu->opcode);
    printf("CYLCES: 0x%02X    | Remaining Cycles\n", cpu->reg.Y);
    printf("SP:     0x%02X    | Stack Pointer\n", cpu->reg.SP);
    printf("PC:     0x%04X  | Program Counter\n", cpu->reg.PC);
    printf("STATUS: 0x%02X    | [ ", cpu->reg.STATUS);

    printf("N:%d ", (CPU_get_flag(cpu, CPU_FLAGS_N)) ? 1 : 0);
    printf("V:%d ", (CPU_get_flag(cpu, CPU_FLAGS_V)) ? 1 : 0);
    printf("U:%d ", (CPU_get_flag(cpu, CPU_FLAGS_U)) ? 1 : 0);
    printf("B:%d ", (CPU_get_flag(cpu, CPU_FLAGS_B)) ? 1 : 0);
    printf("D:%d ", (CPU_get_flag(cpu, CPU_FLAGS_D)) ? 1 : 0);
    printf("I:%d ", (CPU_get_flag(cpu, CPU_FLAGS_I)) ? 1 : 0);
    printf("Z:%d ", (CPU_get_flag(cpu, CPU_FLAGS_Z)) ? 1 : 0);
    printf("C:%d ", (CPU_get_flag(cpu, CPU_FLAGS_C)) ? 1 : 0);
    printf("]\n");
    printf("=====================\n");
}

void CPU_read_pc(CPU *cpu) {
    cpu->opcode = CPU_read(cpu, cpu->reg.PC);
    cpu->reg.PC += 1;
}

void CPU_clock(CPU *cpu) {
    if (cpu->cycles == 0) {
        CPU_read_pc(cpu);

        OP_CODE_MATRIX_ENTRY op_code_matrix_entry = OP_CODE_MATRIX[cpu->opcode];
        cpu->cycles = op_code_matrix_entry.cycles;
        uint8_t cycle_add1 = op_code_matrix_entry.am(cpu);
        uint8_t cycle_add2 = op_code_matrix_entry.op(cpu);

        cpu->cycles += (cycle_add1 & cycle_add2);
    }

    cpu->cycles -= 1;
}

void CPU_reset(CPU *cpu) {
}
void CPU_irq(CPU *cpu) {
}
void CPU_nmi(CPU *cpu) {
}
void CPU_fetch(CPU *cpu) {
}

// Addressing mode functions
uint8_t CPU_AM_IMP(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_ZP0(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_ZPY(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_ABS(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_ABY(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_IZX(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_IMM(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_ZPX(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_REL(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_ABX(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_IND(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_IZY(CPU *cpu) {
    return 0;
}
uint8_t CPU_AM_XXX(CPU *cpu) {
    return 0;
}

// opcode functions
uint8_t CPU_ADC(CPU *cpu) {
    return 0;
}
uint8_t CPU_AND(CPU *cpu) {
    return 0;
}
uint8_t CPU_ASL(CPU *cpu) {
    return 0;
}
uint8_t CPU_BCC(CPU *cpu) {
    return 0;
}
uint8_t CPU_BCS(CPU *cpu) {
    return 0;
}
uint8_t CPU_BEQ(CPU *cpu) {
    return 0;
}
uint8_t CPU_BIT(CPU *cpu) {
    return 0;
}
uint8_t CPU_BMI(CPU *cpu) {
    return 0;
}
uint8_t CPU_BNE(CPU *cpu) {
    return 0;
}
uint8_t CPU_BPL(CPU *cpu) {
    return 0;
}
uint8_t CPU_BRK(CPU *cpu) {
    printf("BRK!");
    return 0;
}
uint8_t CPU_BVC(CPU *cpu) {
    return 0;
}
uint8_t CPU_BVS(CPU *cpu) {
    return 0;
}
uint8_t CPU_CLC(CPU *cpu) {
    return 0;
}
uint8_t CPU_CLD(CPU *cpu) {
    return 0;
}
uint8_t CPU_CLI(CPU *cpu) {
    return 0;
}
uint8_t CPU_CLV(CPU *cpu) {
    return 0;
}
uint8_t CPU_CMP(CPU *cpu) {
    return 0;
}
uint8_t CPU_CPX(CPU *cpu) {
    return 0;
}
uint8_t CPU_CPY(CPU *cpu) {
    return 0;
}
uint8_t CPU_DEC(CPU *cpu) {
    return 0;
}
uint8_t CPU_DEX(CPU *cpu) {
    return 0;
}
uint8_t CPU_DEY(CPU *cpu) {
    return 0;
}
uint8_t CPU_EOR(CPU *cpu) {
    return 0;
}
uint8_t CPU_INC(CPU *cpu) {
    return 0;
}
uint8_t CPU_INX(CPU *cpu) {
    return 0;
}
uint8_t CPU_INY(CPU *cpu) {
    return 0;
}
uint8_t CPU_JMP(CPU *cpu) {
    return 0;
}
uint8_t CPU_JSR(CPU *cpu) {
    return 0;
}
uint8_t CPU_LDA(CPU *cpu) {
    return 0;
}
uint8_t CPU_LDX(CPU *cpu) {
    return 0;
}
uint8_t CPU_LDY(CPU *cpu) {
    return 0;
}
uint8_t CPU_LSR(CPU *cpu) {
    return 0;
}
uint8_t CPU_NOP(CPU *cpu) {
    return 0;
}
uint8_t CPU_ORA(CPU *cpu) {
    return 0;
}
uint8_t CPU_PHA(CPU *cpu) {
    return 0;
}
uint8_t CPU_PHP(CPU *cpu) {
    return 0;
}
uint8_t CPU_PLA(CPU *cpu) {
    return 0;
}
uint8_t CPU_PLP(CPU *cpu) {
    return 0;
}
uint8_t CPU_ROL(CPU *cpu) {
    return 0;
}
uint8_t CPU_ROR(CPU *cpu) {
    return 0;
}
uint8_t CPU_RTI(CPU *cpu) {
    return 0;
}
uint8_t CPU_RTS(CPU *cpu) {
    return 0;
}
uint8_t CPU_SBC(CPU *cpu) {
    return 0;
}
uint8_t CPU_SEC(CPU *cpu) {
    return 0;
}
uint8_t CPU_SED(CPU *cpu) {
    return 0;
}
uint8_t CPU_SEI(CPU *cpu) {
    return 0;
}
uint8_t CPU_STA(CPU *cpu) {
    return 0;
}
uint8_t CPU_STX(CPU *cpu) {
    return 0;
}
uint8_t CPU_STY(CPU *cpu) {
    return 0;
}
uint8_t CPU_TAX(CPU *cpu) {
    return 0;
}
uint8_t CPU_TAY(CPU *cpu) {
    return 0;
}
uint8_t CPU_TSX(CPU *cpu) {
    return 0;
}
uint8_t CPU_TXA(CPU *cpu) {
    return 0;
}
uint8_t CPU_TXS(CPU *cpu) {
    return 0;
}
uint8_t CPU_TYA(CPU *cpu) {
    return 0;
}
uint8_t CPU_XXX(CPU *cpu) {
    return 0;
}
