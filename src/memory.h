#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 32 ? 32 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

uint8_t getBytePart(int val, uint8_t part);
int packBytes(uint8_t* bytes, size_t size, size_t start, size_t end);
int pack24int(uint8_t byte1, uint8_t byte2, uint8_t byte3);

#endif
