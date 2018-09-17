#include "djn_alloc.h"
#include <stdlib.h>

#if DJN_ALLOC_LEAK_DETECT
#include <stdio.h>
int32_t current_alloc_count = 0; 
#endif

#if DJN_ALLOC_LOG

void* ___djn_alloc_log(size_t size, const char* file, int line)
{
	void* ptr = ___djn_alloc(size);
	printf("ALLOC_LOG : ++ %p ++ %s:%d\n",ptr,file, line);
	return ptr;
}

void* ___djn_calloc_log(size_t size, uint8_t value, const char* file, int line)
{
	void* ptr = ___djn_calloc(size,value);
	printf("ALLOC_LOG : +0 %p 0+ %s:%d\n",ptr,file, line);
	return ptr;
}

void* ___djn_realloc_log(void* ptr, size_t new_size, const char* file, int line)
{
	void* new_ptr = ___djn_realloc(ptr, new_size);
	if (ptr == NULL)
	{
		printf("ALLOC_LOG : +r %p r+ %s:%d\n",new_ptr,file, line);
	}
	return new_ptr;
}


void  ___djn_free_log(void* ptr, const char* file, int line)
{
	if (ptr != NULL)
	{
		printf("ALLOC_LOG : -- %p -- %s:%d\n", ptr, file, line);
	}
	else
	{
		printf("ALLOC_LOG : warn : freeing null ptr here : %s:%d \n", file, line);
	}
	return ___djn_free(ptr);
}

#endif


void* ___djn_alloc(size_t size)
{
	#if DJN_ALLOC_LEAK_DETECT
		current_alloc_count++; 
	#endif
	return malloc(size);
}

void* ___djn_calloc(size_t size, uint8_t value)
{
	#if DJN_ALLOC_LEAK_DETECT
		current_alloc_count++; 
	#endif
	return calloc(size, value);
}

void ___djn_free(void* ptr)
{
	#if DJN_ALLOC_LEAK_DETECT
		current_alloc_count-= (ptr != NULL); 
	#endif
	free(ptr);
}

void* ___djn_realloc(void* ptr, size_t new_size)
{
	#if DJN_ALLOC_LEAK_DETECT
		current_alloc_count += (ptr == NULL); 
	#endif
	return realloc(ptr, new_size);
}

void djn_memory_leak_report()
{
#if DJN_ALLOC_LEAK_DETECT
	if (current_alloc_count == 0)
	{
		printf("No leak found !\n");
	}
	else
	{
		printf("Found %d leaks !\n", current_alloc_count);
	}
#endif
}

