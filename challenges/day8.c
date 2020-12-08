//
// Created by Michael on 08/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "day8.h"

typedef enum {
    ACC,
    JMP,
    NOP
} Cmd;

Cmd str2Cmd(const char *cmd) {
    switch (cmd[0]) {
        case 'a':
            return ACC;
        case 'j':
            return JMP;
        case 'n':
            return NOP;
        default:
            abort();
    }
}

typedef struct instruction {
    Cmd cmd;
    int val;
    int exec;
} Instruction;

static void readInput(Instruction **instructions, int *instructionCount) {
    FILE *input = fopen("../challenges/day8_boot_code.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *instructions = realloc(*instructions, ++*instructionCount * sizeof(Instruction));
        Instruction *ins = &(*instructions)[*instructionCount - 1];
        bzero(ins, sizeof (Instruction));
        char cmd[4];
        sscanf(buf, "%s %d", cmd, &ins->val);
        ins->cmd = str2Cmd(cmd);
    }
    fclose(input);
    free(buf);
}

static int err;
int needMoneyForQemu(Instruction *instructions, int instructionsCount) {
    err = 0;
    int acc = 0, ip = 0;
    while (ip < instructionsCount) {
        Instruction *ins = &instructions[ip];
        if (ins->exec) {
            err = 1;
            break;
        }
        ins->exec = 1;
        switch (ins->cmd) {
            case JMP:
                ip += ins->val;
                break;
            case ACC:
                acc += ins->val;
//                ++ip; break; // Asking for trouble :P
            case NOP:
                ++ip;
                break;
        }
    }
    return acc;
}

void resetExec(Instruction *instructions, int instructionsCount) {
    while (--instructionsCount > -1)
        instructions[instructionsCount].exec = 0;
}

void bootCode1() {
    Instruction *instructions = NULL;
    int instructionsCount = 0;
    readInput(&instructions, &instructionsCount);
    printf("Answer: %d\n", needMoneyForQemu(instructions, instructionsCount));
    assert(err == 1);
    free(instructions);
}

void bootCode2() {
    Instruction *instructions = NULL;
    int instructionCount = 0;
    readInput(&instructions, &instructionCount);
    int acc = 0;
    for (int i = 0; i < instructionCount; ++i) {
        Instruction *ins = &instructions[i];
        if (ins->cmd == ACC)
            continue;
        ins->cmd = ins->cmd == JMP ? NOP : JMP;
        acc = needMoneyForQemu(instructions, instructionCount);
        ins->cmd = ins->cmd == JMP ? NOP : JMP; // restore
        resetExec(instructions, instructionCount);
        if (!err)
            break;
    }
    printf("Answer: %d\n", acc);
    assert(err == 0);
    free(instructions);
}
