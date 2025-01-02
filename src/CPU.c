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

uint8_t CPU_read_from_stack(CPU *cpu) {
    cpu->reg.SP += 1;
    return CPU_read(cpu, STACK_ORIGIN + cpu->reg.SP);
}

uint8_t CPU_read_pc(CPU *cpu) {
    // Also increments the PC
    uint8_t val = CPU_read(cpu, cpu->reg.PC);
    cpu->reg.PC += 1;
    return val;
}

void CPU_write(CPU *cpu, uint16_t addr, uint8_t data) {
    BUS_write(cpu->bus, addr, data);
}

void CPU_write_to_stack(CPU *cpu, uint8_t data) {
    CPU_write(cpu, STACK_ORIGIN + cpu->reg.SP, data);
    cpu->reg.SP -= 1;
}

uint8_t CPU_get_flag(CPU *cpu, CPU_FLAGS flag) {
    return cpu->reg.STATUS & flag;
}

void CPU_set_flag(CPU *cpu, CPU_FLAGS flag, bool activate) {
    if (activate) {
        cpu->reg.STATUS |= flag;
    } else {
        cpu->reg.STATUS &= ~flag;
    }
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

void CPU_clock(CPU *cpu) {
    if (cpu->cycles == 0) {
        cpu->opcode = CPU_read_pc(cpu);

        OP_CODE_MATRIX_ENTRY op_code_matrix_entry = OP_CODE_MATRIX[cpu->opcode];
        cpu->cycles = op_code_matrix_entry.cycles;
        uint8_t cycle_add1 = op_code_matrix_entry.am(cpu);
        uint8_t cycle_add2 = op_code_matrix_entry.op(cpu);

        cpu->cycles += (cycle_add1 & cycle_add2);
    }

    cpu->cycles -= 1;
}

void CPU_reset(CPU *cpu) {
    cpu->reg.A = 0;
    cpu->reg.X = 0;
    cpu->reg.Y = 0;
    cpu->reg.SP = 0xFD;
    cpu->reg.STATUS = 0x00 | CPU_FLAGS_U;

    cpu->addr_abs = 0xFFFC;
    uint16_t low = CPU_read(cpu, cpu->addr_abs + 0);
    uint16_t high = CPU_read(cpu, cpu->addr_abs + 1);

    cpu->reg.PC = (high << 8) | low;

    cpu->addr_rel = 0x0000;
    cpu->addr_abs = 0x0000;
    cpu->fetched = 0x00;

    cpu->cycles = 8;
}
void CPU_irq(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_I) == 0) return;

    CPU_write_to_stack(cpu, (cpu->reg.PC >> 8) & 0x00FF);
    CPU_write_to_stack(cpu, cpu->reg.PC & 0x00FF);

    CPU_set_flag(cpu, CPU_FLAGS_B, false);
    CPU_set_flag(cpu, CPU_FLAGS_U, true);
    CPU_set_flag(cpu, CPU_FLAGS_I, true);
    CPU_write_to_stack(cpu, cpu->reg.STATUS);

    cpu->addr_abs = 0xFFFE;
    uint16_t low = CPU_read(cpu, cpu->addr_abs + 0);
    uint16_t high = CPU_read(cpu, cpu->addr_abs + 1);
    cpu->reg.PC = (high << 8) | low;

    cpu->cycles = 7;
}

void CPU_nmi(CPU *cpu) {
    CPU_write_to_stack(cpu, (cpu->reg.PC) >> 8 & 0x00FF);
    CPU_write_to_stack(cpu, (cpu->reg.PC) & 0x00FF);

    CPU_set_flag(cpu, CPU_FLAGS_B, false);
    CPU_set_flag(cpu, CPU_FLAGS_U, true);
    CPU_set_flag(cpu, CPU_FLAGS_I, true);
    CPU_write_to_stack(cpu, cpu->reg.STATUS);

    cpu->addr_abs = 0xFFFE;
    uint16_t low = CPU_read(cpu, cpu->addr_abs + 0);
    uint16_t high = CPU_read(cpu, cpu->addr_abs + 1);
    cpu->reg.PC = (high << 8) | low;

    cpu->cycles = 8;
}

void CPU_fetch(CPU *cpu) {
    if (OP_CODE_MATRIX[cpu->opcode].am == CPU_AM_IMP) {
        cpu->fetched = CPU_read(cpu, cpu->addr_abs);
    }
    return cpu->fetched;
}

