#include <stdlib.h>

#include "memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if (!result) exit(0);
  return result;
}

uint8_t getBytePart(int val, uint8_t part) {
    if (sizeof(val) < part || part < 1) exit(0);
    int nshift = (part - 1) * 8;
    uint8_t byte = (uint8_t) ((val >> nshift) & 0xFF);
    return byte;
}

int packBytes(uint8_t* bytes, size_t size, size_t start, size_t end) {
    if (sizeof(int) < (end - start + 1) || end <= start || end >= size) exit(0);
    
    int val = 0;
    size_t i = end;
    while (i >= start) {
        uint8_t byte = bytes[i];
        int nshift = (i - start) * 8;
        val = (int) ((val | byte) << nshift);
        i--;
    }
    return val;
}

int pack24int(uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    int val = 0;
    val = (int) ((val | byte3) << 16); 
    val = (int) ((val | byte2) << 8); 
    val = (int) (val | byte1);
    return val;
}
