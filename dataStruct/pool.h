#ifndef COMM_DATASTRUCTURE_POOL
#define COMM_DATASTRUCTURE_POOL
#include <stdlib.h>
#include <stdint.h>
typedef struct CommPool{
	uint64_t capacity;
	size_t size;
	uint64_t stack_top;
	uint64_t *free_stack;
	struct CommPool *nxt;
	char pool[];
} CommPool;

CommPool *commPool_new(size_t size, uint64_t capacity);
int commPool_del(CommPool *pool);
void *commPool_get(CommPool *pool);
int commPool_free(CommPool *pool, void *item);

#endif
