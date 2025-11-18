#include "common.h"
#include "vm.h"
#include "chunk.h"
#include "debug.h"

int main(void) {
    initVM();
    Chunk c;
    initChunk(&c);
//    for (int i = 0; i < 260; i++) {
//        writeConstant(&c, i, 1);
//    }
    writeConstant(&c, 3, 1);
    writeConstant(&c, 4, 1);
    writeChunk(&c, OP_ADD, 1);
    writeConstant(&c, 2, 1);
    writeChunk(&c, OP_DIV, 1);
    writeChunk(&c, OP_RETURN, 2);
    disassembleChunk(&c, "test chunk");
    vm_interpret(&c);
    freeVM();
    freeChunk(&c);
    return 0;
}
