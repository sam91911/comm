#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "GF.h"
#include "GFPoly.h"

static uint64_t rand64(){
	uint64_t rt = rand();
	rt = rt<<32 | rand();
	return rt;
}

int main(int argc, char **argv){
	int len = 16;
	int loop = 1<<12;
	uint64_t src[len];
	uint64_t x[len];
	uint64_t y[len];
	uint64_t RMatrix[len*len];
	uint64_t reverse[len];
	int result;
	for(int i = 0; i < loop; i++){
		for(int j = 0; j < len; j++){
			src[j] = rand64();
			x[j] = rand64();
		}
		for(int j = 0; j < len; j++){
			y[j] = commGFPoly_insert(src, x[j], len);
		}
		commGFPoly_RMatrix(x, RMatrix, len);
		commGFPoly_reverse(y, RMatrix, reverse, len);
		int result = 0;
		for(int j = 0; j < len; j++){
			if(src[j] != reverse[j])
				result = 1;
		}
		if(result != 0){
			printf("\n");
			for(int j = 0; j < len; j++){
				printf("%016lx\t", src[j]);
			}
			printf("\n");
			for(int j = 0; j < len; j++){
				printf("%016lx\t", x[j]);
			}
			printf("\n");
			for(int j = 0; j < len; j++){
				printf("%016lx\t", reverse[j]);
			}
			printf("\n");
		}
		printf("loop:%d\r", i);
	}
	return 0;
}
