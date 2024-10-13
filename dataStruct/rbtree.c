#include "rbtree.h"

typedef enum{
	BLACK,
	RED
} Color;

typedef struct node{
	struct node *parent;
	struct node *left;
	struct node *right;
	void *data;
	Color color;
} node;

int commRBTree_init(CommRBTree *rbtree, CommRBTreeCmp cmp, CommRBTreePrint print, CommRBTreeDel del, CommPool *pool){
	if(!rbtree) return -1;
	if(!cmp) return -1;
	if(!pool) return -1;
	if(pool->size != 40) return -2;
	rbtree->len = 0;
	rbtree->cmp = cmp;
	rbtree->print = print;
	rbtree->del = del;
	rbtree->root = NULL;
	rbtree->pool = pool;
	return 0;
}

static void tree_del(CommRBTree *rbtree, node *node){
	if(!node) return;
	tree_del(rbtree, node->right);
	tree_del(rbtree, node->left);
	if(rbtree->del){
		rbtree->del(node->data);
	}
	commPool_free(rbtree->pool, (void *)node);
	return;
}

void commRBTree_del(CommRBTree *rbtree){
	if(!rbtree) return;
	tree_del(rbtree, (node *)(rbtree->root));
	return;
}

static char node_status(node *item){
	char status = 0;
	const node *left = item->left;
	const node *right = item->right;
	if(left != NULL){
		status |= 0x08;
		if(left->color == RED){
			status |= 0x04;
		}
	}
	if(right != NULL){
		status |= 0x02;
		if(right->color == RED){
			status |= 0x01;
		}
	}
	return status;
}

static int rotate_left(CommRBTree *rbtree, node *item){
	node *parent = item->parent;
	node *child = item->right;
	node *swap_leaf = child->left;
	item->parent = child;
	child->parent = parent;
	item->right = swap_leaf;
	child->left = item;
	if(swap_leaf != NULL){
		swap_leaf->parent = item;
	}
	if(parent != NULL){
		if(parent->right == item){
			parent->right = child;
		}else{
			parent->left = child;
		}
	}else{
		rbtree->root = (uint64_t *)child;
	}
	return 0;
}

static int rotate_right(CommRBTree *rbtree, node *item){
	node *parent = item->parent;
	node *child = item->left;
	node *swap_leaf = child->right;
	item->parent = child;
	child->parent = parent;
	item->left = swap_leaf;
	child->right = item;
	if(swap_leaf != NULL){
		swap_leaf->parent = item;
	}
	if(parent != NULL){
		if(parent->right == item){
			parent->right = child;
		}else{
			parent->left = child;
		}
	}else{
		rbtree->root = (uint64_t *)child;
	}
	return 0;
}

static node * lift(CommRBTree *rbtree, node *item){
	/*
	 * assuming parent super node is only contain one red node, and index has tow red child.
	 */
	node *parent = item->parent;
	node *left = item->left;
	node *right = item->right;
	if(parent == NULL){
		left->color = BLACK;
		right->color = BLACK;
		return item;
	}
	if(parent->color == RED){
		node *grand = parent->parent;
		if(parent->left == item){
			if(grand->left == parent){
				rotate_right(rbtree, grand);
				grand->color = RED;
				parent->color = BLACK;
				item->color = RED;
				left->color = BLACK;
				right->color = BLACK;
				return parent;
			}else{
				rotate_right(rbtree, parent);
				rotate_left(rbtree, grand);
				grand->color = RED;
				parent->color = RED;
				item->color = BLACK;
				left->color = BLACK;
				right->color = BLACK;
				return item;
			}
		}else{
			if(grand->left == parent){
				rotate_left(rbtree, parent);
				rotate_right(rbtree, grand);
				grand->color = RED;
				parent->color = RED;
				item->color = BLACK;
				left->color = BLACK;
				right->color = BLACK;
				return item;
			}else{
				rotate_right(rbtree, grand);
				grand->color = RED;
				parent->color = BLACK;
				item->color = RED;
				left->color = BLACK;
				right->color = BLACK;
				return parent;
			}
		}
	}else{
		item->color = RED;
		left->color = BLACK;
		right->color = BLACK;
		return parent;
	}
	return 0;
}

static node * borrow_from_left(CommRBTree *rbtree, node *item, char status){
	/*
	 * assuming parent is red or root, and the item node is the right child of the parent.
	 */
	node *parent = item->parent;
	node *uncle = parent->left;
	if(status&0x01){
		node *child = uncle->right;
		rotate_left(rbtree, uncle);
		rotate_right(rbtree, parent);
		child->color = parent->color;
		parent->color = BLACK;
		item->color = RED;
		return parent;
	}else{
		node *child = uncle->left;
		rotate_right(rbtree, parent);
		uncle->color = parent->color;
		if(status&0x04){
			child->color = BLACK;
		}
		parent->color = BLACK;
		item->color = RED;
		return parent;
	}
}

