#ifndef COMM_GF_POLY
#define COMM_GF_POLY
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "GF.h"

uint64_t commGFPoly_insert(uint64_t *poly, uint64_t x, uint64_t len);
int commGFPoly_RMatrix(uint64_t *x, uint64_t *RMatrix, uint64_t len);
int commGFPoly_reverse(uint64_t *y, uint64_t *RMatrix, uint64_t *poly, uint64_t len);
int commGFPoly_FIFO_insert(int filein, int fileout, uint64_t x, uint64_t len);
int commGFPoly_FIFO_reverse(int filein, int fileout, uint64_t *poly, uint64_t len);
int commGFPoly_FIFO_combine(int *filein, int fileout, uint64_t len);

#endif
