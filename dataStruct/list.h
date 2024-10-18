#ifndef COMM_DATASTRUCTURE_LIST
#define COMM_DATASTRUCTURE_LIST
#include <stdlib.h>
#include <stdint.h>
#include "pool.h"

typedef int (*CommListCmp)(void *, void *, void *);

typedef struct {
	uint64_t *head;
	uint64_t *tail;
	uint64_t len;
	CommPool *pool;
} CommList;

typedef struct {
	uint64_t *post;
	uint64_t *cur;
} CommListIter;

int commList_init(CommList *list, CommPool *pool);
int commList_del(CommList *list);
int commList_insert_front(CommList *list, void *data);
int commList_insert_end(CommList *list, void *data);
void *commList_get(CommList *list, int index);
int commList_remove(CommList *list, int index);
int commList_search(CommList *list, CommListCmp cmp, void *data, void *arg);
void *commList_iter(CommList *list, CommListIter *iter);
int commList_iter_init(CommList *list, CommListIter *iter);

#endif
