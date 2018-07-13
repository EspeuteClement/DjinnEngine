#include "game.h"
#include <cstdio>
#include "external/linmath.h"

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 1.f, 1.f },
    {  0.6f, -0.4f, 1.f, 1.f, 1.f },
    {   0.f,  0.6f, 1.f, 1.f, 1.f }
};
static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

Memory* mem;

GAME_INIT_GRAPHIC(game_init_graphic)
{
    mem = memory;
    uGlLoadGL(memory->proc, &(memory->ctxt));

    mem->ctxt.glGenBuffers(1, &vertex_buffer);
    mem->ctxt.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    mem->ctxt.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vertex_shader = mem->ctxt.glCreateShader(GL_VERTEX_SHADER);
    mem->ctxt.glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    mem->ctxt.glCompileShader(vertex_shader);
    fragment_shader = mem->ctxt.glCreateShader(GL_FRAGMENT_SHADER);
    mem->ctxt.glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    mem->ctxt.glCompileShader(fragment_shader);
    program = mem->ctxt.glCreateProgram();
    mem->ctxt.glAttachShader(program, vertex_shader);
    mem->ctxt.glAttachShader(program, fragment_shader);
    mem->ctxt.glLinkProgram(program);
    mvp_location = mem->ctxt.glGetUniformLocation(program, "MVP");
    vpos_location = mem->ctxt.glGetAttribLocation(program, "vPos");
    vcol_location = mem->ctxt.glGetAttribLocation(program, "vCol");
    mem->ctxt.glEnableVertexAttribArray(vpos_location);
    mem->ctxt.glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    mem->ctxt.glEnableVertexAttribArray(vcol_location);
    mem->ctxt.glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));
}


GAME_LOOP(game_loop)
{
    /*float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    ratio = memory->screen_width / (float) memory->screen_height;
    glViewport(0, 0, memory->screen_width, memory->screen_height);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4x4_identity(m);

    
    mat4x4_rotate_Z(m, m, (float) -memory->x/5);


    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawArrays(GL_TRIANGLES, 0, 3);*/


    printf("%5d : Sup ! !\n", memory->x);
    memory->x ++ ;
}

GAME_UNLOAD_GRAPHIC(game_unload_graphic)
{
    // Do nothing at the moment
}