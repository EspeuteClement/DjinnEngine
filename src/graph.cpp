#include "graph.h"
#include "external/linmath.h"
#include "external/ugl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

unsigned char *test_image;

struct vertex_struct
{
    float x, y;
    float r, g, b;
    float u, v;
};

vertex_struct vertex_array[6] =
{
    {   0.0f, 0.0f, 1.f, 1.f, 1.f, 0.0f, 0.0f },
    {   0.0f, 240.0f, 1.f, 1.f, 1.f, 0.0f, 1.0f },
    {   512.0f, 240.0f, 1.f, 1.f, 1.f, 1.0f, 1.0f },
    {   0.0f, 0.0f, 1.f, 1.f, 1.f, 0.0f, 0.0f },
    {   512.0f, 0.0f, 1.f, 1.f, 1.f, 1.0f, 0.0f },
    {   512.0f, 240.0f, 1.f, 1.f, 1.f, 1.0f, 1.0f }
};

GLuint vertex_buffer_id;


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
"{\n" // vec4(color, 1.0) * 
"    gl_FragColor = texture2D(tex, texcoord);\n"
"}\n";

GLuint vertex_shader, fragment_shader, program;

GLuint tex;
GLint mvp_location;

#define FACTOR 80000
#define MAX_VERTEX FACTOR*7
struct
{
    vertex_struct v[MAX_VERTEX];
    uint64_t count;
} vertex_data;


void graph_init(Memory* memory)
{
    if (memory->proc)
    {
        uGlLoadGL((uGlLoadProc)memory->proc);
    }

    vertex_data.count = 0;

    glDisable(GL_MULTISAMPLE);

    glGenBuffers(1, &vertex_buffer_id); 
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id); 
    
    program = uGl_LoadProgram(vertex_shader_text, fragment_shader_text);

    GLuint vpos_location, vcol_location, vtex_location;

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

    int x,y,n;
    test_image = stbi_load("data/tiles.png", &x,&y,&n, 4);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_image);
}

void push_triangle(const vertex_struct& v1, const vertex_struct& v2, const vertex_struct& v3)
{
    assert(vertex_data.count+3 < MAX_VERTEX);

    vertex_data.v[vertex_data.count] = v1;
    vertex_data.count ++;
    vertex_data.v[vertex_data.count] = v2;
    vertex_data.count ++;
    vertex_data.v[vertex_data.count] = v3;
    vertex_data.count ++;
}

void push_quad(float x, float y, float w, float h, float ox, float oy, float ow, float oh)
{
    float u1 = ox/512.0f;
    float u2 = u1+ow/512.0f;

    float v1 = oy/240.0f;
    float v2 = v1+oh/240.0f;

    float x2 = x + w;
    float y2 = y + h;

    {
        vertex_struct s1 = {   x, y, 1.f, 1.f, 1.f,      u1, v1 };
        vertex_struct s2 = {   x, y2, 1.f, 1.f, 1.f,     u1, v2 };
        vertex_struct s3 = {   x2, y2, 1.f, 1.f, 1.f,    u2, v2 };
        push_triangle
        (
            s1,
            s2,
            s3
        );
    }

    {
        vertex_struct s1 = {   x, y, 1.f, 1.f, 1.f,      u1, v1 };
        vertex_struct s2 = {   x2, y, 1.f, 1.f, 1.f,     u2, v1 };
        vertex_struct s3 = {   x2, y2, 1.f, 1.f, 1.f,    u2, v2 };

        push_triangle
        (
            s1,
            s2,
            s3
        );
    }

}

bool is_init = false;
void graph_draw_all(Memory* memory)
{
    
    /*push_triangle
    (
            {   0.0f, 0.0f, 1.f, 1.f, 1.f,      16.0f/512.0f, 16.0f/240.0f },
            {   0.0f, 64.0f, 1.f, 1.f, 1.f,     16.0f/512.0f, 2*16.0f/240.0f },
            {   64.0f, 64.0f, 1.f, 1.f, 1.f,    2*16.0f/512.0f, 2*16.0f/240.0f }
    );*/

    if (!is_init)
    {
        if (memory->x % 1 == 0)
        {
            vertex_data.count = 0;
            for (uint32_t i = 0; i < FACTOR; i++)
            {
                push_quad(rand()%512,rand()%256,   16,16,      (rand()%16)*16,(rand()%16)*16,      16,16);
            }
        }

        glBufferData(GL_ARRAY_BUFFER, vertex_data.count * sizeof(vertex_struct), vertex_data.v, GL_STREAM_DRAW); 

        is_init = false;
    }


    // 512 240
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    ratio = memory->screen_width / (float) memory->screen_height;
    glViewport(0, 0, memory->screen_width, memory->screen_height);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4x4_identity(m);
    //mat4x4_translate(m, memory->screen_width/2, memory->screen_height/2, 0);
    //mat4x4_rotate_Z(m, m, (float) memory->x/50);


    mat4x4_ortho(p, 0, memory->screen_width, memory->screen_height, 0, 0, 1.f);
    mat4x4_mul(mvp, p, m);
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);


    glDrawArrays(GL_TRIANGLES, 0, vertex_data.count);
}