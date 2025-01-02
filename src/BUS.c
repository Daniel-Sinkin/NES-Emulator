#include <BUS.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <UTIL.h>

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

void BUS_dump_memory(BUS *bus, size_t num_bytes) {
    if (num_bytes > RAM_SIZE) {
        PANIC_FMT("Requested bytes of %zu exceeds RAM size of %d\n", num_bytes, RAM_SIZE);
    }

    size_t bytes_per_line = 16; // Standard for memory dumps
    for (size_t i = 0; i < num_bytes; i += bytes_per_line) {
        printf("%04X  ", (uint16_t)i);

        for (size_t j = 0; j < bytes_per_line; j++) {
            if (i + j < num_bytes) {
                printf("%02X ", bus->ram[i + j]);
            } else {
                printf("   ");
            }
        }

        printf(" |");
        for (size_t j = 0; j < bytes_per_line; j++) {
            if (i + j < num_bytes) {
                uint8_t byte = bus->ram[i + j];
                printf("%c", (byte >= 32 && byte <= 126) ? byte : '.');
            } else {
                printf(" ");
            }
        }
        printf("|\n");
    }
}