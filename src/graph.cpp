#include "graph.h"
#include "external/linmath.h"
#include "external/ugl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include "tools/djinn_pack_api.h"
#include "external/imgui/imgui.h"

#include "game.h"

unsigned char *test_image;
Graph* graph;

struct vertex_struct
{
    float x, y;
    float r, g, b;
    float u, v;
};

GLuint vbo;
GLuint vao;


GLuint vertex_shader, fragment_shader, program;
GLuint render_buffer;
GLuint render_buffer_texture;
GLuint render_buffer_vertex_vbo;
GLuint render_buffer_vertex_vao;

GLuint tex;
GLint mvp_location;

#define TARGET_WIDTH 320
#define TARGET_HEIGHT 240

#define FACTOR 200
#define MAX_VERTEX FACTOR*7
struct
{
    vertex_struct v[MAX_VERTEX];
    uint64_t count;
} vertex_data;

static const vertex_struct render_buffer_vertex[6] =
{
    { 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,                               0.0f, 1.0f },
    { TARGET_WIDTH, 0.0f,   1.0f, 1.0f, 1.0f,                       1.0f, 1.0f },
    { TARGET_WIDTH, TARGET_HEIGHT,   1.0f, 1.0f, 1.0f,              1.0f, 0.0f },
    
    { 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,                               0.0f, 1.0f },
    { 0.0f, TARGET_HEIGHT,   1.0f, 1.0f, 1.0f,                      0.0f, 0.0f },
    { TARGET_WIDTH, TARGET_HEIGHT,   1.0f, 1.0f, 1.0f,              1.0f, 0.0f }
};

void djn_init_image(Memory* memory)
{
    int n;
    test_image = stbi_load("data/pack.png", &(graph->img_width),&(graph->img_height),&n, 4);

    pack_final data;
    pack_open("data/pack.dat", data, "r");
    
    // @TODO : Allocation in Frame Allocation
    data.pack_data_buffer = (pack_data*) malloc(data.num_images * sizeof(pack_data));
    data.pack_name_buffer = (pack_name*) malloc(data.num_images * sizeof(pack_name));

    pack_read(data);

    // Tranform packed data into usable data
    uint32_t count = data.num_images;
    for (int current_image_index = 0; current_image_index < count; ++current_image_index)
    {
        pack_data & packed = data.pack_data_buffer[current_image_index];
        djn_quad  & quad = graph->data[current_image_index];

        quad.offset.x = packed.ox;
        quad.offset.y = packed.oy;

        quad.originalSize.x = packed.ow;
        quad.originalSize.y = packed.oh;

        quad.uv[0].x        = (float) (packed.q.u1)  / (float) (graph->img_width);
        quad.uv[0].y        = (float) (packed.q.v1)  / (float) (graph->img_height);
        quad.uv[1].x        = (float) (packed.q.u2)  / (float) (graph->img_width);
        quad.uv[1].y        = (float) (packed.q.v2)  / (float) (graph->img_height);

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (graph->img_width),(graph->img_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, test_image);
}

void djn_init_opengl(Memory* memory)
{
    if (memory->proc)
    {
        uGlLoadGL((uGlLoadProc)memory->proc);
    }
    
    vertex_data.count = 0;

    glDisable(GL_MULTISAMPLE);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


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


    // ===================================================================================================

    glGenFramebuffers(1, &render_buffer);

    glBindFramebuffer(GL_FRAMEBUFFER, render_buffer);

    glGenTextures(1, &render_buffer_texture);

    glBindTexture(GL_TEXTURE_2D, render_buffer_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TARGET_WIDTH, TARGET_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_buffer_texture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);
    glGenVertexArrays(1, &render_buffer_vertex_vao);
 
    glBindVertexArray(render_buffer_vertex_vao);
    glGenBuffers(1, &render_buffer_vertex_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, render_buffer_vertex_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(render_buffer_vertex), render_buffer_vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vpos_location); 
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 7, (void*) 0); 
    glEnableVertexAttribArray(vcol_location); 
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 7, (void*) (sizeof(float) * 2));

    glEnableVertexAttribArray(vtex_location);
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 7, (void*) (sizeof(float) * 5));
}

