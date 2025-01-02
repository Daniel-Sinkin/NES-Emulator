#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Util.h>

#define RAM_SIZE 64 * 1024

typedef struct {
    uint8_t ram[RAM_SIZE];
} BUS;

void BUS_init(BUS *bus) {
    for (size_t i = 0; i < RAM_SIZE; i++) {
        bus->ram[i] = 0;
    }
}

void BUS_write(BUS *bus, uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0xFFFF) {
        bus->ram[addr] = data;
    } else {
        PANIC_FMT("ACCESS VIOLATION WRITE, addr = %d, data = %d", addr, data);
    }
}
uint8_t BUS_read(BUS *bus, uint16_t addr, bool read_only) {
    if (addr >= 0x0000 && addr <= 0xFFFF) {
        return bus->ram[addr];
    } else {
        PANIC_FMT("ACCESS VIOLATION READ, addr = %d", addr);
    }
}