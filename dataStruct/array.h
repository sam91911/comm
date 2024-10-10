#ifndef COMM_DATASTRUCTURE_ARRAY
#define COMM_DATASTRUCTURE_ARRAY
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char len;
	size_t size;
	char align;
	char *data;
} CommArray;

int commArray_init(CommArray *array, uint64_t len, size_t size, char align);
void commArray_del(CommArray *array);
int commArray_adjust(CommArray *array, uint64_t len);
int commArray_check(CommArray *array, uint64_t len);
int commArray_set(CommArray *array, void *item, uint64_t index);
void * commArray_get(CommArray *array, uint64_t index);

#endif
