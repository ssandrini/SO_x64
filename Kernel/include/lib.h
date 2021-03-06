#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
extern void _int20();
char *cpuVendor(char *result);
void strcpy(char *src, char *dest);
int strlen(char *string);
#endif