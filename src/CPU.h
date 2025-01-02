#pragma once

#include <BUS.h>
#include <Util.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum CPU_FLAGS {
    C = (1 << 0), // Carry Bit
    Z = (1 << 1), // Zero
    I = (1 << 2), // Disable Interrupts
    D = (1 << 3), // Decimal Mode
    B = (1 << 4), // Break
    U = (1 << 5), // Unused
    V = (1 << 6), // Overflow
    N = (1 << 7)  // Negative
};

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
} CPU;

void CPU_init(CPU *cpu, BUS *bus) {
    if (!cpu || !bus) {
        PANIC("NULL POINTER in init!\n");
    }

    cpu->bus = bus;

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