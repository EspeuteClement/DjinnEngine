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

GAME_INIT_GRAPHIC(game_init_graphic)
{
    uGlLoadGL(memory->proc, &(memory->ctxt));
    memory->ctxt.glGenBuffers(1, &vertex_buffer);
    memory->ctxt.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    memory->ctxt.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vertex_shader = memory->ctxt.glCreateShader(GL_VERTEX_SHADER);
    memory->ctxt.glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    memory->ctxt.glCompileShader(vertex_shader);
    fragment_shader = memory->ctxt.glCreateShader(GL_FRAGMENT_SHADER);
    memory->ctxt.glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    memory->ctxt.glCompileShader(fragment_shader);
    program = memory->ctxt.glCreateProgram();
    memory->ctxt.glAttachShader(program, vertex_shader);
    memory->ctxt.glAttachShader(program, fragment_shader);
    memory->ctxt.glLinkProgram(program);
    mvp_location = memory->ctxt.glGetUniformLocation(program, "MVP");
    vpos_location = memory->ctxt.glGetAttribLocation(program, "vPos");
    vcol_location = memory->ctxt.glGetAttribLocation(program, "vCol");
    memory->ctxt.glEnableVertexAttribArray(vpos_location);
    memory->ctxt.glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    memory->ctxt.glEnableVertexAttribArray(vcol_location);
    memory->ctxt.glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
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