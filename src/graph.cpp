#include "graph.h"
#include "external/linmath.h"
#include "external/ugl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include "tools/djinn_pack_api.h"
#include "external/imgui/imgui.h"

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
"uniform vec2 uTexSize;\n"
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
/*"   if(gl_FragColor.a == 0.0f)"
"   {"
"       discard;"
"   }"*/
"}\n";

GLuint vertex_shader, fragment_shader, program;

GLuint tex;
GLint mvp_location;

#define FACTOR 6000
#define MAX_VERTEX FACTOR*7
struct
{
    vertex_struct v[MAX_VERTEX];
    uint64_t count;
} vertex_data;

void djn_init_image(Memory* memory)
{
    int n;
    test_image = stbi_load("data/pack.png", &(memory->graph.img_width),&(memory->graph.img_height),&n, 4);

    pack_final data;
    pack_open("data/pack.dat", data, "r");

    //data.pack_data_buffer = &(memory->graph.data[0]);
    
    // @TODO : Allocation in Frame Allocation
    data.pack_data_buffer = (pack_data*) malloc(data.num_images * sizeof(pack_data));
    data.pack_name_buffer = (pack_name*) malloc(data.num_images * sizeof(pack_name));

    pack_read(data);
    
    //int index = pack_find("Hector1.png", data.pack_name_buffer, data.num_images);
    //memory->graph.test_image_data = data.pack_data_buffer[index];

    // Tranform packed data into usable data
    uint32_t count = data.num_images;
    for (int current_image_index = 0; current_image_index < count; ++current_image_index)
    {
        pack_data & packed = data.pack_data_buffer[current_image_index];
        djn_quad  & quad = memory->graph.data[current_image_index];

        quad.offset.x = packed.ox;
        quad.offset.y = packed.oy;

        quad.originalSize.x = packed.ow;
        quad.originalSize.y = packed.oh;

        quad.uv[0].x        = (float) (packed.q.u1)  / (float) (memory->graph.img_width);
        quad.uv[0].y        = (float) (packed.q.v1)  / (float) (memory->graph.img_height);
        quad.uv[1].x        = (float) (packed.q.u2)  / (float) (memory->graph.img_width);
        quad.uv[1].y        = (float) (packed.q.v2)  / (float) (memory->graph.img_height);

        quad.quadSize.x = packed.q.u2 - packed.q.u1;
        quad.quadSize.y = packed.q.v2 - packed.q.v1;

    }

    pack_close(data);
    free(data.pack_data_buffer);
    free(data.pack_name_buffer);

    assert(test_image != nullptr);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (memory->graph.img_width),(memory->graph.img_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, test_image);
}

void djn_init_opengl(Memory* memory)
{
    if (memory->proc)
    {
        uGlLoadGL((uGlLoadProc)memory->proc);
    }
    
    vertex_data.count = 0;

    glDisable(GL_MULTISAMPLE);

    glGenBuffers(1, &vertex_buffer_id); 
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id); 

    program = uGl_LoadProgram_File("data/base.vert", "data/base.frag");
    //program = uGl_LoadProgram(vertex_shader_text, fragment_shader_text);

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

    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void djn_gfx_init(Memory* memory)
{
    djn_init_opengl(memory);
    djn_init_image(memory);
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
    float u1 = ox;
    float u2 = ow;

    float v1 = oy;
    float v2 = oh;

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

void push_sprite(int id, float x, float y, float scale_w = 1.0f, float scale_h = 1.0f)
{
    
    djn_quad & d = djn_memory->graph.data[id];

    push_quad(x+d.offset.x,y+d.offset.y, d.quadSize.x, d.quadSize.y * scale_h, d.uv[0].x, d.uv[0].y, d.uv[1].x, d.uv[1].y);
}

void djn_gfx_begin(Memory* memory)
{
    vertex_data.count = 0;
}


void djn_gfx_draw_vertex_data(Memory* memory)
{
    glBufferData(GL_ARRAY_BUFFER, vertex_data.count * sizeof(vertex_struct), vertex_data.v, GL_STREAM_DRAW); 
    glDrawArrays(GL_TRIANGLES, 0, vertex_data.count);
}

void djn_gfx_setup_view(Memory* memory)
{
    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    ratio = memory->screen_width / (float) memory->screen_height;
    glViewport(0, 0, memory->screen_width, memory->screen_height);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4x4_identity(m);
    mat4x4_translate(m, memory->screen_width/2, memory->screen_height/2, 0);
    mat4x4_rotate_Z(m, m, (float) memory->x/50);
    mat4x4_translate_in_place(m, -memory->screen_width/2, -memory->screen_height/2, 0);
    mat4x4_scale_aniso(m,m,memory->screen_width/400,memory->screen_height/300,1);

    memory->x ++;

    mat4x4_ortho(p, 0, memory->screen_width, memory->screen_height, 0, 0, 1.f);
    mat4x4_mul(mvp, p, m);
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
}

void djn_gfx_end(Memory* memory)
{
    djn_gfx_setup_view(memory);
    djn_gfx_draw_vertex_data(memory);
}

bool is_init = false;
void djn_gfx_draw_all(Memory* memory)
{
    djn_gfx_begin(memory);

    push_sprite((memory->x++/4)%221, 64,64);

    djn_gfx_end(memory);
}


// Test draw image imgui
#if 0
    ImGui::Begin("Atlas Explorer", 0, ImGuiWindowFlags_HorizontalScrollbar);
    ImTextureID id = (void *)(intptr_t) tex;
    ImGui::Image(id, ImVec2(memory->graph.img_width, memory->graph.img_height), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
    ImGui::End();
#endif

// Benchmark draw tons of triangles
#if 0
    if (!is_init)
    {
        if (memory->x % 1 == 0)
        {
            vertex_data.count = 0;
            for (uint32_t i = 0; i < FACTOR; i++)
            {
                push_quad((i%64) * 16,(i/64) * 16,   16,16,      ((i*871+(i/64)*147)%16)*16,((i*0x777121+(i/16)*789)%16)*16,      16,16);
            }
        }

        glBufferData(GL_ARRAY_BUFFER, vertex_data.count * sizeof(vertex_struct), vertex_data.v, GL_STREAM_DRAW); 

        is_init = false;
    }
#endif