static node * borrow_from_right(CommRBTree *rbtree, node *item, char status){
	/*
	 * assuming parent is red or root, and the item node is the left child of the parent.
	 */
	node *parent = item->parent;
	node *uncle = parent->right;
	if(status&0x04){
		node *child = uncle->left;
		rotate_right(rbtree, uncle);
		rotate_left(rbtree, parent);
		child->color = parent->color;
		parent->color = BLACK;
		item->color = RED;
		return parent;
	}else{
		node *child = uncle->right;
		rotate_left(rbtree, parent);
		uncle->color = parent->color;
		if(status&0x01){
			child->color = BLACK;
		}
		parent->color = BLACK;
		item->color = RED;
		return parent;
	}
}

static node * borrow_from_parent(CommRBTree *rbtree, node *item){
	/*
	 * assuming parent is red or root, and the uncle node has no red child.
	 */
	node *parent = item->parent;
	parent->color = BLACK;
	item->color = RED;
	if(parent->left == item){
		node *uncle = parent->right;
		uncle->color = RED;
		return parent;
	}else{
		node *uncle = parent->left;
		uncle->color = RED;
		return parent;
	}
}

static node * span(CommRBTree *rbtree, node *item){
	/*
	 * assuming the item is not root and has no red child, parent super node is root or contain red child.
	 */
	node *parent = item->parent;
	node *grand = item->parent;
	if(parent->color == BLACK){
		if(parent->right == item){
			node *ant = parent->left;
			rotate_right(rbtree, parent);
			parent->color = RED;
			ant->color = BLACK;
		}else{
			node *ant = parent->right;
			rotate_left(rbtree, parent);
			parent->color = RED;
			ant->color = BLACK;
		}
	}
	//parent is red or root
	if(parent->left == item){
		node *uncle = parent->right;
		const char status = node_status(uncle);
		if(status&0x05){
			return borrow_from_right(rbtree, item, status);
		}else{
			return borrow_from_parent(rbtree, item);
		}
	}else{
		node *uncle = parent->left;
		const char status = node_status(uncle);
		if(status&0x05){
			return borrow_from_left(rbtree, item, status);
		}else{
			return borrow_from_parent(rbtree, item);
		}
	}
}

static node * succ_node(CommRBTree *rbtree, node *item){
	node *child = item->right;
	if(child != 0){
		//has right tree;
		while(child->right != 0){
			//right going until end
			child = child->right;
		}
		return child;
	}
	node *current = item;
	node *parent = current->parent;
	if(parent != 0){
		while(parent->right == current){
			current = parent;
			parent = current->parent;
			if(parent == 0) return 0;
		}
		child = parent->right;
		if(child != 0){
			while(child->right != 0){
				child = child->right;
			}
			return child;
		}else{
			return parent;
		}
	}
}

static int swap_succ(CommRBTree *rbtree, node *item){
	node *succ = succ_node(rbtree, item);
	void *swap = item->data;
	item->data = succ->data;
	succ->data = swap;
	return 0;
}

static void replace_data(CommRBTree *rbtree, void *data, node *item){
	if(rbtree->del){
		rbtree->del(item->data);
	}
	item->data = data;
	return;
}

static node * insert_assure(CommRBTree *rbtree, void *data, node *cur){
	const char status = node_status(cur);
	if((status&0x04)&&(status&0x01)){
		int cmp_result = rbtree->cmp(data, cur->data);
		if(cmp_result == 0){
			replace_data(rbtree, data, cur);
			return 0;
		}
		if(cmp_result > 0){
			node *child = cur->right;
			lift(rbtree, cur);
			return child;
		}else{
			node *child = cur->left;
			lift(rbtree, cur);
			return child;
		}
	}
	return cur;
}

