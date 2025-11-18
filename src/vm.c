#include "common.h"
#include "vm.h"
#include "memory.h"
#include "debug.h"

VM vm; 

void resetStack() {
    vm.stackTop = vm.stack;
}

static void push(Value val) {
    *vm.stackTop = val;
    vm.stackTop++;
}

static Value pop() {
    return *(--vm.stackTop);
}

void initVM() {
    resetStack();
}

void freeVM() {

}

static InterpretRes run() {

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()]);
#define BINARY_OP(op) \
    do {              \
        double right = pop(); \
        double left = pop();  \
        push(left op right);  \
    } while(false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("        ");
        for (Value* pos = vm.stack; pos < vm.stackTop; pos++) {
            printf("[ ");
            printValue(*pos);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                break;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_CONSTANT_L: {
                int offset = pack24int(READ_BYTE(), READ_BYTE(), READ_BYTE());
                Value constant = vm.chunk->constants.values[offset];
                push(constant);
                break;
            }
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_ADD: {
                BINARY_OP(+);
                break;
            }
            case OP_SUB: {
                BINARY_OP(-);
                break;
            }
            case OP_MULT: {
                BINARY_OP(*);
                break;
            }
            case OP_DIV: {
                BINARY_OP(/);
                break;
            }
        }
        if (vm.ip >= &(vm.chunk->code[vm.chunk->count])) {
            return INTERPRET_OK;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP 
}

InterpretRes vm_interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

