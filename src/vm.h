#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"

#define INIT_SIZE 256

typedef struct {
  Chunk* chunk;
  uint8_t* ip;
  ValueArray stack;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR, 
    INTERPRET_RUNTIME_ERROR,
} InterpretRes;

void initVM();
void freeVM();
InterpretRes vm_interpret(Chunk* chunk);

#endif