static node * insert_loop(CommRBTree *rbtree, void *data, node *cur){
	const int cmp_result = rbtree->cmp(data, cur->data);
	if(cmp_result == 0){
		replace_data(rbtree, data, cur);
		return 0;
	}
	if(cmp_result > 0){
		node *child = cur->right;
		if(child == NULL){
			node *new_node = commPool_get(rbtree->pool);
			new_node->parent = cur;
			new_node->left = NULL;
			new_node->right = NULL;
			new_node->data = data;
			new_node->color = RED;
			cur->right = new_node;
			return 0;
		}
		if(child->color == BLACK){
			return child;
		}
		const int child_cmp = rbtree->cmp(data, child->data);
		if(child_cmp == 0){
			replace_data(rbtree, data, child);
			return 0;
		}
		if(child_cmp > 0){
			node *grand_son = child->right;
			if(grand_son == NULL){
				node *new_node = commPool_get(rbtree->pool);
				new_node->parent = child;
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->data = data;
				new_node->color = RED;
				child->right = new_node;
				rotate_left(rbtree, cur);
				child->color = BLACK;
				cur->color = RED;
				return 0;
			}
			return grand_son;
		}else{
			node *grand_son = child->left;
			if(grand_son == NULL){
				node *new_node = commPool_get(rbtree->pool);
				new_node->parent = child;
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->data = data;
				new_node->color = BLACK;
				child->left = new_node;
				rotate_right(rbtree, child);
				rotate_left(rbtree, cur);
				child->color = RED;
				cur->color = RED;
				return 0;
			}
			return grand_son;
		}
	}else{
		node *child = cur->left;
		if(child == NULL){
			node *new_node = commPool_get(rbtree->pool);
			new_node->parent = cur;
			new_node->left = NULL;
			new_node->right = NULL;
			new_node->data = data;
			new_node->color = RED;
			cur->left = new_node;
			return 0;
		}
		if(child->color == BLACK){
			return child;
		}
		const int child_cmp = rbtree->cmp(data, child->data);
		if(child_cmp == 0){
			replace_data(rbtree, data, child);
			return 0;
		}
		if(child_cmp > 0){
			node *grand_son = child->right;
			if(grand_son == NULL){
				node *new_node = commPool_get(rbtree->pool);
				new_node->parent = child;
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->data = data;
				new_node->color = BLACK;
				child->right = new_node;
				rotate_left(rbtree, child);
				rotate_right(rbtree, cur);
				child->color = RED;
				cur->color = RED;
				return 0;
			}
			return grand_son;
		}else{
			node *grand_son = child->left;
			if(grand_son == NULL){
				node *new_node = commPool_get(rbtree->pool);
				new_node->parent = child;
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->data = data;
				new_node->color = RED;
				child->left = new_node;
				rotate_right(rbtree, cur);
				child->color = BLACK;
				cur->color = RED;
				return 0;
			}
			return grand_son;
		}
	}
}

int commRBTree_insert(CommRBTree *rbtree, void *data){
	if(!rbtree) return -1;
	if(!rbtree->pool) return -1;
	if(!rbtree->cmp) return -1;
	node *cur = (node *)rbtree->root;
	//root is empty
	if(cur == NULL){
		node *new_node = commPool_get(rbtree->pool);
		new_node->parent = NULL;
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->data = data;
		new_node->color = BLACK;
		return 0;
	}
	while(1){
		cur = insert_assure(rbtree, data, cur);
		if(cur == NULL){
			return 0;
		}
		cur = insert_loop(rbtree, data, cur);
		if(cur == NULL){
			return 0;
		}
	}
}

void * commRBTree_get(CommRBTree *rbtree, void *data){
	if(!rbtree) return NULL;
	if(!rbtree->cmp) return NULL;
	node *cur = (node *)rbtree->root;
	int cmp_result;
	while(cur != NULL){
		cmp_result = rbtree->cmp(data, cur->data);
		if(cmp_result == 0){
			return cur->data;
		}
		if(cmp_result > 0){
			cur = cur->right;
		}else{
			cur = cur->left;
		}
	}
	return NULL;
}

static node * remove_assure(CommRBTree *rbtree, void *data, node *cur){
	const char status = node_status(cur);
	if((!(status&0x05))&&(cur->parent != NULL)){
		return span(rbtree, cur);
	}
	return cur;
}

static node * remove_loop(CommRBTree *rbtree, void *data, node *cur){
	int cmp_result = rbtree->cmp(data, cur->data);
	if(cmp_result == 0){
		node *right = cur->right;
		node *left = cur->left;
		if(right == NULL){
			node *parent = cur->parent;
			if(parent == NULL){
				rbtree->root = (uint64_t *)left;
			}else if(parent->right == cur){
				parent->right = left;
			}else{
				parent->left = left;
			}
			if(left != NULL){
				left->parent = parent;
				left->color = BLACK;
			}
			if(rbtree->del){
				rbtree->del(cur->data);
			}
			commPool_free(rbtree->pool, (void *)cur);
			return 0;
		}
		if(left == NULL){
			node *parent = cur->parent;
			if(parent == NULL){
				rbtree->root = (uint64_t *)right;
			}else if(parent->right == cur){
				parent->right = right;
			}else{
				parent->left = right;
			}
			right->parent = parent;
			right->color = BLACK;
			if(rbtree->del){
				rbtree->del(cur->data);
			}
			commPool_free(rbtree->pool, (void *)cur);
			return 0;
		}
		swap_succ(rbtree, cur);
		cmp_result = 1;
	}
	if(cmp_result > 0){
		node *child = cur->right;
		if(child == NULL){
			return 0;
		}
		if(child->color == BLACK){
			return child;
		}
		const int child_cmp = rbtree->cmp(data, child->data);
		if(child_cmp == 0){
			if(child->right == NULL){
				cur->right = NULL;
				if(rbtree->del){
					rbtree->del(cur->data);
				}
				commPool_free(rbtree->pool, (void *)child);
				return 0;
			}
			swap_succ(rbtree, child);
			return child->right;
		}
		if(child_cmp > 0){
			return child->right;
		}else{
			return child->left;
		}
	}else{
		node *child = cur->left;
		if(child == NULL){
			return 0;
		}
		if(child->color == BLACK){
			return child;
		}
		const int child_cmp = rbtree->cmp(data, child->data);
		if(child_cmp == 0){
			if(child->right == NULL){
				cur->left = NULL;
				if(rbtree->del){
					rbtree->del(cur->data);
				}
				commPool_free(rbtree->pool, (void *)child);
				return 0;
			}
			swap_succ(rbtree, child);
			return child->right;
		}
		if(child_cmp > 0){
			return child->right;
		}else{
			return child->left;
		}
	}
}

