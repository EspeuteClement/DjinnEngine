#include "djn_start.h"
#define OBJ_NAME hero

#include <stdio.h>

typedef struct
{
	obj_base base;
	int other_property;
	int another_stuff;
} OBJ_NAME;

#include "djn_step.h"
{
	self->other_property = 0;
}

#include "djn_draw.h"
{
	printf("I'm the object : %d\n", self->other_property);
}

#include "djn_end.h"