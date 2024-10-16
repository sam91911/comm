#ifndef COMM_DATASTRUCTURE_STACK
#define COMM_DATASTRUCTURE_STACK
#include <stdlib.h>
#include <stdint.h>
#include "array.h"

typedef struct{
	uint64_t stack_top;
	CommArray array;
} CommStack;

int commStack_init(CommStack *stack, uint64_t len);
int commStack_del(CommStack *stack);
int commStack_push(CommStack *stack, void *data);
void * commStacl_pop(CommStack *stack);

#endif
