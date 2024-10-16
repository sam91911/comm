#ifndef COMM_DATASTRUCTURE_QUEUE
#define COMM_DATASTRUCTURE_QUEUE
#include <stdlib.h>
#include <stdint.h>
#include "pool.h"
#include "list.h"

typedef CommList CommQueue;

int commQueue_init(CommQueue *queue, CommPool *pool);
int commQueue_del(CommQueue * queue);
int commQueue_enqueue(CommQueue *queue, void *data);
void * commQueue_dequeue(CommQueue *queue);

#endif
