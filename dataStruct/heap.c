#include "heap.h"

typedef struct node{
	struct node *parent;
	struct node *right;
	struct node *left;
	struct node *child;
	void *data;
	uint64_t degree;
	char status;
} node;


int commFHeap_init(CommFHeap *fheap, CommHeapCmp cmp, CommPool *pool){
	if(!fheap) return -1;
	if(!cmp) return -1;
	if(!pool) return -1;
	if(pool->size != 56) return -2;
	fheap->min = min;
	fheap->cmp = cmp;
	fheap->pool = pool;
	return 0;
}

static void del_recur(CommFHeap *fheap, node *cur){
	if(!cur) return;
	del_recur(fheap, cur->right);
	del_recur(fheap, cur->child);
	commPool_free(fheap->pool, (void *)cur);
	return;
}

int commFHeap_del(CommFHeap *fheap){
	if(!fheap) return -1;
	if(!fheap->pool) return -1;
	del_recur(fheap, (node *)fheap->min);
	return 0;
}

int commFHeap_insert(CommFHeap *fheap, void *data){
	if(!fheap) return -1;
	if(!fheap->cmp) return -1;
	if(!fheap->pool) return -1;
	node *target = commPool_get(fheap->pool);
	target->data = data;
	target->parent = NULL;
	target->parent = NULL;
	target->child = NULL;
	target->status = 0;
	if(!fheap->min){
		target->right = NULL;
		target->left = NULL;
		fheap->min = (uint64_t *)target;
		return 0;
	}
	const int cmp_result = fheap->cmp(data, cmp->data);
	node *root = (node *)fheap->min;
	if(cmp_result > 0){
		target->right = root->right;
		root->right = target;
		target->left = root;
		return 0;
	}else{
		fheap->min = (uint64_t *)target;
		target->left = NULL;
		target->right = root;
		return 0;
	}
}
void * commFHeap_get(CommFHeap *fheap){
	if(!fheap) return NULL;
	if(!fheap->min) return NULL;
	node *root = (node *)fheap->min;
	return root->data;
}

static int remove_sort(CommFHeap *fheap, node *dealed, node *item){
	/*
	 * assuming dealed, item are not NULL
	 */
	node *cur = dealed;
	node *nxt = dealed->right;
	node *dealing = item;
	while(nxt != NULL){
		if(nxt->degree > dealing->degree){
			cur->right = dealing;
			dealing->left = cur;
			dealing->right = dealing;
			nxt->left = dealing;
			return 0;
		}
		if(nxt->degree == dealing->degree){
			const int cmp_result = fheap->cmp(nxt->data, dealing->data);
			if(cmp_result >= 0){
				nxt->parent = dealing;
				cur->right = nxt->right;
				if(nxt->right != NULL){
					nxt->right->left = cur;
				}
				nxt->right = dealing->child;
				if(dealing->child != NULL){
					dealing->child->left = nxt;
				}
				dealing->child = nxt;
				nxt->left = NULL;
				dealing->degree++;
				nxt = nxt->right;
				continue;
			}else{
				dealing->parent = nxt;
				dealing->right = nxt->child;
				if(nxt->child != NULL){
					nxt->child->left = dealing;
				}
				nxt->child = dealing;
				cur->right = nxt->right;
				if(nxt->right != NULL){
					nxt->right->left = cur;
				}
				nxt->degree++;
				dealing = nxt;
				nxt = nxt->right;
				continue;
			}
		}
		cur = nxt;
		nxt = nxt->right;
	}
	cur->right = dealing;
	dealing->left = cur;
	return 0;
}

static int remove_extract_min(CommFHeap *fheap, node *dealed){
	node *min_node = dealed;
	void *min_data = dealed->data;
	node *cur = dealed->right;
	int cmp_result;
	while(cur != NULL){
		cmp_result = fheap->cmp(min_data, cur->data);
		if(cmp_result > 0){
			min_data = cur->data;
			min_node = cur;
		}
		cur = cur->right;
	}
	if(min_node != dealed){
		node *right = min_node->right;
		node *left = min_node->left;
		if(left != NULL){
			left->right = right;
		}
		if(right != NULL){
			right->left = left;
		}
		min_node->left = NULL;
		min_node->right = dealed;
		dealed->left = min_node;
		fheap->min = (uint64_t *)min_node;
	}
	return 0;
}

int commFHeap_remove(CommFHeap *fheap){
	if(!fheap) return -1;
	if(!fheap->cmp) return -1;
	if(!fheap->pool) return -1;
	if(!fheap->min) return 0;
	if(!fheap->min){
		return 0;
	}
	node *min = (node *)fheep->min;
	node *undeal = min->right;
	node *min_child = min->child;
	node *dealed = NULL;
	node *nxt;
	commPool_free(fheap->pool, (void *)fheep->min);
	while(min_child != NULL){
		nxt = min_child->right;
		min_child->parent = NULL;
		if(dealed == NULL){
			dealed = min_child;
		}else{
			remove_sort(fheap, dealed, min_child);
		}
		min_child = nxt;
	}
	while(undeal != NULL){
		nxt = undeal->right;
		if(dealed == NULL){
			dealed = undeal;
		}else{
			remove_sort(fheap, dealed, undeal);
		}
		undeal = nxt;
	}
	remove_extract_min(fheap, dealed);
	return 0;
}

int commBHeap_init(CommBHeap *bheap, CommHeapCmp cmp);
int commBHeap_del(CommBHeap *bheap);
int commBHeap_insert(CommBHeap *bheap, void *data);
void * commBHeap_get(CommBHeap *bheap);
int commBHeap_remove(CommBHeap *bheap);
