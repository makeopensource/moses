#include <inttypes.h>
#include <moses/utils.h>
#include <moses/instructions.h>


/* executes group 0b01 instructions */
static void ex_group01(struct instr_01 instruction) {

}

/* executes group 0b10 instructions */
static void ex_group02(struct instr_02 instruction) {

}

/* executes group 0b00 instructions */
static void ex_group03(struct instr_03 instruction) {

}

/* executes conditional branches instructions */
static void ex_cond(struct instr_cond instruction) {

}

/* executes the others, which aren't super easy to group */
static void ex_unformatted(uint8_t instruction) {

}

/*
 * adds general format for parsing 6502 instructions, including
 * setting up skeleton code for each group's instruction execution.
 *
 * format for instructions in groups 1,2, 3: aaabbbcc, where aaa & cc are the
 * op code, and bbb is the addressing mode.
 *
 * format for conditional instructions: xxy10000, where xx encodes the status flag
 * and y encodes the bit stored there.
 *
 * other instructions will be handled case by case, because they don't follow
 * a reliable format.
 *
 * more info here: https://llx.com/Neil/a2/opcodes.html
 */

void parse_instruction(uint8_t opcode){
    uint8_t group = EXT(opcode, 0, 2);

    if (group == 1) {
        struct instr_01 instruction;
        instruction.op = EXT(opcode, 5, 3);
        instruction.mode = EXT(opcode, 2, 3);
        ex_group01(instruction);

    } else if (group == 2) {
        struct instr_02 instruction;
        instruction.op = EXT(opcode, 5, 3);
        instruction.mode = EXT(opcode, 2, 3);
        ex_group02(instruction);

    } else if (group == 0) {
        struct instr_03 instruction;
        instruction.op = EXT(opcode, 5, 3);
        instruction.mode = EXT(opcode, 2, 3);
        ex_group03(instruction);

    } else if (EXT(opcode,0,5) == 16) {
        struct instr_cond instruction;
        instruction.flag = EXT(opcode, 6, 2);
        instruction.bit = BIT(opcode, 5);
        ex_cond(instruction);

    } else {
        ex_unformatted(opcode);
    }
}