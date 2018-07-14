#include "game.h"
#include <cstdio>
#include "external/linmath.h"
#include "external/ugl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 1.f },
    {  0.6f, -0.4f, 0.f, 1.f, 1.f },
    {   0.f,  0.6f, 1.f, 1.f, 0.f }
};

static const struct
{
    float x, y;
    float r, g, b;
    float u, v;
} new_vertices[3] =
{
    {   0.0f, 0.0f, 1.f, 1.f, 1.f, 0.0f, 0.0f },
    {   0.0f, 1.0f, 1.f, 1.f, 1.f, 0.0f, 1.0f },
    {   1.0f, 1.0f, 1.f, 1.f, 1.f, 1.0f, 1.0f }
};


static const char* vertex_shader_text =
"#version 100\n"
"#ifdef GL_ES\n"
"precision mediump float;\n"
"#endif\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"attribute vec2 vTexCoord;\n"
"varying vec2 texcoord;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    texcoord = vTexCoord;\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"#version 100\n"
"#ifdef GL_ES\n"
"precision mediump float;\n"
"#endif\n"
"varying vec3 color;\n"
"varying vec2 texcoord;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0) * texture2D(tex, texcoord);\n"
"}\n";

    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location, vtex_location;
    GLuint tex;

unsigned char *test_image;

GAME_INIT_GRAPHIC(game_init_graphic)
{
    printf("Hello world\n");
    if (memory->proc)
    {
        uGlLoadGL((uGlLoadProc)memory->proc);
    }

    glGenBuffers(1, &vertex_buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(new_vertices), new_vertices, GL_STATIC_DRAW); 
    
    program = uGl_LoadProgram(vertex_shader_text, fragment_shader_text);

    mvp_location = glGetUniformLocation(program, "MVP"); 
    vpos_location = glGetAttribLocation(program, "vPos"); 
    vcol_location = glGetAttribLocation(program, "vCol");
    vtex_location = glGetAttribLocation(program, "vTexCoord");

    glEnableVertexAttribArray(vpos_location); 
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 7, (void*) 0); 
    glEnableVertexAttribArray(vcol_location); 
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 7, (void*) (sizeof(float) * 2));

    glEnableVertexAttribArray(vtex_location);
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 7, (void*) (sizeof(float) * 5));

    printf("Opengl inited\n");

    int x,y,n;
    test_image = stbi_load("data/tiles.png", &x,&y,&n, 4);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_image);

}


GAME_LOOP(game_loop)
{
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    ratio = memory->screen_width / (float) memory->screen_height;
    glViewport(0, 0, memory->screen_width, memory->screen_height);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4x4_identity(m);

    mat4x4_rotate_Z(m, m, (float) -memory->x/50);


    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    //printf("%5d : FUCK THE RULES ! !\n", memory->screen_width);
    memory->x ++ ;
}

GAME_UNLOAD_GRAPHIC(game_unload_graphic)
{
    stbi_image_free(test_image);
    // Do nothing at the moment
}