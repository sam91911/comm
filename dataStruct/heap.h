#ifndef COMM_DATASTRUCTURE_HEAP
#define COMM_DATASTRUCTURE_HEAP
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pool.h"
#include "array.h"
typedef int (*CommHeapCmp)(void *, void *);
typedef struct {
	uint64_t *min;
	CommHeapCmp cmp;
	CommPool *pool;
} CommFHeap;

typedef struct {
	CommHeapCmp cmp;
	CommArray *array;
	uint64_t len;
} CommBHeap;
int commFHeap_init(CommFHeap *fheap, CommHeapCmp cmp, CommPool *pool);
int commFHeap_del(CommFHeap *fheap);
int commFHeap_insert(CommFHeap *fheap, void *data);
void * commFHeap_get(CommFHeap *fheap);
int commFHeap_remove(CommFHeap *fheap);
int commBHeap_init(CommBHeap *bheap, CommHeapCmp cmp, CommArray *array, uint64_t len);
int commBHeap_del(CommBHeap *bheap);
int commBHeap_insert(CommBHeap *bheap, void *data);
void * commBHeap_get(CommBHeap *bheap);
int commBHeap_remove(CommBHeap *bheap);

#endif
