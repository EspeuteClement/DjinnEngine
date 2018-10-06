#ifndef _DJN_ALLOC_H_
#define _DJN_ALLOC_H_

#include <stdint.h>

#define DJN_ALLOC_LEAK_DETECT 1
#define DJN_ALLOC_LOG 1

#if DJN_ALLOC_LOG

#define djn_alloc(size) ___djn_alloc_log(size, __FILE__, __LINE__)
#define djn_calloc(size) ___djn_calloc_log(size, __FILE__, __LINE__)
#define djn_realloc(ptr, new_size) ___djn_realloc_log(ptr, new_size, __FILE__, __LINE__)
#define djn_free(ptr) ___djn_free_log(ptr, __FILE__, __LINE__)

void* ___djn_alloc_log(size_t size, const char* file, int line);
void* ___djn_calloc_log(size_t size, const char* file, int line);
void* ___djn_realloc_log(void* ptr, size_t new_size, const char* file, int line);
void  ___djn_free_log(void* ptr, const char* file, int line);

#else // DJN_ALLOC_LOG

#define djn_alloc(size) ___djn_alloc(size)
#define djn_calloc(size) ___djn_calloc(size)
#define djn_realloc(ptr, new_size) ___djn_realloc(ptr, new_size)
#define djn_free(ptr) ___djn_free(ptr)

#endif

void* ___djn_alloc(size_t size);
void* ___djn_calloc(size_t size);
void* ___djn_realloc(void* ptr, size_t new_size);
void  ___djn_free(void* ptr);

void djn_memory_leak_report();

#endif