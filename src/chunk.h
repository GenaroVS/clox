#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "values.h"

typedef enum {
    OP_RETURN,
    OP_CONSTANT,
} OpCode;

typedef struct {
    size_t count;
    size_t capacity;
    int* line;
} Lines;

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t* code;
    Lines lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);

void initLines(Lines* lines);
void writeLines(Lines* lines, int line);
void freeLines(Lines* lines);

int addConstant(Chunk* chunk, Value value);
int getLine(Chunk* chunk, int offset);

#endif
