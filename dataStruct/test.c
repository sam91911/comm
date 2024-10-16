#include "pool.h"
#include "rbtree.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int int_cmp(void *a, void *b, void *arg){
	return *(int *)a-*(int *)b;
}

void int_print(void *a, void *arg){
	printf("%d\t", *(int *)a);
	return;
}

int main(int argc, char ** argv){
	const int len = 1ULL<<19;
	int test[len];
	CommPool *pool = commPool_new(40, 1024);
	CommRBTree rbtree;
	commRBTree_init(&rbtree, int_cmp, int_print, 0, pool);
	for(int i = 0; i < len; i++){
		test[i] = i;
	}
	for(int i = 0; i < len; i++){
		commRBTree_insert(&rbtree, test+i);
	}
	commPool_del(pool);
	return 0;
}
