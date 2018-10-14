#include "ugl/ugl.h"
#include <assert.h>
#include "djn_debug.h"
#include <memory.h>

#include "djn_graph.h"
#include "external/linmath.h"

#include <SDL.h>

#define TARGET_WIDTH SCREEN_W
#define TARGET_HEIGHT SCREEN_H

#define MAX_VERTEX (200 * 6)
struct
{
    vertex_data v[MAX_VERTEX];
    uint64_t count;
} vertex_buffer;

GLuint vbo;
GLuint vao;

GLuint program;
GLuint vertexbuffer;

GLuint mvp_location;

void djn_graph_init()
{
	vertex_buffer.count = 0;
	memset(&vertex_buffer.v, 0, sizeof(vertex_buffer.v));

	program = uGl_LoadProgram_File("engine/shaders/base.vert", "engine/shaders/base.frag");
	glGenBuffers(1, &vertexbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint mvp_location = glGetUniformLocation(program, "MVP");
    GLuint vpos_location = glGetAttribLocation(program, "vPos"); 
    GLuint vcol_location = glGetAttribLocation(program, "vCol");
    GLuint vtex_location = glGetAttribLocation(program, "vTexCoord");

    //dump_memory("vtex_location", (void*)&vtex_location, sizeof(vtex_location));

    // 3 pos
    glEnableVertexAttribArray(vpos_location); 
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) 0); 
    // 4 color
    glEnableVertexAttribArray(vcol_location); 
    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) (sizeof(float) * 3));
    // 2 uv
    glEnableVertexAttribArray(vtex_location);
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) (sizeof(float) * 7));

	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw_triangle(const vertex_data* v1, const vertex_data* v2, const vertex_data* v3)
{
    assert(vertex_buffer.count+3 < MAX_VERTEX);

    vertex_buffer.v[vertex_buffer.count] = *v1;
    vertex_buffer.count ++;
    vertex_buffer.v[vertex_buffer.count] = *v2;
    vertex_buffer.count ++;
    vertex_buffer.v[vertex_buffer.count] = *v3;
    vertex_buffer.count ++;
}

void draw_quad(float x, float y, float z, float w, float h, float ox, float oy, float ow, float oh)
{
    float u1 = ox;
    float u2 = ow;

    float v1 = oy;
    float v2 = oh;

    float x2 = x + w;
    float y2 = y + h;

    {
        vertex_data s1 = {   x, y, 	z, 1.f, 1.f, 1.f, 1.f,      u1, v1 };
        vertex_data s2 = {   x, y2, z, 1.f, 1.f, 1.f, 1.f,     u1, v2 };
        vertex_data s3 = {   x2, y2,z, 1.f, 1.f, 1.f, 1.f,    u2, v2 };
        draw_triangle
        (
            &s1,
            &s2,
            &s3
        );
    }

    {
        vertex_data s1 = {   x, y, 	z, 1.f, 1.f, 1.f, 1.f,      u1, v1 };
        vertex_data s2 = {   x2, y, z, 1.f, 1.f, 1.f, 1.f,     u2, v1 };
        vertex_data s3 = {   x2, y2,z, 1.f, 1.f, 1.f, 1.f,    u2, v2 };

        draw_triangle
        (
            &s1,
            &s2,
            &s3
        );
    }
}

void djn_graph_draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
    
    float ratio;
//    int width, height;
    mat4x4 m, p, mvp;

    ratio = TARGET_HEIGHT / (float) TARGET_WIDTH;
    glViewport(0, 0, TARGET_WIDTH, TARGET_HEIGHT);
    mat4x4_identity(m);

    mat4x4_ortho(p, 0, TARGET_WIDTH, TARGET_HEIGHT, 0, 0, 1.f);
    mat4x4_mul(mvp, p, m);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);

    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertex_buffer.count * sizeof(vertex_data), vertex_buffer.v, GL_STREAM_DRAW); 
	glDrawArrays(GL_TRIANGLES, 0, (GLuint) vertex_buffer.count);

	vertex_buffer.count = 0;
}