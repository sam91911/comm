#ifndef COMM_DATASTRUCTURE_RBTREE
#define COMM_DATASTRUCTURE_RBTREE
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pool.h"

typedef int (*CommRBTreeCmp)(void *, void *, void *);
typedef void (*CommRBTreePrint)(void *, void *);
typedef void (*CommRBTreeDel)(void *, void *);

typedef struct{
	uint64_t len;
	CommRBTreeCmp cmp;
	CommRBTreePrint print;
	CommRBTreeDel del;
	void *cmp_arg;
	void *print_arg;
	void *del_arg;
	uint64_t *root;
	CommPool *pool;
} CommRBTree;

int commRBTree_init(CommRBTree *rbtree, CommRBTreeCmp cmp, CommRBTreePrint print, CommRBTreeDel del, CommPool *pool);
void commRBTree_del(CommRBTree *rbtree);
int commRBTree_insert(CommRBTree *rbtree, void *data);
void * commRBTree_get(CommRBTree *rbtree, void *data);
int commRBTree_remove(CommRBTree *rbtree, void *data);
uint64_t * commRBTree_succ(CommRBTree *rbtree, uint64_t *item);
int commRBTree_print(CommRBTree *rbtree, char type);
#endif
