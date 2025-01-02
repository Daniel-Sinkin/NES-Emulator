#ifndef CPU_H
#define CPU_H

#include <BUS.h>
#include <UTIL.h>
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

void CPU_init(CPU *cpu, BUS *bus);

uint8_t CPU_read(CPU *cpu, uint16_t addr);
void CPU_write(CPU *cpu, uint16_t addr, uint8_t data);
uint8_t CPU_get_flag(CPU *cpu, CPU_FLAGS flag);
void CPU_set_flag(CPU *cpu, CPU_FLAGS flag);
void CPU_unset_flag(CPU *cpu, CPU_FLAGS flag);
void CPU_print_registers(CPU *cpu);
void CPU_read_pc(CPU *cpu);
void CPU_clock(CPU *cpu);

void CPU_reset(CPU *cpu);
void CPU_irq(CPU *cpu);
void CPU_nmi(CPU *cpu);
void CPU_fetch(CPU *cpu);

// Addressing mode functions
uint8_t CPU_AM_IMP(CPU *cpu);
uint8_t CPU_AM_ZP0(CPU *cpu);
uint8_t CPU_AM_ZPY(CPU *cpu);
uint8_t CPU_AM_ABS(CPU *cpu);
uint8_t CPU_AM_ABY(CPU *cpu);
uint8_t CPU_AM_IZX(CPU *cpu);
uint8_t CPU_AM_IMM(CPU *cpu);
uint8_t CPU_AM_ZPX(CPU *cpu);
uint8_t CPU_AM_REL(CPU *cpu);
uint8_t CPU_AM_ABX(CPU *cpu);
uint8_t CPU_AM_IND(CPU *cpu);
uint8_t CPU_AM_IZY(CPU *cpu);
uint8_t CPU_AM_XXX(CPU *cpu);

// opcode functions
uint8_t CPU_ADC(CPU *cpu);
uint8_t CPU_AND(CPU *cpu);
uint8_t CPU_ASL(CPU *cpu);
uint8_t CPU_BCC(CPU *cpu);
uint8_t CPU_BCS(CPU *cpu);
uint8_t CPU_BEQ(CPU *cpu);
uint8_t CPU_BIT(CPU *cpu);
uint8_t CPU_BMI(CPU *cpu);
uint8_t CPU_BNE(CPU *cpu);
uint8_t CPU_BPL(CPU *cpu);
uint8_t CPU_BRK(CPU *cpu);
uint8_t CPU_BVC(CPU *cpu);
uint8_t CPU_BVS(CPU *cpu);
uint8_t CPU_CLC(CPU *cpu);
uint8_t CPU_CLD(CPU *cpu);
uint8_t CPU_CLI(CPU *cpu);
uint8_t CPU_CLV(CPU *cpu);
uint8_t CPU_CMP(CPU *cpu);
uint8_t CPU_CPX(CPU *cpu);
uint8_t CPU_CPY(CPU *cpu);
uint8_t CPU_DEC(CPU *cpu);
uint8_t CPU_DEX(CPU *cpu);
uint8_t CPU_DEY(CPU *cpu);
uint8_t CPU_EOR(CPU *cpu);
uint8_t CPU_INC(CPU *cpu);
uint8_t CPU_INX(CPU *cpu);
uint8_t CPU_INY(CPU *cpu);
uint8_t CPU_JMP(CPU *cpu);
uint8_t CPU_JSR(CPU *cpu);
uint8_t CPU_LDA(CPU *cpu);
uint8_t CPU_LDX(CPU *cpu);
uint8_t CPU_LDY(CPU *cpu);
uint8_t CPU_LSR(CPU *cpu);
uint8_t CPU_NOP(CPU *cpu);
uint8_t CPU_ORA(CPU *cpu);
uint8_t CPU_PHA(CPU *cpu);
uint8_t CPU_PHP(CPU *cpu);
uint8_t CPU_PLA(CPU *cpu);
uint8_t CPU_PLP(CPU *cpu);
uint8_t CPU_ROL(CPU *cpu);
uint8_t CPU_ROR(CPU *cpu);
uint8_t CPU_RTI(CPU *cpu);
uint8_t CPU_RTS(CPU *cpu);
uint8_t CPU_SBC(CPU *cpu);
uint8_t CPU_SEC(CPU *cpu);
uint8_t CPU_SED(CPU *cpu);
uint8_t CPU_SEI(CPU *cpu);
uint8_t CPU_STA(CPU *cpu);
uint8_t CPU_STX(CPU *cpu);
uint8_t CPU_STY(CPU *cpu);
uint8_t CPU_TAX(CPU *cpu);
uint8_t CPU_TAY(CPU *cpu);
uint8_t CPU_TSX(CPU *cpu);
uint8_t CPU_TXA(CPU *cpu);
uint8_t CPU_TXS(CPU *cpu);
uint8_t CPU_TYA(CPU *cpu);
uint8_t CPU_XXX(CPU *cpu);

typedef uint8_t (*CPU_OpFunc)(CPU *cpu);
typedef uint8_t (*CPU_AMFunc)(CPU *cpu);

