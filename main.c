#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BUS.h>
#include <CPU.h>

int main() {
    CPU cpu;
    BUS bus;

    CPU_init(&cpu, &bus);

    CPU_clock(&cpu);

    return EXIT_SUCCESS;
}