#ifndef H_STRING
#define H_STRING

#include <stddef.h>
#include <stdint.h>

void* memcpy(void* dst, void* src, size_t len);
void* memset(void* dst, uint8_t value, size_t len);
char* strrev(char* buffer);
size_t strlen(char* str);


#endif