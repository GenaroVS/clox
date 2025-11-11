#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initLines(&chunk->lines);
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLines(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
                oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    writeLines(&chunk->lines, line);
    chunk->count++;
}

// for debugging
static void printLines(Lines* lines) {
    printf("=== Line Array ===\n");
    int* array = lines->line;
    size_t count = lines->count;
    for (size_t i = 0; i < count - 1; i += 2) {
        printf("Line: %d\n", array[i]);
        printf("Freq: %d\n", array[i + 1]);
        printf("\n");
    }
}

void initLines(Lines* lines) {
    lines->count = 0;
    lines->capacity = 0;
    lines->line = NULL;
}

void freeLines(Lines* lines) {
    FREE_ARRAY(int, lines->line, lines->capacity);
    initLines(lines);
}

void writeLines(Lines* lines, int line) {
    int lastLine = lines->count >= 2 ? lines->line[lines->count - 2] : -1;
    if (line == lastLine) {
        int lastLineFreq = lines->line[lines->count - 1];
        lines->line[lines->count - 1] = lastLineFreq + 1;
    } else {
        if (lines->capacity < lines->count + 2) {
            int oldCapacity = lines->capacity;
            lines->capacity = GROW_CAPACITY(oldCapacity);
            lines->line = GROW_ARRAY(int, lines->line, oldCapacity, lines->capacity);
        }
        lines->line[lines->count] = line;
        lines->line[lines->count + 1] = 1;
        lines->count += 2;
    }   
    printLines(lines);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int offset) {
    int code_count = 0;
    size_t i = 0;
    int line = 0;
    int* lineArray = chunk->lines.line;
    while (code_count < offset + 1 && i < chunk->lines.count - 1) {
        line = lineArray[i];
        code_count += lineArray[i + 1];
        i += 2;
    }
    return line;
}

