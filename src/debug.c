#include <stdio.h>

#include "debug.h"
#include "values.h"

static int simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constantOffset = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constantOffset);
    printValue(chunk->constants.values[constantOffset]);
    printf("'\n");
    return offset + 2;
}

static int constantLongInstruction(const char* name, Chunk* chunk, int offset) {
    int constantOffset = packBytes(chunk->code, chunk->count, offset + 1, offset + 3);
    printf("%-16s %4d '", name, constantOffset);
    printValue(chunk->constants.values[constantOffset]);
    printf("'\n");
    return offset + 4;
}

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (size_t offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    int prevLine = getLine(chunk, offset > 0 ? offset - 1 : 0);
    int curLine = getLine(chunk, offset);

    if (offset > 0 && prevLine == curLine) {
        printf("   | ");
    } else {
        printf("%4d ", curLine);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_L:
            return constantLongInstruction("OP_CONSTANT_L", chunk, offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        default:
            printf("Unknown OpCode %d\n", instruction);
            return offset + 1;
    }
}

