#ifndef COMM_CRYPT_BYTES
#define COMM_CRYPT_BYTES
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int commBytes_hex_encode(char *dst, unsigned char *src, uint64_t len);
int commBytes_hex_decode(unsigned char *dst, char *src, uint64_t len);
int commBytes_base64_encode(char *dst, char *src, uint64_t len);
int commBytes_base64_decode(char *dst, char *src, uint64_t len);
#endif
