#pragma once

#include <BUS.h>
#include <Util.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CPU_FLAGS_C = (1 << 0), // Carry Bit
    CPU_FLAGS_Z = (1 << 1), // Zero
    CPU_FLAGS_I = (1 << 2), // Disable Interrupts
    CPU_FLAGS_D = (1 << 3), // Decimal Mode
    CPU_FLAGS_B = (1 << 4), // Break
    CPU_FLAGS_U = (1 << 5), // Unused
    CPU_FLAGS_V = (1 << 6), // Overflow
    CPU_FLAGS_N = (1 << 7)  // Negative
} CPU_FLAGS;

typedef struct {
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t SP;
    uint8_t PC;
    uint8_t STATUS;
} REG;

typedef struct {
    BUS *bus;
    REG reg;
    size_t clock_counter;
    uint8_t fetched;
    uint16_t addr_abs;
    uint16_t addr_rel; // Relative address for jump instr.
    uint8_t opcode;
    uint8_t cycles; // cylcles remaining for current instruction
} CPU;

void CPU_init(CPU *cpu, BUS *bus) {
    if (!cpu || !bus) {
        PANIC("NULL POINTER in init!\n");
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
    cpu->opcode = CPU_read(&cpu, cpu->reg.PC);
    cpu->reg.PC += 1;
}

void CPU_clock(CPU *cpu) {
    if (cpu->cycles == 0) {
        CPU_read_pc(cpu);
    }
}
void CPU_reset(CPU *cpu);
void CPU_irq(CPU *cpu);
void CPU_nmi(CPU *cpu);
void CPU_fetch(CPU *cpu);