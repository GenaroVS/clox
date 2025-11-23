#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "memory.h"
#include "debug.h"

VM vm; 

#define VM_STACK_TOP vm.stack.values[vm.stack.count]

void resetStack() {
    vm.stack.count = 0;
}

static void push(Value val) {
    ValueArray* array = &vm.stack;
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values,
                oldCapacity, array->capacity);
    }
    VM_STACK_TOP = val;
    array->count++;
}

static Value pop() {
    vm.stack.count--;
    return VM_STACK_TOP;
}

void initVM() {
    initValueArrayOfCapacity(&vm.stack, INIT_SIZE);
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
        for (Value* pos = vm.stack.values; pos < vm.stack.values + vm.stack.count; pos++) {
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
                VM_STACK_TOP = -VM_STACK_TOP;
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

InterpretRes interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}

InterpretRes vm_interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

