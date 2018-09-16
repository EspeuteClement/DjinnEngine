#ifndef _STEP_IMPLEMENTED_
#include "djn_step.h"
{
	int i = 0;
}
#endif

#ifndef _DRAW_IMPLEMENTED_
#include "djn_draw.h"
{
	printf("I'm the object : %d\n", self->base->x);
}
#endif

OBJ_NAME * EVALUATOR(OBJ_NAME, ctor) ()
{
	OBJ_NAME * ptr = (OBJ_NAME *) malloc(sizeof(OBJ_NAME), 0);
	ptr->base.step = &EVALUATOR(OBJ_NAME, step);
	ptr->base.draw = &EVALUATOR(OBJ_NAME, draw);

	return ptr;
}

#undef _STEP_IMPLEMENTED_
#undef _DRAW_IMPLEMENTED_