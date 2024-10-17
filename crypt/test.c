#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "GF.h"
#include "GFPoly.h"

static uint64_t rand64(){
	uint64_t rt = rand();
	rt = rt<<32 | rand();
	return rt;
}

int main(int argc, char **argv){
	if(argc < 2) return 0;
	int len = 256;
	uint64_t key[len];
	uint64_t reverse_poly[len];
	char path_buffer[256];
	for(int i = 0; i < len; i++){
		key[i] = rand64();
	}
	int file_target = open(argv[1], O_RDONLY);
	int fileout;
	if(file_target <= 0) return -1;
	for(int i = 0; i < len; i++){
		sprintf(path_buffer, "cache/%016lX_insert", key[i]);
		lseek(file_target, 0, SEEK_SET);
		fileout = open(path_buffer, O_WRONLY|O_CREAT, 0666);
		if(fileout <= 0) return -1;
		commGFPoly_FIFO_insert(file_target, fileout, key[i], len);
		close(fileout);
	}
	close(file_target);
	for(int i = 0; i < len; i++){
		sprintf(path_buffer, "cache/%016lX_insert", key[i]);
		file_target = open(path_buffer, O_RDONLY);
		sprintf(path_buffer, "cache/%016lX_reverse", key[i]);
		fileout = open(path_buffer, O_WRONLY|O_CREAT, 0666);
		commGFPoly_reverse_poly(key, reverse_poly, len, i);
		commGFPoly_FIFO_reverse(file_target, fileout, reverse_poly, len);
		close(file_target);
		close(fileout);
		sprintf(path_buffer, "cache/%016lX_insert", key[i]);
		remove(path_buffer);
	}
	int file_reverse[len];
	for(int i = 0; i < len; i++){
		sprintf(path_buffer, "cache/%016lX_reverse", key[i]);
		file_reverse[i] = open(path_buffer, O_RDONLY);
	}
	file_target = open("result", O_WRONLY|O_CREAT, 0666);
	commGFPoly_FIFO_combine(file_reverse, file_target, len);
	for(int i = 0; i < len; i++){
		close(file_reverse[i]);
		sprintf(path_buffer, "cache/%016lX_reverse", key[i]);
		remove(path_buffer);
	}
	close(file_target);
	return 0;
}
