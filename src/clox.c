#include "common.h"
#include "vm.h"
#include "chunk.h"
#include "debug.h"

int main(void) {
    initVM();
    Chunk c;
    initChunk(&c);
    writeChunk(&c, OP_RETURN, 0);
    for (int i = 0; i < 260; i++) {
        writeConstant(&c, i, 1);
    }
    disassembleChunk(&c, "test chunk");
    vm_interpret(&c);
    freeVM();
    freeChunk(&c);
    return 0;
}
