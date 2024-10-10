#include "dsforest.h"

int commDSForest_init(CommDSForest *dsforest, int size){
	if(!dsforest) return -1;
	dsforest->size = size;
	commArray_init(&(dsforest->data), size, 16, 0);
	int64_t *item = (int64_t *)dsforest->data.data;
	for(int i = 0; i < size; i++){
		item[2*i] = i;
		item[2*i+1] = 0;
	}
	return 0;
}
void commDSForest_del(CommDSForest *dsforest){
	if(!dsforest) return;
	commArray_del(&(dsforest->data));
	return;
}
int commDSForest_find(CommDSForest *dsforest, int index){
	if(!dsforest) return -1;
	if(dsforest->size <= index) return -2;
	if(index < 0) return -2;
	int64_t *item = (int64_t *)dsforest->data.data;
	while(item[2*index] != index)
		index = item[2*index] = item[2*item[2*index]];
	return index;
}
int commDSForest_union(CommDSForest *dsforest, int index1, int index2){
	if(!dsforest) return -1;
	if(dsforest->size <= index1) return -2;
	if(dsforest->size <= index2) return -2;
	if(index1 < 0) return -2;
	if(index2 < 0) return -2;
	int64_t *item = (int64_t *)dsforest->data.data;
	if(item[2*index1+1] == item[2*index2+1]){
		item[2*index2] = index1;
		item[2*index1+1]++;
	}else if(item[2*index1+1] > item[2*index2+1]){
		item[2*index2] = index1;
	}else{
		item[2*index1] = index2;
	}
	return 0;
}
