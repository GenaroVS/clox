#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(void) {
    Chunk c;
    initChunk(&c);
    writeChunk(&c, OP_RETURN, 0);
    writeChunk(&c, OP_CONSTANT, 1);
    int constantOffset = addConstant(&c, 1.5);
    writeChunk(&c, constantOffset, 1);
    disassembleChunk(&c, "test chunk");
    freeChunk(&c);
    return 0;
}
