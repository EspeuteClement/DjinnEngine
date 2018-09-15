#ifndef _DJN_GRAPH_H_
#define _DJN_GRAPH_H_

#define SCREEN_W 800
#define SCREEN_H 600

typedef struct 
{
	float x,y,z;
	float r,g,b,a;
	float u, v;
} vertex_data;

void draw_triangle(const vertex_data* v1, const vertex_data* v2, const vertex_data* v3);
void draw_quad(float x, float y, float z, float w, float h, float ox, float oy, float ow, float oh);

#endif