#include "stack.h"

int commStack_init(CommStack *stack, uint64_t len){
	if(!stack) return -1;
	commArray_init(&(stack->array), len, 8, 0);
	stack->stack_top = 0;
	return 0;
}
int commStack_del(CommStack *stack){
	if(!stack) return -1;
	commArray_del(&(stack->array));
	return 0;
}
int commStack_push(CommStack *stack, void *data){
	if(!stack) return -1;
	const uint64_t top = stack->stack_top++;
	if(commArray_adjust(&(stack->array), stack->stack_top) != 0){
		return -1;
	}
	uint64_t *content = (uint64_t *)stack->array.data;
	content[top] = (uint64_t)data;
	return 0;
}
void * commStacl_pop(CommStack *stack){
	if(!stack) return 0;
	const uint64_t top = --stack->stack_top;
	uint64_t *content = (uint64_t *)stack->array.data;
	return (void *)(content[top]);
}
