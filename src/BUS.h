#ifndef BUS_H
#define BUS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <UTIL.h>

#define RAM_SIZE 65536 // 64 * 1024

typedef struct {
    uint8_t ram[RAM_SIZE];
} BUS;

void BUS_init(BUS *bus);
void BUS_write(BUS *bus, uint16_t addr, uint8_t data);
uint8_t BUS_read(BUS *bus, uint16_t addr, bool read_only);

void BUS_dump_memory(BUS *bus, size_t num_bytes);

#endif // BUS_H