void djn_gfx_init(Memory* memory)
{
    graph = (Graph*) malloc(sizeof(Graph));
    djn_init_opengl(memory);
    djn_init_image(memory);
}

void djn_gfx_deinit(Memory* memory)
{
    free(graph);
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
    
    djn_quad & d = graph->data[id];

    push_quad(x+d.offset.x,y+d.offset.y, d.quadSize.x, d.quadSize.y * scale_h, d.uv[0].x, d.uv[0].y, d.uv[1].x, d.uv[1].y);
}

void djn_gfx_begin(Memory* memory)
{
    vertex_data.count = 0;
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}


void djn_gfx_draw_vertex_data(Memory* memory)
{
    glBindTexture(GL_TEXTURE_2D, tex);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.count * sizeof(vertex_struct), vertex_data.v, GL_STREAM_DRAW); 
    glDrawArrays(GL_TRIANGLES, 0, vertex_data.count);
}

void djn_gfx_setup_view(Memory* memory)
{
    glClear(GL_COLOR_BUFFER_BIT);
    

    float ratio;
    int width, height;
    mat4x4 m, p, mvp;

    glBindFramebuffer(GL_FRAMEBUFFER, render_buffer);
    glClear(GL_COLOR_BUFFER_BIT);

    ratio = TARGET_HEIGHT / (float) TARGET_WIDTH;
    glViewport(0, 0, TARGET_WIDTH, TARGET_HEIGHT);


    mat4x4_identity(m);
   //mat4x4_translate(m, memory->screen_width/2, memory->screen_height/2, 0);
   //mat4x4_rotate_Z(m, m, (float) memory->x/50);
   //mat4x4_translate_in_place(m, -memory->screen_width/2, -memory->screen_height/2, 0);
   //mat4x4_scale_aniso(m,m,memory->screen_width/400,memory->screen_height/300,1);

    mat4x4_ortho(p, 0, TARGET_WIDTH, TARGET_HEIGHT, 0, 0, 1.f);
    mat4x4_mul(mvp, p, m);
    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
}

void djn_draw_game_normal(Memory* memory)
{
    int height = TARGET_HEIGHT*2;

    glViewport(0, memory->screen_height - height,TARGET_WIDTH*2, height);
    glBindVertexArray(render_buffer_vertex_vao);
    glBindBuffer(GL_ARRAY_BUFFER, render_buffer_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(render_buffer_vertex), render_buffer_vertex, GL_STATIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, render_buffer_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void djn_gfx_end(Memory* memory)
{
    djn_gfx_setup_view(memory);
    djn_gfx_draw_vertex_data(memory);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    switch (memory->debug.game_draw_mode)
    {
        case GDM_Normal:
        {
            djn_draw_game_normal(memory);
            break;
        }
        case GDM_Window:
        {
            ImGui::Begin("Game", 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
            ImTextureID id = (void *)(intptr_t) render_buffer_texture;
            ImGui::Image(id, ImVec2(TARGET_WIDTH*2,TARGET_HEIGHT*2), ImVec2(0,1), ImVec2(1,0), ImColor(255,255,255,255), ImColor(255,255,255,128));
            ImGui::End();
            break;
        }
        default:
        {
            break;
        }
    }

}

bool is_init = false;
void djn_gfx_draw_all(Memory* memory)
{
    djn_gfx_begin(memory);

    push_sprite(0, memory->x,memory->y);

    djn_gfx_end(memory);
}


// Test draw image imgui
#if 0
    ImGui::Begin("Atlas Explorer", 0, ImGuiWindowFlags_HorizontalScrollbar);
    ImTextureID id = (void *)(intptr_t) tex;
    ImGui::Image(id, ImVec2(graph->img_width, graph->img_height), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
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