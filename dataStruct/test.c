#include "pool.h"
#include "rbtree.h"
#include "list.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char ** argv){
	const int len = 1ULL<<10;
	int test[len];
	CommPool *pool = commPool_new(16, 1024);
	CommList list;
	CommListIter iter;
	commList_init(&list, pool);
	for(int i = 0; i < len; i++){
		test[i] = i;
	}
	for(int i = 0; i < len; i++){
		commList_insert_front(&list, test+i);
	}
	commList_iter_init(&list, &iter);
	int *data;
	while(1){
		data = commList_iter(&list, &iter);
		if(data == 0) break;
		printf("%d\n", *data);
	}
	commList_del(&list);
	commPool_del(pool);
	return 0;
}
