#include "bytes.h"

static char hex_encode_map[] = "0123456789ABCDEF";
static unsigned char hex_decode_map[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, //0-9
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //:;<=>?@
	0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f//a-f
};

int commBytes_hex_encode(char *dst, unsigned char *src, uint64_t len){
	if(!dst) return -1;
	if(!src) return -1;
	for(uint64_t i = 0; i < len; i++){
		dst[2*i] = hex_encode_map[src[i]>>4];
		dst[2*i+1] = hex_encode_map[src[i]&0x0f];
	}
	dst[2*len] = 0;
	return 0;
}
int commBytes_hex_decode(unsigned char *dst, char *src, uint64_t len){
	if(!dst) return -1;
	if(!src) return -1;
	for(uint64_t i = 0; i < len; i++){
		dst[i] = (hex_decode_map[src[2*i]-48]<<4)|(hex_decode_map[src[2*i+1]-48]);
	}
	return 0;
}
int commBytes_base64_encode(char *dst, char *src, uint64_t len);
int commBytes_base64_decode(char *dst, char *src, uint64_t len);
