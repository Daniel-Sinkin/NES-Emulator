#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BUS.h>
#include <CPU.h>
#include <OP_Code.h>

int main() {
    BUS bus;
    BUS_init(&bus);
    CPU cpu;
    CPU_init(&cpu, &bus);

    CPU_write(&cpu, 0x1, 0xDC);
    CPU_write(&cpu, 0x3, 0xA3);

    for (size_t i = 0; i < RAM_SIZE; i++) {
        printf("%d\n", CPU_read(&cpu, i));
        if (i > 5) break;
    }

    return EXIT_SUCCESS;
}