#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BUS.h>
#include <CPU.h>
#include <OP_Code.h>

int main() {
    /*
    BUS bus;
    BUS_init(&bus);
    CPU cpu;
    CPU_init(&cpu, &bus);

    CPU_print_registers(&cpu);
    CPU_set_flag(&cpu, CPU_FLAGS_U);
    CPU_print_registers(&cpu);
    CPU_unset_flag(&cpu, CPU_FLAGS_U);
    CPU_print_registers(&cpu);
    */

    opcode_matrix_example();

    return EXIT_SUCCESS;
}