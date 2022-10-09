#ifdef STACK_TYPE

#include <malloc.h>

#define WITH_TYPE_PREFIX(name, T) T ## name
#define STACK(T) WITH_TYPE_PREFIX(Stack, T)

typedef struct {
    size_t size;
    size_t backing_array_size;
    STACK_TYPE backing_array[];
} STACK(STACK_TYPE);



#define STACK_CREATE(T) WITH_TYPE_PREFIX(stack_create, T)

STACK(STACK_TYPE)* STACK_CREATE(STACK_TYPE)(size_t size)
{
    STACK(STACK_TYPE) *stack;
    stack = malloc(sizeof(*stack) + sizeof(STACK_TYPE) * size);

    stack->backing_array_size = size;
    stack->size = 0;

    return stack;
}



#define STACK_PUSH(T) WITH_TYPE_PREFIX(stack_push, T)

void STACK_PUSH(STACK_TYPE)(STACK(STACK_TYPE) *stack, STACK_TYPE element)
{
    stack->backing_array[stack->size++] = element;
}



#define STACK_POP(T) WITH_TYPE_PREFIX(stack_pop, T)

STACK_TYPE STACK_POP(STACK_TYPE)(STACK(STACK_TYPE) *stack)
{
    return stack->backing_array[--stack->size];
}



#define STACK_PEEK(T) WITH_TYPE_PREFIX(stack_peek, T)

STACK_TYPE STACK_PEEK(STACK_TYPE)(STACK(STACK_TYPE) *stack)
{
    return stack->backing_array[stack->size - 1];
}



#define STACK_LENGTH(T) WITH_TYPE_PREFIX(stack_length, T)

size_t STACK_LENGTH(STACK_TYPE)(STACK(STACK_TYPE) *stack)
{
    return stack->size;
}



#define STACK_FCONTAINS(T) WITH_TYPE_PREFIX(stack_fcontains, T)
#define STACK_EQ_FUNCTION(T) WITH_TYPE_PREFIX(EqualityFunction, T)

typedef int (*STACK_EQ_FUNCTION(STACK_TYPE))(STACK_TYPE, STACK_TYPE);

int STACK_FCONTAINS(STACK_TYPE)(STACK(STACK_TYPE) *stack, STACK_TYPE target, STACK_EQ_FUNCTION(STACK_TYPE) areEqual)
{
    for (int i = 0; i < stack->size; i++) {
        if (areEqual(stack->backing_array[i], target)) return 1;
    }
    return 0;
}



#define STACK_DESTROY(T) WITH_TYPE_PREFIX(stack_destroy, T)

void STACK_DESTROY(STACK_TYPE)(STACK(STACK_TYPE) *stack)
{
    free(stack);
}



#define STACK_COMPLETE_DESTROY(T) WITH_TYPE_PREFIX(stack_complete_destroy, T)

void STACK_COMPLETE_DESTROY(STACK_TYPE)(STACK(STACK_TYPE) *stack)
{
    for (int i = 0; i < stack->size; i++)
    {
        free(STACK_POP(STACK_TYPE)(stack));
    }

    free(stack);
}

#endif