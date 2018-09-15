// Debug library
#include <stdio.h>
#include "djn_debug.h"

void dump_memory(const char * name, const unsigned char* buffer, size_t length)
{
	fprintf(stdout, "%s:\n", name);
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%02x ", (int)buffer[i]);
		if (i % 16 == 15)
		{
			fprintf(stdout, "\n");
		}
	}
	getchar();
}