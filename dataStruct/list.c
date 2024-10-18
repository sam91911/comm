#include "list.h"

int commList_init(CommList *list, CommPool *pool){
	if(!list) return -1;
	if(!pool) return -1;
	if(pool->size < 16) return -1;
	list->head = NULL;
	list->tail = NULL;
	list->len = 0;
	list->pool = pool;
	return 0;
}

static void del_loop(CommList *list){
	CommPool *pool = list->pool;
	uint64_t *nxt = (uint64_t *)list->head[0];
	uint64_t *cur = list->head;
	commPool_free(pool, (void *)(list->head));
	list->head = nxt;
	if(nxt){
		nxt[0] ^= (uint64_t)cur;
	}
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
	if(!list->pool) return -1;
	uint64_t *target = commPool_get(list->pool);
	target[1] = (uint64_t)data;
	if(!(list->head)){
		list->head = target;
		list->tail = target;
		target[0] = 0;
		return 0;
	}
	uint64_t *head = list->head;
	head[0] ^= (uint64_t)target;
	target[0] = (uint64_t)head;
	list->head = target;
	return 0;
}
int commList_insert_end(CommList *list, void *data){
	if(!list) return -1;
	if(!list->pool) return -1;
	uint64_t *target = commPool_get(list->pool);
	target[1] = (uint64_t)data;
	if(!(list->tail)){
		list->head = target;
		list->tail = target;
		target[0] = 0;
		return 0;
	}
	uint64_t *tail = list->tail;
	tail[0] ^= (uint64_t)target;
	target[0] = (uint64_t)tail;
	list->tail = target;
	return 0;
}
void *commList_get(CommList *list, int index){
	if(!list) return 0;
	uint64_t *post = 0;
	uint64_t *cur;
	uint64_t *nxt;
	if(index >= 0){
		cur = list->head;
	}else{
		cur = list->tail;
		index = -(index+1);
	}
	while(cur != 0){
		if(index == 0){
			return (void *)(cur[1]);
		}
		index--;
		nxt = (uint64_t *)(cur[0]^(uint64_t)post);
		post = cur;
		cur = nxt;
	}
	return 0;
}

static void remove_cell(CommList *list, uint64_t *post, uint64_t *cur, uint64_t *nxt){
	if(post){
		post[0] ^= ((uint64_t)cur)^((uint64_t)nxt);
	}
	if(nxt){
		nxt[0] ^= ((uint64_t)cur)^((uint64_t)post);
	}
	commPool_free(list->pool, cur);
	return;
}

int commList_remove(CommList *list, int index){
	if(!list) return -1;
	if(!list->pool) return -1;
	uint64_t *post = 0;
	uint64_t *cur;
	uint64_t *nxt;
	if(index >= 0){
		cur = list->head;
	}else{
		cur = list->tail;
		index = -(index+1);
	}
	while(cur != 0){
		nxt = (uint64_t *)(cur[0]^(uint64_t)post);
		if(index == 0){
			remove_cell(list, post, cur, nxt);
			return 0;
		}
		index--;
		post = cur;
		cur = nxt;
	}
	return -1;
}
int commList_search(CommList *list, CommListCmp cmp, void *data, void *arg){
	if(!list) return -1;
	if(!cmp) return -1;
	uint64_t *post = 0;
	uint64_t *cur = list->head;
	uint64_t *nxt;
	int cmp_result;
	int index = 0;
	while(cur != 0){
		nxt = (uint64_t *)(cur[0]^(uint64_t)post);
		cmp_result = cmp(data, (void *)(cur[1]), arg);
		if(cmp_result == 0){
			return index;
		}
		post = cur;
		cur = nxt;
		index++;
	}
	return -2;
}
void *commList_iter(CommList *list, CommListIter *iter){
	if(!list) return 0;
	if(!iter) return 0;
	if(iter->cur == 0) return 0;
	uint64_t *nxt = (uint64_t *)((uint64_t)iter->post^iter->cur[0]);
	void * data = (void *)iter->cur[1];
	iter->post = iter->cur;
	iter->cur = nxt;
	return data;
}
int commList_iter_init(CommList *list, CommListIter *iter){
	if(!list) return -1;
	if(!iter) return -1;
	iter->post = 0;
	iter->cur = list->head;
	return 0;
}
