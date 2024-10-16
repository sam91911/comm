#include "GF.h"

static uint64_t base = 0x0000000247f43cb7;
static uint64_t mask = 0x8000000000000000;

uint64_t commGF_mul(uint64_t a, uint64_t b){
	uint64_t rt = 0;
	for(int i = 0; i < 64; i++){
		if(rt&mask){
			rt = (rt<<1)^(base);
		}else{
			rt = rt<<1;
		}
		if(b&mask){
			rt = rt^a;
		}
		b = b<<1;
	}
	return rt;
}

uint64_t commGF_inv(uint64_t a){
	uint64_t rt = a;
	for(int i = 0; i < 62; i++){
		rt = commGF_mul(a, commGF_mul(rt, rt));
	}
	rt = commGF_mul(rt, rt);
	rt = commGF_mul(a, commGF_mul(rt, rt));
	return rt;
}