// Addressing mode functions
uint8_t CPU_AM_IMP(CPU *cpu) {
    cpu->fetched = cpu->reg.A; // Fetches accumulator value
    return 0;
}
uint8_t CPU_AM_ZP0(CPU *cpu) {
    // 0XABCD -> page 0xAB offset into that page 0xCD, have 255 pages w/ 255 entries each
    cpu->addr_abs = CPU_read_pc(cpu);
    cpu->addr_abs &= 0x00FF;

    return 0;
}
uint8_t CPU_AM_ZPY(CPU *cpu) {
    cpu->addr_abs = (CPU_read_pc(cpu) + cpu->reg.Y) & 0x00FF;
    return 0;
}
uint8_t CPU_AM_ABS(CPU *cpu) {
    uint16_t low = CPU_read_pc(cpu);
    uint16_t high = CPU_read_pc(cpu);

    cpu->addr_abs = (high << 8) | low;

    return 0;
}
uint8_t CPU_AM_ABY(CPU *cpu) {
    uint16_t low = CPU_read_pc(cpu);
    uint16_t high = CPU_read_pc(cpu);

    cpu->addr_abs = (high << 8) | low;
    cpu->addr_abs += cpu->reg.Y;

    // Deals with overflows
    if ((cpu->addr_abs & 0xFF00) != (high << 8)) {
        return 1;
    } else {
        return 0;
    }
}
uint8_t CPU_AM_IZX(CPU *cpu) {
    uint16_t addr = CPU_read_pc(cpu);

    uint16_t low_addr = (uint16_t)(addr + (uint16_t)cpu->reg.X) & 0x00FF;
    uint16_t low = CPU_read(cpu, low_addr);
    uint16_t high_addr = (uint16_t)(addr + (uint16_t)cpu->reg.X + 1) & 0x00FF;
    uint16_t high = CPU_read(cpu, high_addr);

    cpu->addr_abs = (high << 8) | low;

    return 0;
}
uint8_t CPU_AM_IMM(CPU *cpu) {
    cpu->addr_abs += cpu->reg.PC;
    cpu->reg.PC += 1;
    return 0;
}
uint8_t CPU_AM_ZPX(CPU *cpu) {
    cpu->addr_abs = (cpu) + cpu->reg.X;
    cpu->addr_abs &= 0x00FF;

    return 0;
}
uint8_t CPU_AM_REL(CPU *cpu) {
    cpu->addr_rel = CPU_read_pc(cpu);
    if (cpu->addr_rel & 0x80) {
        cpu->addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t CPU_AM_ABX(CPU *cpu) {
    uint16_t low = CPU_read_pc(cpu);
    uint16_t high = CPU_read_pc(cpu);

    cpu->addr_abs = (high << 8) | low;
    cpu->addr_abs += cpu->reg.X;

    // Deals with overflows
    if ((cpu->addr_abs & 0xFF00) != (high << 8)) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t CPU_AM_IND(CPU *cpu) {
    uint16_t ptr_low = CPU_read_pc(cpu);
    uint16_t ptr_high = CPU_read_pc(cpu);

    uint16_t ptr = (ptr_high << 8) | ptr_low;
    // There is a hardware bug in the NES CPUs, this introduces this bug here to
    // align with the hardware behaviour, see also:
    // http://wiki.nesdev.com/w/index.php/CPU_addressing_modes
    if (ptr_low == 0x00FF) {
        cpu->addr_abs = (CPU_read(cpu, ptr & 0xFF0) << 8) | CPU_read(cpu, ptr + 0);
    } else {
        cpu->addr_abs = (CPU_read(cpu, ptr + 1) << 8) | CPU_read(cpu, ptr + 0);
    }

    return 0;
}

uint8_t CPU_AM_IZY(CPU *cpu) {
    uint16_t addr = CPU_read_pc(cpu);

    uint16_t low = CPU_read(cpu, addr & 0x00FF);
    uint16_t high = CPU_read(cpu, (addr + 1) & 0x00FF);

    cpu->addr_abs = (high << 8) | low + cpu->reg.Y;

    if ((cpu->addr_abs & 0xFF00) != (high << 8)) {
        return 1;
    } else {
        return 0;
    }
}
uint8_t CPU_AM_XXX(CPU *cpu) {
    return 0;
}

// opcode functions
uint8_t CPU_ADC(CPU *cpu) {
    CPU_fetch(cpu);

    uint16_t tmp = (uint16_t)cpu->reg.A + (uint16_t)cpu->fetched + (uint16_t)CPU_get_flag(cpu, CPU_FLAGS_C);
    CPU_set_flag(cpu, CPU_FLAGS_C, tmp > 255);
    CPU_set_flag(cpu, CPU_FLAGS_Z, (tmp & 0x00FF) == 0);
    CPU_set_flag(cpu, CPU_FLAGS_N, 0x80);
    CPU_set_flag(
        cpu,
        CPU_FLAGS_V,
        (~((uint16_t)cpu->reg.A ^ (uint16_t)cpu->fetched) & ((uint16_t)cpu->reg.A ^ (uint16_t)tmp)));
    cpu->reg.A = tmp & 0x00FF;
    return 1;
}
uint8_t CPU_AND(CPU *cpu) {
    CPU_fetch(cpu);
    cpu->reg.A &= cpu->fetched;
    if (cpu->reg.A == 0x00) CPU_set_flag(cpu, CPU_FLAGS_Z, true);
    if (cpu->reg.A & 0x80) CPU_set_flag(cpu, CPU_FLAGS_N, true);
    return 1;
}
uint8_t CPU_ASL(CPU *cpu) {
    return 0;
}
uint8_t CPU_BCC(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_C) == 0) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BCS(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_C)) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BEQ(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_Z)) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BIT(CPU *cpu) {
    return 0;
}
uint8_t CPU_BMI(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_N)) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BNE(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_Z) == 0) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BPL(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_N) == 0) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BRK(CPU *cpu) {
    printf("BRK!");
    return 0;
}
uint8_t CPU_BVC(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_V) == 0) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_BVS(CPU *cpu) {
    if (CPU_get_flag(cpu, CPU_FLAGS_V)) {
        cpu->cycles += 1;
        cpu->addr_abs = cpu->reg.PC + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->reg.PC & 0xFF00)) cpu->cycles += 1;

        cpu->reg.PC = cpu->addr_abs;
    }
    return 0;
}
uint8_t CPU_CLC(CPU *cpu) {
    CPU_set_flag(cpu, CPU_FLAGS_C, false);
    return 0;
}
uint8_t CPU_CLD(CPU *cpu) {
    CPU_set_flag(cpu, CPU_FLAGS_D, false);
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
    // Push on stack
    CPU_write_to_stack(cpu, cpu->reg.A);
    return 0;
}
uint8_t CPU_PHP(CPU *cpu) {
    return 0;
}
uint8_t CPU_PLA(CPU *cpu) {
    // Pop off stack
    cpu->reg.A = CPU_read_from_stack(cpu);
    CPU_set_flag(cpu, CPU_FLAGS_Z, cpu->reg.A == 0x00);
    CPU_set_flag(cpu, CPU_FLAGS_N, cpu->reg.A & 0x80);
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
    cpu->reg.STATUS = CPU_read_from_stack(cpu);
    CPU_set_flag(cpu, CPU_FLAGS_B, false);
    CPU_set_flag(cpu, CPU_FLAGS_U, false);

    cpu->reg.PC = CPU_read_from_stack(cpu);
    cpu->reg.PC |= CPU_read_from_stack(cpu) << 8;

    return 0;
}
uint8_t CPU_RTS(CPU *cpu) {
    return 0;
}
uint8_t CPU_SBC(CPU *cpu) {
    CPU_fetch(cpu);

    uint16_t fetched_inv = ((uint16_t)cpu->fetched) ^ 0x00FF;

    uint16_t tmp = (uint16_t)cpu->reg.A + fetched_inv + (uint16_t)CPU_get_flag(cpu, CPU_FLAGS_C);
    CPU_set_flag(cpu, CPU_FLAGS_C, tmp & 0xFF00);
    CPU_set_flag(cpu, CPU_FLAGS_Z, (tmp & 0x00FF) == 0);
    CPU_set_flag(cpu, CPU_FLAGS_N, 0x0080);
    CPU_set_flag(
        cpu,
        CPU_FLAGS_V,
        (tmp ^ (uint16_t)cpu->reg.A) & (tmp ^ fetched_inv) & 0x0080);
    cpu->reg.A = tmp & 0x00FF;
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
