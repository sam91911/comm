#include "list.h"

int commList_init(CommList *list, CommPool *pool){
	if(!list) return -1;
	if(!pool) return -1;
	list->head = NULL;
	list->tail = NULL;
	list->len = 0;
	list->pool = pool;
	return 0;
}

static void del_loop(CommList *list){
	if(!list->head) return;
	CommPool *pool = list->pool;
	uint64_t *nxt = (uint64_t *)(list->head[0]);
	commPool_free(pool, (void *)(list->head));
	list->head = nxt;
	return;
}

int commList_del(CommList *list){
	if(!list) return -1;
	while(list->head){
		del_loop(list);
	}
	return 0;
}
int commList_insert_front(CommList *list, void *data){
	if(!list) return -1;
	//pass;
}
int commList_insert_end(CommList *list, void *data);
void *commList_get(CommList *list, int index);
int commList_remove(CommList *list, int index);
uint64_t commList_search(CommList *list, CommListCmp cmp, void *data);