typedef struct {
    CPU_OpFunc op;
    CPU_AMFunc am;
    uint8_t cycles;
} OP_CODE_MATRIX_ENTRY;

// clang-format off
static const OP_CODE_MATRIX_ENTRY OP_CODE_MATRIX[] = {
	{ CPU_BRK, CPU_AM_IMM, 7 },{ CPU_ORA, CPU_AM_IZX, 6 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 3 },{ CPU_ORA, CPU_AM_ZP0, 3 },{ CPU_ASL, CPU_AM_ZP0, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_PHP, CPU_AM_IMP, 3 },{ CPU_ORA, CPU_AM_IMM, 2 },{ CPU_ASL, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_ORA, CPU_AM_ABS, 4 },{ CPU_ASL, CPU_AM_ABS, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },
	{ CPU_BPL, CPU_AM_REL, 2 },{ CPU_ORA, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_ORA, CPU_AM_ZPX, 4 },{ CPU_ASL, CPU_AM_ZPX, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_CLC, CPU_AM_IMP, 2 },{ CPU_ORA, CPU_AM_ABY, 4 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 7 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_ORA, CPU_AM_ABX, 4 },{ CPU_ASL, CPU_AM_ABX, 7 },{ CPU_XXX, CPU_AM_IMP, 7 },
	{ CPU_JSR, CPU_AM_ABS, 6 },{ CPU_AND, CPU_AM_IZX, 6 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_BIT, CPU_AM_ZP0, 3 },{ CPU_AND, CPU_AM_ZP0, 3 },{ CPU_ROL, CPU_AM_ZP0, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_PLP, CPU_AM_IMP, 4 },{ CPU_AND, CPU_AM_IMM, 2 },{ CPU_ROL, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_BIT, CPU_AM_ABS, 4 },{ CPU_AND, CPU_AM_ABS, 4 },{ CPU_ROL, CPU_AM_ABS, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },
	{ CPU_BMI, CPU_AM_REL, 2 },{ CPU_AND, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_AND, CPU_AM_ZPX, 4 },{ CPU_ROL, CPU_AM_ZPX, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_SEC, CPU_AM_IMP, 2 },{ CPU_AND, CPU_AM_ABY, 4 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 7 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_AND, CPU_AM_ABX, 4 },{ CPU_ROL, CPU_AM_ABX, 7 },{ CPU_XXX, CPU_AM_IMP, 7 },
	{ CPU_RTI, CPU_AM_IMP, 6 },{ CPU_EOR, CPU_AM_IZX, 6 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 3 },{ CPU_EOR, CPU_AM_ZP0, 3 },{ CPU_LSR, CPU_AM_ZP0, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_PHA, CPU_AM_IMP, 3 },{ CPU_EOR, CPU_AM_IMM, 2 },{ CPU_LSR, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_JMP, CPU_AM_ABS, 3 },{ CPU_EOR, CPU_AM_ABS, 4 },{ CPU_LSR, CPU_AM_ABS, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },
	{ CPU_BVC, CPU_AM_REL, 2 },{ CPU_EOR, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_EOR, CPU_AM_ZPX, 4 },{ CPU_LSR, CPU_AM_ZPX, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_CLI, CPU_AM_IMP, 2 },{ CPU_EOR, CPU_AM_ABY, 4 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 7 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_EOR, CPU_AM_ABX, 4 },{ CPU_LSR, CPU_AM_ABX, 7 },{ CPU_XXX, CPU_AM_IMP, 7 },
	{ CPU_RTS, CPU_AM_IMP, 6 },{ CPU_ADC, CPU_AM_IZX, 6 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 3 },{ CPU_ADC, CPU_AM_ZP0, 3 },{ CPU_ROR, CPU_AM_ZP0, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_PLA, CPU_AM_IMP, 4 },{ CPU_ADC, CPU_AM_IMM, 2 },{ CPU_ROR, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_JMP, CPU_AM_IND, 5 },{ CPU_ADC, CPU_AM_ABS, 4 },{ CPU_ROR, CPU_AM_ABS, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },
	{ CPU_BVS, CPU_AM_REL, 2 },{ CPU_ADC, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_ADC, CPU_AM_ZPX, 4 },{ CPU_ROR, CPU_AM_ZPX, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_SEI, CPU_AM_IMP, 2 },{ CPU_ADC, CPU_AM_ABY, 4 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 7 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_ADC, CPU_AM_ABX, 4 },{ CPU_ROR, CPU_AM_ABX, 7 },{ CPU_XXX, CPU_AM_IMP, 7 },
	{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_STA, CPU_AM_IZX, 6 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_STY, CPU_AM_ZP0, 3 },{ CPU_STA, CPU_AM_ZP0, 3 },{ CPU_STX, CPU_AM_ZP0, 3 },{ CPU_XXX, CPU_AM_IMP, 3 },{ CPU_DEY, CPU_AM_IMP, 2 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_TXA, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_STY, CPU_AM_ABS, 4 },{ CPU_STA, CPU_AM_ABS, 4 },{ CPU_STX, CPU_AM_ABS, 4 },{ CPU_XXX, CPU_AM_IMP, 4 },
	{ CPU_BCC, CPU_AM_REL, 2 },{ CPU_STA, CPU_AM_IZY, 6 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_STY, CPU_AM_ZPX, 4 },{ CPU_STA, CPU_AM_ZPX, 4 },{ CPU_STX, CPU_AM_ZPY, 4 },{ CPU_XXX, CPU_AM_IMP, 4 },{ CPU_TYA, CPU_AM_IMP, 2 },{ CPU_STA, CPU_AM_ABY, 5 },{ CPU_TXS, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_NOP, CPU_AM_IMP, 5 },{ CPU_STA, CPU_AM_ABX, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },
	{ CPU_LDY, CPU_AM_IMM, 2 },{ CPU_LDA, CPU_AM_IZX, 6 },{ CPU_LDX, CPU_AM_IMM, 2 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_LDY, CPU_AM_ZP0, 3 },{ CPU_LDA, CPU_AM_ZP0, 3 },{ CPU_LDX, CPU_AM_ZP0, 3 },{ CPU_XXX, CPU_AM_IMP, 3 },{ CPU_TAY, CPU_AM_IMP, 2 },{ CPU_LDA, CPU_AM_IMM, 2 },{ CPU_TAX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_LDY, CPU_AM_ABS, 4 },{ CPU_LDA, CPU_AM_ABS, 4 },{ CPU_LDX, CPU_AM_ABS, 4 },{ CPU_XXX, CPU_AM_IMP, 4 },
	{ CPU_BCS, CPU_AM_REL, 2 },{ CPU_LDA, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_LDY, CPU_AM_ZPX, 4 },{ CPU_LDA, CPU_AM_ZPX, 4 },{ CPU_LDX, CPU_AM_ZPY, 4 },{ CPU_XXX, CPU_AM_IMP, 4 },{ CPU_CLV, CPU_AM_IMP, 2 },{ CPU_LDA, CPU_AM_ABY, 4 },{ CPU_TSX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 4 },{ CPU_LDY, CPU_AM_ABX, 4 },{ CPU_LDA, CPU_AM_ABX, 4 },{ CPU_LDX, CPU_AM_ABY, 4 },{ CPU_XXX, CPU_AM_IMP, 4 },
	{ CPU_CPY, CPU_AM_IMM, 2 },{ CPU_CMP, CPU_AM_IZX, 6 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_CPY, CPU_AM_ZP0, 3 },{ CPU_CMP, CPU_AM_ZP0, 3 },{ CPU_DEC, CPU_AM_ZP0, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_INY, CPU_AM_IMP, 2 },{ CPU_CMP, CPU_AM_IMM, 2 },{ CPU_DEX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_CPY, CPU_AM_ABS, 4 },{ CPU_CMP, CPU_AM_ABS, 4 },{ CPU_DEC, CPU_AM_ABS, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },
	{ CPU_BNE, CPU_AM_REL, 2 },{ CPU_CMP, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_CMP, CPU_AM_ZPX, 4 },{ CPU_DEC, CPU_AM_ZPX, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_CLD, CPU_AM_IMP, 2 },{ CPU_CMP, CPU_AM_ABY, 4 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 7 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_CMP, CPU_AM_ABX, 4 },{ CPU_DEC, CPU_AM_ABX, 7 },{ CPU_XXX, CPU_AM_IMP, 7 },
	{ CPU_CPX, CPU_AM_IMM, 2 },{ CPU_SBC, CPU_AM_IZX, 6 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_CPX, CPU_AM_ZP0, 3 },{ CPU_SBC, CPU_AM_ZP0, 3 },{ CPU_INC, CPU_AM_ZP0, 5 },{ CPU_XXX, CPU_AM_IMP, 5 },{ CPU_INX, CPU_AM_IMP, 2 },{ CPU_SBC, CPU_AM_IMM, 2 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_SBC, CPU_AM_IMP, 2 },{ CPU_CPX, CPU_AM_ABS, 4 },{ CPU_SBC, CPU_AM_ABS, 4 },{ CPU_INC, CPU_AM_ABS, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },
	{ CPU_BEQ, CPU_AM_REL, 2 },{ CPU_SBC, CPU_AM_IZY, 5 },{ CPU_XXX, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 8 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_SBC, CPU_AM_ZPX, 4 },{ CPU_INC, CPU_AM_ZPX, 6 },{ CPU_XXX, CPU_AM_IMP, 6 },{ CPU_SED, CPU_AM_IMP, 2 },{ CPU_SBC, CPU_AM_ABY, 4 },{ CPU_NOP, CPU_AM_IMP, 2 },{ CPU_XXX, CPU_AM_IMP, 7 },{ CPU_NOP, CPU_AM_IMP, 4 },{ CPU_SBC, CPU_AM_ABX, 4 },{ CPU_INC, CPU_AM_ABX, 7 },{ CPU_XXX, CPU_AM_IMP, 7 },
};
// clang-format on

#endif // CPU_H