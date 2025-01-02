#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    OP_ADDR_IMPLIED,  // Implied addressing mode
    OP_ADDR_RELATIVE, // Relative addressing mode
    OP_ADDR_ABSOLUTE, // Absolute addressing mode
    OP_ADDR_IMM,      // Immediate addressing mode
    OP_ADDR_IND_X,    // Indexed-indirect (X) addressing mode
    OP_ADDR_IND_Y,    // Indirect-indexed (Y) addressing mode
    OP_ADDR_ZP,       // Zero-page addressing mode
    OP_ADDR_ZP_X,     // Zero-page X addressing mode
    OP_ADDR_ABS_Y,    // Absolute Y addressing mode
    OP_ADDR_ACCUM,    // Accumulator addressing mode
    OP_ADDR_ABS,      // Absolute addressing mode
    OP_ADDR_NOT_INIT, // Default for not Initialized, not actually a opcode
    OP_ADDR_ENUM_SIZE // How many opcode addressing mode we have
} ADDRESSING_MODE;

typedef struct {
    char mnemonic[4];     // Mnemonic
    ADDRESSING_MODE mode; // Addressing mode (from the enum)
    uint8_t size;         // Opcode size in bytes
    uint8_t cycles;       // Number of cycles this opcode takes
    // 0 -> normal
    // 1 -> * Add 1 ot N if page boundary is crossed
    // 2 -> ** Add 1 to N if branch occurs to same page, add 2 to N if branch occurs to different page
    uint8_t cycle_modifier;
} OPCODE_MATRIX_ENTRY;

void set_opcode_matrix_entry(
    OPCODE_MATRIX_ENTRY *entry,
    const char *mnemonic,
    ADDRESSING_MODE mode,
    uint8_t size,
    uint8_t cycles,
    uint8_t cycle_modifier) {

    strncpy(entry->mnemonic, mnemonic, sizeof(entry->mnemonic));
    entry->mnemonic[sizeof(entry->mnemonic) - 1] = '\0';
    entry->mode = mode;
    entry->size = size;
    entry->cycles = cycles;
    entry->cycle_modifier = cycle_modifier;
}

int main() {
    OPCODE_MATRIX_ENTRY op_code_matrix[16][16];

    OPCODE_MATRIX_ENTRY default_entry = {
        .mnemonic = "NOP",        // Default mnemonic
        .mode = OP_ADDR_NOT_INIT, // Default addressing mode
        .size = 255,              // Default size
        .cycles = 255             // Default cycles
    };

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            op_code_matrix[i][j] = default_entry;
        }
    }

    set_opcode_matrix_entry(&op_code_matrix[0x0][0x0], "BRK", OP_ADDR_IMPLIED, 1, 7, 0);
    set_opcode_matrix_entry(&op_code_matrix[0x1][0x0], "BPL", OP_ADDR_IMPLIED, 2, 7, 2);
    set_opcode_matrix_entry(&op_code_matrix[0x2][0x0], "JSR", OP_ADDR_ABSOLUTE, 3, 6, 0);
    set_opcode_matrix_entry(&op_code_matrix[0x3][0x0], "BMI", OP_ADDR_RELATIVE, 2, 2, 2);
    set_opcode_matrix_entry(&op_code_matrix[0x4][0x0], "RTI", OP_ADDR_RELATIVE, 1, 6, 0);
    set_opcode_matrix_entry(&op_code_matrix[0x5][0x0], "BVC", OP_ADDR_RELATIVE, 2, 2, 2);
    set_opcode_matrix_entry(&op_code_matrix[0x6][0x0], "RTS", OP_ADDR_IMPLIED, 1, 6, 0);
    set_opcode_matrix_entry(&op_code_matrix[0x7][0x0], "BVS", OP_ADDR_RELATIVE, 2, 2, 2);

    set_opcode_matrix_entry(&op_code_matrix[0xA][0x9], "LDA", OP_ADDR_IMM, 2, 2, 0);

    printf("Entry at (0, 0):\n");
    printf("Mnemonic: %s\n", op_code_matrix[0][0].mnemonic);
    printf("Mode: %d\n", op_code_matrix[0][0].mode);
    printf("Size: %d\n", op_code_matrix[0][0].size);
    printf("Cycles: %d\n", op_code_matrix[0][0].cycles);

    return 0;
}