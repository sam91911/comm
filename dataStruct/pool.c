#include "pool.h"

CommPool *commPool_new(size_t size, uint64_t capacity){
	if(capacity == 0) return 0;
	if(size == 0) return 0;

	//ceiling the size to multiplier of 8
	const size_t real_size = (size+7)&(~7);

	//ceiling the capacity to the power of 2
	const int capacity_bit = 64-__builtin_clzll(capacity-1);
	const uint64_t real_capacity = (1ULL<<capacity_bit);

	CommPool *target = malloc(real_size*real_capacity+40);

	target->capacity = real_capacity;
	target->size = real_size;
	target->stack_top = 0;
	target->free_stack = NULL;
	target->nxt = NULL;
	return target;
}
int commPool_del(CommPool *pool){
	if(!pool) return -1;
	CommPool *cur = pool;
	CommPool *nxt = pool->nxt;
	while(cur != NULL){
		//remeber next address, which will discard when current pool freed
		nxt = cur->nxt;
		//free current pool
		free(cur);
		//go next block
		cur = nxt;
	}
	return 0;
}
void *commPool_get(CommPool *pool){
	if(!pool) return 0;
	//check whether free_stack is empty
	if(pool->free_stack != NULL){
		//free_stack not empty, pop the block on free_stack

		//return address(target) is the top item in free_stack
		const uint64_t *target = pool->free_stack;
		//the address of next block in stack is store in the first block as type uint64_t
		pool->free_stack = (uint64_t *)(*target);
		//return uint64_t * to void * using type casting
		return (void *)target;
	}
	//check whether the pool is full
	if(pool->stack_top < pool->capacity){
		//pool not full

		//return address(target) is first item out of the stack_top, meanwhile increase the stack_top
		const char *target = pool->pool+((pool->stack_top)*pool->size);
		pool->stack_top++;
		//return char * to void * using type casting
		return (void *)target;
	}
	//pool is full, check next pool
	if(pool->nxt){
		return commPool_get(pool->nxt);
	}

	//create new pool and attech to next
	CommPool *new_pool = commPool_new(pool->size, pool->capacity*2);
	pool->nxt = new_pool;
	return commPool_get(new_pool);
}
int commPool_free(CommPool *pool, void *item){
	CommPool *cur = pool;
	char *target = item;
	//check all pool
	while(cur != NULL){
		//check whether target is in the current pool
		if((cur->pool <= target)&&(target < cur->pool+(cur->size*cur->capacity))){
			//treat target as uint64_t
			uint64_t *block = (uint64_t *)target;
			//store the address of free_stack head as uint64_t type
			block[0] = (uint64_t)cur->free_stack;
			cur->free_stack = block;
			return 0;
		}
		//go next
		cur = cur->nxt;
	}
	return -1;
}
