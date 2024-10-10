#ifndef COMM_DATASTRUCTURE_DSFOREST
#define COMM_DATASTRUCTURE_DSFOREST
#include <stdint.h>
#include "array.h"

typedef struct{
	int size;
	CommArray data;
} CommDSForest;

int commDSForest_init(CommDSForest *dsforest, int size);
void commDSForest_del(CommDSForest *dsforest);
int commDSForest_find(CommDSForest *dsforest, int index);
int commDSForest_union(CommDSForest *dsforest, int index1, int index2);

#endif
