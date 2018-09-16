#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)

typedef struct
{
	void (*step)(void * self);
	void (*draw)(void * self);
	int id;
	int x;
	int y;
} obj_base;