static node *remove_root(CommRBTree *rbtree, void *data, node *cur){
	int cmp_result = rbtree->cmp(data, cur->data);
	if(cmp_result == 0){
		node *right = cur->right;
		node *left = cur->left;
		if(right == NULL){
			rbtree->root = (uint64_t *)left;
			if(left != NULL){
				left->parent = NULL;
			}
			if(rbtree->del){
				rbtree->del(cur->data);
			}
			commPool_free(rbtree->pool, (void *)cur);
			return NULL;
		}
		if(left == NULL){
			rbtree->root = (uint64_t *)right;
			right->parent = NULL;
			if(rbtree->del){
				rbtree->del(cur->data);
			}
			commPool_free(rbtree->pool, (void *)cur);
			return NULL;
		}
		swap_succ(rbtree, cur);
		cmp_result = 1;
	}
	if(cmp_result > 0){
		node *child = cur->right;
		if(child == NULL){
			return NULL;
		}
		if(child->color == BLACK){
			return child;
		}
		const int child_cmp = rbtree->cmp(data, child->data);
		if(child_cmp == 0){
			if(child->right == NULL){
				cur->right = NULL;
				if(rbtree->del){
					rbtree->del(child->data);
				}
				commPool_free(rbtree->pool, (void *)child);
				return NULL;
			}
			swap_succ(rbtree, child);
			return child->right;
		}
		if(child_cmp > 0){
			return child->right;
		}else{
			return child->left;
		}
	}else{
		node *child = cur->left;
		if(child == NULL){
			return NULL;
		}
		if(child->color == BLACK){
			return child;
		}
		const int child_cmp = rbtree->cmp(data, child->data);
		if(child_cmp == 0){
			if(child->right == NULL){
				cur->left = NULL;
				if(rbtree->del){
					rbtree->del(child->data);
				}
				commPool_free(rbtree->pool, (void *)child);
				return NULL;
			}
			swap_succ(rbtree, child);
			return child->right;
		}
		if(child_cmp > 0){
			return child->right;
		}else{
			return child->left;
		}
	}
}

int commRBTree_remove(CommRBTree *rbtree, void *data){
	if(!rbtree) return -1;
	if(!rbtree->cmp) return -1;
	if(!rbtree->pool) return -1;
	if(rbtree->root == NULL) return 0;
	node *cur = (node *)(rbtree->root);
	cur = remove_root(rbtree, data, cur);
	while(cur != NULL){
		cur = remove_assure(rbtree, data, cur);
		if(cur == NULL) return 0;
		cur = remove_loop(rbtree, data, cur);
	}
	return 0;
}
uint64_t *commRBTree_succ(CommRBTree *rbtree, uint64_t *item){
	if(!rbtree) return NULL;
	if(item == NULL){
		node *cur = (node *)rbtree->root;
		if(cur == NULL) return NULL;
		while(cur->left != NULL){
			cur = cur->left;
		}
		return (uint64_t *)cur;
	}
	return (uint64_t *)succ_node(rbtree, (node *)item);
}

static void print_recur(CommRBTree *rbtree, char type, node *cur){
	if(cur == NULL) return;
	if(type&0x01){
		rbtree->print(cur->data);
	}
	print_recur(rbtree, type, cur->left);
	if(type&0x02){
		rbtree->print(cur->data);
	}
	print_recur(rbtree, type, cur->right);
	if(type&0x04){
		rbtree->print(cur->data);
	}
	return;
}

int commRBTree_print(CommRBTree *rbtree, char type){
	if(!rbtree) return -1;
	if(!rbtree->print) return 0;
	print_recur(rbtree, type, (node *)(rbtree->root));
	return 0;
}
