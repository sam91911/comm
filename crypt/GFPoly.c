#include "GFPoly.h"
#define BUFFER_SIZE 4096

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

int commGFPoly_FIFO_insert(int filein, int fileout, uint64_t x, uint64_t len){
	if(filein <= 0) return -1;
	if(fileout <= 0) return -1;
	if(len == 0) return -1;
	ssize_t read_size = 0;
	uint64_t result;
	char buffer[len*8];
	char read_buffer[BUFFER_SIZE];
	uint64_t read_buffer_pt = 0;
	uint64_t remain = 0;
	char write_buffer[BUFFER_SIZE];
	uint64_t write_buffer_pt = 0;
	for(;;){
		read_size = read(filein, read_buffer, BUFFER_SIZE);
		if(read_size < 0) break;
		read_buffer_pt = 0;
		if(remain > 0){
			if(read_size >= (len*8-remain)){
				memcpy(buffer+remain, read_buffer, len*8-remain);
				read_buffer_pt = len*8-remain;
			}else{
				memcpy(buffer+remain, read_buffer, read_size);
				remain += read_size;
				memset(buffer+remain, 0, len*8-remain);
				read_buffer_pt = read_size;
			}
			result = commGFPoly_insert((uint64_t *)buffer, x, len);
			if((BUFFER_SIZE - write_buffer_pt) < 8){
				if(write(fileout, write_buffer, write_buffer_pt) == -1) return -1;
				write_buffer_pt = 0;
			}
			*(uint64_t *)(write_buffer+write_buffer_pt) = result;
			write_buffer_pt += 8;
		}
		remain = 0;
		while(read_buffer_pt < (read_size-len*8)){
			result = commGFPoly_insert((uint64_t *)(read_buffer+read_buffer_pt), x, len);
			if((BUFFER_SIZE - write_buffer_pt) < 8){
				if(write(fileout, write_buffer, write_buffer_pt) == -1) return -1;
				write_buffer_pt = 0;
			}
			*(uint64_t *)(write_buffer+write_buffer_pt) = result;
			write_buffer_pt += 8;
			read_buffer_pt += len*8;
		}
		remain = read_size - read_buffer_pt;
		memcpy(buffer, read_buffer+read_buffer_pt, remain);
		if(read_size != BUFFER_SIZE){
			if(remain > 0){
				memset(buffer+remain, 0, len*8-remain);
				result = commGFPoly_insert((uint64_t *)buffer, x, len);
				if((BUFFER_SIZE - write_buffer_pt) < 8){
					if(write(fileout, write_buffer, write_buffer_pt) == -1) return -1;
					write_buffer_pt = 0;
				}
				*(uint64_t *)(write_buffer+write_buffer_pt) = result;
				write_buffer_pt += 8;
			}
			if(write(fileout, write_buffer, write_buffer_pt) == -1) return -1;
			break;
		}
	}
	if(read_size < 0) return (int)read_size;
	return 0;
}

int commGFPoly_FIFO_reverse(int filein, int fileout, uint64_t *poly, uint64_t len){
	if(filein <= 0) return -1;
	if(fileout <= 0) return -1;
	if(!poly) return -1;
	if(len == 0) return -1;
	ssize_t read_size = 0;
	char read_buffer[BUFFER_SIZE];
	char write_buffer[BUFFER_SIZE];
	uint64_t read_buffer_pt = 0;
	uint64_t write_buffer_pt = 0;
	uint64_t buffer[len];
	for(;;){
		read_size = read(filein, read_buffer, BUFFER_SIZE);
		if(read_size < 0) return -1;
		read_buffer_pt = 0;
		while(read_buffer_pt < read_size){
			for(uint64_t i = 0; i < len; i++){
				if(write_buffer_pt >= BUFFER_SIZE){
					if(write(fileout, write_buffer, write_buffer_pt) == -1) return -1;
					write_buffer_pt = 0;
				}
				*(uint64_t *)(write_buffer+write_buffer_pt) = commGF_mul(poly[i], *(uint64_t *)(read_buffer+read_buffer_pt));
				write_buffer_pt += 8;
			}
			read_buffer_pt += 8;
		}
		if(read_size < BUFFER_SIZE){
			if(write_buffer_pt > 0){
				if(write(fileout, write_buffer, write_buffer_pt) == -1) return -1;
			}
			break;
		}
	}
	if(read_size < 0) return (int)read_size;
	return 0;
}
int commGFPoly_FIFO_combine(int *filein, int fileout, uint64_t len){
	if(!filein) return -1;
	for(uint64_t i = 0; i < len; i++){
		if(filein[i] <= 0) return -1;
	}
	if(fileout <= 0) return -1;
	if(len == 0) return -1;
	char buffer_in[BUFFER_SIZE];
	char buffer_out[BUFFER_SIZE];
	ssize_t read_size;
	ssize_t m_read_size;
	char break_token = 1;
	while(break_token){
		memset(buffer_out, 0, BUFFER_SIZE);
		m_read_size = BUFFER_SIZE;
		for(uint64_t i = 0; i < len; i++){
			read_size = read(filein[i], buffer_in, BUFFER_SIZE);
			if(read_size < m_read_size) m_read_size = read_size;
			if(read_size < BUFFER_SIZE) break_token = 0;
			for(uint64_t j = 0; j < read_size; j+=8){
				*(uint64_t *)(buffer_out+j) ^= *(uint64_t *)(buffer_in+j);
			}
		}
		if(write(fileout, buffer_out, m_read_size) == -1) return -1;
	}
	return 0;
}
