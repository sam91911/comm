#include "GFPoly.h"

uint64_t commGFPoly_insert(uint64_t *poly, uint64_t x, uint64_t len){
	if(!poly) return 0;
	uint64_t rt = 0;
	uint64_t x_power = 1;
	for(uint64_t i = 0; i < len; i++){
		rt ^= commGF_mul(x_power, poly[i]);
		x_power = commGF_mul(x_power, x);
	}
	return rt;
}
int commGFPoly_RMatrix(uint64_t *x, uint64_t *RMatrix, uint64_t len){
	if(!x) return -1;
	if(!RMatrix) return -1;
	uint64_t *tmp;
	uint64_t eff;
	for(uint64_t i = 0; i < len; i++){
		tmp = RMatrix+len*i;
		memset(tmp, 0, len*8);
		tmp[0] = 1;
		eff = 1;
		for(uint64_t j = 0; j < len; j++){
			if(j == i){
				continue;
			}
			eff = commGF_mul(eff, commGF_inv(x[i]^x[j]));
			for(uint64_t k = len-1; k > 0; k--){
				tmp[k] = commGF_mul(tmp[k], x[j])^tmp[k-1];
			}
			tmp[0] = commGF_mul(tmp[0], x[j]);
		}
		for(uint64_t j = 0; j < len; j++){
			tmp[j] = commGF_mul(eff, tmp[j]);
		}
	}
	return 0;
}

int commGFPoly_reverse(uint64_t *y, uint64_t *RMatrix, uint64_t *poly, uint64_t len){
	if(!y) return -1;
	if(!RMatrix) return -1;
	if(!poly) return -1;
	memset(poly, 0, len*8);
	for(uint64_t i = 0; i < len; i++){
		for(uint64_t j = 0; j < len; j++){
			poly[j] ^= commGF_mul(y[i], RMatrix[i*len+j]);
		}
	}
	return 0;
}
