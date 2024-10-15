#include "pool.h"
#include "rbtree.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int int_cmp(void *a, void *b){
	return *(int *)a-*(int *)b;
}

void int_print(void *a){
	printf("%d\n", *(int *)a);
	return;
}

int main(int argc, char ** argv){
	const int len = 32;
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
	commRBTree_remove(&rbtree, test+12);
	commRBTree_remove(&rbtree, test+21);
	commRBTree_remove(&rbtree, test+21);
	commRBTree_print(&rbtree, 0x01);
	printf("\n");
	commRBTree_print(&rbtree, 0x02);
	commPool_del(pool);
	return 0;
}
