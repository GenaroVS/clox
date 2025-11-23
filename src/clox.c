#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "vm.h"
#include "chunk.h"
#include "debug.h"

static void repl() {
  char line[1024];
  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
      fprintf(stderr, "Could not open file \"%s\".\n", path);
      exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL) {
      fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
      exit(74);
  }
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
      fprintf(stderr, "Could not read file \"%s\".\n", path);
      exit(74);
  }
  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

static void runFile(const char* path) {
  char* source = readFile(path);
  InterpretRes result = interpret(source);
  free(source); 

  if (result == INTERPRET_COMPILE_ERROR) exit(65);
  if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[1]) {
    initVM();
//    Chunk c;
//    initChunk(&c);
//    for (int i = 0; i < 260; i++) {
//        writeConstant(&c, i, 1);
//    }
//    writeConstant(&c, 3, 1);
//    writeConstant(&c, 4, 1);
//    writeChunk(&c, OP_ADD, 1);
//    writeConstant(&c, 2, 1);
//    writeChunk(&c, OP_DIV, 1);
//    writeChunk(&c, OP_RETURN, 2);
//    disassembleChunk(&c, "test chunk");
//    vm_interpret(&c);

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }
    freeVM();
    return 0;
}
