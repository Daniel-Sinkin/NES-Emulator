#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Addressing mode functions
uint8_t f_AM_IMP() {
    printf("Adressing mode function works!\n");
    return 0;
}
uint8_t f_AM_ZP0();
uint8_t f_AM_ZPY();
uint8_t f_AM_ABS();
uint8_t f_AM_ABY();
uint8_t f_AM_IZX();
uint8_t f_AM_IMM();
uint8_t f_AM_ZPX();
uint8_t f_AM_REL();
uint8_t f_AM_ABX();
uint8_t f_AM_IND();
uint8_t f_AM_IZY();
uint8_t f_AM_XXX() {
    PANIC("ILLEGAL Adressing mode hit!");
} // Illegal Addressing mode

// opcode functions
uint8_t f_OP_ADC();
uint8_t f_OP_AND();
uint8_t f_OP_ASL();
uint8_t f_OP_BCC();
uint8_t f_OP_BCS();
uint8_t f_OP_BEQ();
uint8_t f_OP_BIT();
uint8_t f_OP_BMI();
uint8_t f_OP_BNE();
uint8_t f_OP_BPL();
uint8_t f_OP_BRK() {
    printf("OPCODE function works!\n");
    return 0;
}
uint8_t f_OP_BVC();
uint8_t f_OP_BVS();
uint8_t f_OP_CLC();
uint8_t f_OP_CLD();
uint8_t f_OP_CLI();
uint8_t f_OP_CLV();
uint8_t f_OP_CMP();
uint8_t f_OP_CPX();
uint8_t f_OP_CPY();
uint8_t f_OP_DEC();
uint8_t f_OP_DEX();
uint8_t f_OP_DEY();
uint8_t f_OP_EOR();
uint8_t f_OP_INC();
uint8_t f_OP_INX();
uint8_t f_OP_INY();
uint8_t f_OP_JMP();
uint8_t f_OP_JSR();
uint8_t f_OP_LDA();
uint8_t f_OP_LDX();
uint8_t f_OP_LDY();
uint8_t f_OP_LSR();
uint8_t f_OP_NOP();
uint8_t f_OP_ORA();
uint8_t f_OP_PHA();
uint8_t f_OP_PHP();
uint8_t f_OP_PLA();
uint8_t f_OP_PLP();
uint8_t f_OP_ROL();
uint8_t f_OP_ROR();
uint8_t f_OP_RTI();
uint8_t f_OP_RTS();
uint8_t f_OP_SBC();
uint8_t f_OP_SEC();
uint8_t f_OP_SED();
uint8_t f_OP_SEI();
uint8_t f_OP_STA();
uint8_t f_OP_STX();
uint8_t f_OP_STY();
uint8_t f_OP_TAX();
uint8_t f_OP_TAY();
uint8_t f_OP_TSX();
uint8_t f_OP_TXA();
uint8_t f_OP_TXS();
uint8_t f_OP_TYA();
uint8_t f_OP_XXX() {
    PANIC("OP_XXX HIT!");
} // Illegal op code

// Function ptr types
typedef uint8_t (*opcode_func)(void);
typedef uint8_t (*addressing_func)(void);

typedef struct {
    opcode_func op;     // Pointer to the CPU opcode handler
    addressing_func am; // Pointer to the addressing-mode function
    uint8_t cycles;     // Number of cycles
} OPCODE_MATRIX_ENTRY;

static inline void set_opcode_matrix_entry(
    OPCODE_MATRIX_ENTRY *entry,
    opcode_func op,
    addressing_func am,
    uint8_t cycles) {
    entry->op = op;
    entry->am = am;
    entry->cycles = cycles;
}

