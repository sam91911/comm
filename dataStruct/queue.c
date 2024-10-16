#include "queue.h"

int commQueue_init(CommQueue *queue, CommPool *pool){
	if(!queue) return -1;
	if(!pool) return -1;
	commList_init((CommList *)queue, pool);
	return 0;
}
int commQueue_del(CommQueue * queue){
	if(!queue) return -1;
	commList_del((CommList *)queue);
	return 0;
}
int commQueue_enqueue(CommQueue *queue, void *data){
	if(!queue) return -1;
	commList_insert_end((CommList *)queue, data);
	return 0;
}
void * commQueue_dequeue(CommQueue *queue){
	if(!queue) return 0;
	void *target = commList_get((CommList *)queue, 0);
	commList_remove((CommList *)queue, 0);
	return target;
}
