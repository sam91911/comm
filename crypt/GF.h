#ifndef COMM_CRYPT_GF
#define COMM_CRYPT_GF
#include <stdint.h>

uint64_t commGF_mul(uint64_t a, uint64_t b);
uint64_t commGF_inv(uint64_t a);

#endif