void op_code_matrix_fill(OPCODE_MATRIX_ENTRY op_code_matrix[16][16]) {
    // Heavily modified from this: https://github.com/OneLoneCoder/olcNES/blob/master/Part%232%20-%20CPU/olc6502.cpp

    // Default entry that can serve as a placeholder
    OPCODE_MATRIX_ENTRY default_entry = {
        .op = f_OP_XXX,
        .am = f_AM_XXX,
        .cycles = 255};

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            op_code_matrix[i][j] = default_entry;
        }
    }

    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x0],
        f_OP_BRK, f_AM_IMM,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x1],
        f_OP_ORA, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x4],
        f_OP_XXX, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x5],
        f_OP_ORA, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x6],
        f_OP_ASL, f_AM_ZP0,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x7],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x8],
        f_OP_PHP, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0x9],
        f_OP_ORA, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0xA],
        f_OP_ASL, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0xD],
        f_OP_ORA, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0xE],
        f_OP_ASL, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x0][0xF],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x0],
        f_OP_BPL, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x1],
        f_OP_ORA, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x4],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x5],
        f_OP_ORA, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x6],
        f_OP_ASL, f_AM_ZPX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x7],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x8],
        f_OP_CLC, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0x9],
        f_OP_ORA, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0xA],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0xB],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0xD],
        f_OP_ORA, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0xE],
        f_OP_ASL, f_AM_ABX,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x1][0xF],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x0],
        f_OP_JSR, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x1],
        f_OP_AND, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x4],
        f_OP_BIT, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x5],
        f_OP_AND, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x6],
        f_OP_ROL, f_AM_ZP0,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x7],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x8],
        f_OP_PLP, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0x9],
        f_OP_AND, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0xA],
        f_OP_ROL, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0xC],
        f_OP_BIT, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0xD],
        f_OP_AND, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0xE],
        f_OP_ROL, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x2][0xF],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x0],
        f_OP_BMI, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x1],
        f_OP_AND, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x4],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x5],
        f_OP_AND, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x6],
        f_OP_ROL, f_AM_ZPX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x7],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x8],
        f_OP_SEC, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0x9],
        f_OP_AND, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0xA],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0xB],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0xD],
        f_OP_AND, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0xE],
        f_OP_ROL, f_AM_ABX,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x3][0xF],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x0],
        f_OP_RTI, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x1],
        f_OP_EOR, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x4],
        f_OP_XXX, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x5],
        f_OP_EOR, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x6],
        f_OP_LSR, f_AM_ZP0,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x7],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x8],
        f_OP_PHA, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0x9],
        f_OP_EOR, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0xA],
        f_OP_LSR, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0xC],
        f_OP_JMP, f_AM_ABS,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0xD],
        f_OP_EOR, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0xE],
        f_OP_LSR, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x4][0xF],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x0],
        f_OP_BVC, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x1],
        f_OP_EOR, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x4],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x5],
        f_OP_EOR, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x6],
        f_OP_LSR, f_AM_ZPX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x7],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x8],
        f_OP_CLI, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0x9],
        f_OP_EOR, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0xA],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0xB],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0xD],
        f_OP_EOR, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0xE],
        f_OP_LSR, f_AM_ABX,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x5][0xF],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x0],
        f_OP_RTS, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x1],
        f_OP_ADC, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x4],
        f_OP_XXX, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x5],
        f_OP_ADC, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x6],
        f_OP_ROR, f_AM_ZP0,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x7],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x8],
        f_OP_PLA, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0x9],
        f_OP_ADC, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0xA],
        f_OP_ROR, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0xC],
        f_OP_JMP, f_AM_IND,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0xD],
        f_OP_ADC, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0xE],
        f_OP_ROR, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x6][0xF],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x0],
        f_OP_BVS, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x1],
        f_OP_ADC, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x4],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x5],
        f_OP_ADC, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x6],
        f_OP_ROR, f_AM_ZPX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x7],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x8],
        f_OP_SEI, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0x9],
        f_OP_ADC, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0xA],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0xB],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0xD],
        f_OP_ADC, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0xE],
        f_OP_ROR, f_AM_ABX,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x7][0xF],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x0],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x1],
        f_OP_STA, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x3],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x4],
        f_OP_STY, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x5],
        f_OP_STA, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x6],
        f_OP_STX, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x7],
        f_OP_XXX, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x8],
        f_OP_DEY, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0x9],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0xA],
        f_OP_TXA, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0xC],
        f_OP_STY, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0xD],
        f_OP_STA, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0xE],
        f_OP_STX, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x8][0xF],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x0],
        f_OP_BCC, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x1],
        f_OP_STA, f_AM_IZY,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x3],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x4],
        f_OP_STY, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x5],
        f_OP_STA, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x6],
        f_OP_STX, f_AM_ZPY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x7],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x8],
        f_OP_TYA, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0x9],
        f_OP_STA, f_AM_ABY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0xA],
        f_OP_TXS, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0xB],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0xC],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0xD],
        f_OP_STA, f_AM_ABX,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0xE],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0x9][0xF],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x0],
        f_OP_LDY, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x1],
        f_OP_LDA, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x2],
        f_OP_LDX, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x3],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x4],
        f_OP_LDY, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x5],
        f_OP_LDA, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x6],
        f_OP_LDX, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x7],
        f_OP_XXX, f_AM_IMP,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x8],
        f_OP_TAY, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0x9],
        f_OP_LDA, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0xA],
        f_OP_TAX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0xC],
        f_OP_LDY, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0xD],
        f_OP_LDA, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0xE],
        f_OP_LDX, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xA][0xF],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x0],
        f_OP_BCS, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x1],
        f_OP_LDA, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x3],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x4],
        f_OP_LDY, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x5],
        f_OP_LDA, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x6],
        f_OP_LDX, f_AM_ZPY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x7],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x8],
        f_OP_CLV, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0x9],
        f_OP_LDA, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0xA],
        f_OP_TSX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0xB],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0xC],
        f_OP_LDY, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0xD],
        f_OP_LDA, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0xE],
        f_OP_LDX, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xB][0xF],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x0],
        f_OP_CPY, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x1],
        f_OP_CMP, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x4],
        f_OP_CPY, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x5],
        f_OP_CMP, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x6],
        f_OP_DEC, f_AM_ZP0,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x7],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x8],
        f_OP_INY, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0x9],
        f_OP_CMP, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0xA],
        f_OP_DEX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0xC],
        f_OP_CPY, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0xD],
        f_OP_CMP, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0xE],
        f_OP_DEC, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xC][0xF],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x0],
        f_OP_BNE, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x1],
        f_OP_CMP, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x4],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x5],
        f_OP_CMP, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x6],
        f_OP_DEC, f_AM_ZPX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x7],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x8],
        f_OP_CLD, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0x9],
        f_OP_CMP, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0xA],
        f_OP_NOP, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0xB],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0xD],
        f_OP_CMP, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0xE],
        f_OP_DEC, f_AM_ABX,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0xD][0xF],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x0],
        f_OP_CPX, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x1],
        f_OP_SBC, f_AM_IZX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x4],
        f_OP_CPX, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x5],
        f_OP_SBC, f_AM_ZP0,
        3);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x6],
        f_OP_INC, f_AM_ZP0,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x7],
        f_OP_XXX, f_AM_IMP,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x8],
        f_OP_INX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0x9],
        f_OP_SBC, f_AM_IMM,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0xA],
        f_OP_NOP, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0xB],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0xC],
        f_OP_CPX, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0xD],
        f_OP_SBC, f_AM_ABS,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0xE],
        f_OP_INC, f_AM_ABS,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xE][0xF],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x0],
        f_OP_BEQ, f_AM_REL,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x1],
        f_OP_SBC, f_AM_IZY,
        5);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x2],
        f_OP_XXX, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x3],
        f_OP_XXX, f_AM_IMP,
        8);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x4],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x5],
        f_OP_SBC, f_AM_ZPX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x6],
        f_OP_INC, f_AM_ZPX,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x7],
        f_OP_XXX, f_AM_IMP,
        6);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x8],
        f_OP_SED, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0x9],
        f_OP_SBC, f_AM_ABY,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0xA],
        f_OP_NOP, f_AM_IMP,
        2);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0xB],
        f_OP_XXX, f_AM_IMP,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0xC],
        f_OP_XXX, f_AM_IMP,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0xD],
        f_OP_SBC, f_AM_ABX,
        4);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0xE],
        f_OP_INC, f_AM_ABX,
        7);
    set_opcode_matrix_entry(
        &op_code_matrix[0xF][0xF],
        f_OP_XXX, f_AM_IMP,
        7);
}

void opcode_matrix_example() {
    OPCODE_MATRIX_ENTRY op_code_matrix[16][16];
    op_code_matrix_fill(op_code_matrix);

    printf("Calling op at (0,0)...\n");
    uint8_t result_opcode = op_code_matrix[0][0].op();
    uint8_t result_addressing = op_code_matrix[0][0].am();

    printf("Opcode function returned:      %u\n", result_opcode);
    printf("Addressing mode function returned: %u\n", result_addressing);

    printf("Cycles: %d\n", op_code_matrix[0][0].cycles);
}