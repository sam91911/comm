#include "array.h"

//NOT COMPLETED YET

int commArray_init(CommArray *array, uint64_t len, size_t size, char align){
	/*
	 * The init function will reset the data pointer in array.
	 * Therefore, please make sure the data pointer not holding a allocated pointer.
	 * It will cause the leakage of memory.
	 * */
	if(!array) return -1;
	array->align = align;
	int size_clz = 63-__builtin_clzll(size-1);
	if(align){
		array->size = (1<<size_clz);
	}else{
		array->size = size;
	}
	commArray_adjust(array, len);
	return 0;
}
void commArray_del(CommArray *array){
	if(!array) return;
	if(array->data){
		free(array->data);
	}
	array->data = NULL;
	return;
}
int commArray_adjust(CommArray *array, uint64_t len){
	if(!array) return -1;
	if(!array->size) return -2;
	int len_bit = 64-__builtin_clzll(array->len);
	if(!array->data){
		array->data = malloc((1<<len_bit)*array->size);
		array->len = len_bit;
		return 0;
	}
	if(array->len == len_bit)
		return 0;
	array->data = realloc(array->data, (1<<len_bit)*array->size);
	array->len = len_bit;
	return 0;
}
int commArray_check(CommArray *array, uint64_t len){
	if(!array) return -1;
	if(!array->size) return -2;
	int len_bit = 64-__builtin_clzll(array->len);
	if(!array->data){
		return -3;
	}
	if(array->len == len_bit)
		return 0;
}
int commArray_set(CommArray *array, void *item, uint64_t index){
	if(!array) return -1;
	if(!array->data) return -2;
	if(index >= (1<<array->len))
		commArray_adjust(array, index+1);
	memcpy(array->data+(index*array->size), item, array->size);
	return 0;
}
void * commArray_get(CommArray *array, uint64_t index){
	if(!array) return NULL;
	if(!array->data) return NULL;
	if(index >= (1<<array->len)) return NULL;
	return array->data+(index*array->size);
}

