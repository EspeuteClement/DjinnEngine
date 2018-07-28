#include "ugl.h"

#include <cstdio>
#include <cstdio>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

// Function true storage
#define _TYPEDECL(x)
#define _FUNCNAME(name) UGL_HANDLE(name) name
#define _PARAMS(x)
#include "ugl.incl"

int uGlLoadGL(uGlLoadProc proc)
{
    // Load function
    #define _TYPEDECL(x)
    #define _FUNCNAME(name) name = (UGL_HANDLE(name)) proc(#name)
    #define _PARAMS(x)
    #include "ugl.incl"

    return 0;
}

uGLAPI GLuint uGl_LoadShader(GLenum type, const GLchar * source)
{
    GLuint shader = glCreateShader(type); 
    glShaderSource(shader, 1, &source, NULL); 
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLchar errbuff[1024];
        GLint length = 0;
        glGetShaderInfoLog(shader, 1024, &length, &errbuff[0]);

        printf("Error compiling shader : %s", errbuff);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

uGLAPI GLuint uGl_LoadProgram(const GLchar * source_vertex, const GLchar * source_fragment)
{
    GLuint  vertex_shader   = uGl_LoadShader(GL_VERTEX_SHADER, source_vertex);
    GLuint  fragment_shader = uGl_LoadShader(GL_FRAGMENT_SHADER,source_fragment);

    GLuint  program         = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    return program;
}

// Don't forget to free the returned string
char * load_file(const char * file_path)
{
    char * buffer = nullptr;
    long length;
    FILE * f = fopen (file_path, "rb");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char*) malloc (length+1);
      if (buffer)
      {
        fread (buffer, 1, length, f);
        buffer[length] = 0;
      }
      fclose (f);
    }

    return buffer;
} 

uGLAPI GLuint uGl_LoadProgram_File(const char * source_vertex_file, const char * source_fragment_file)
{
    char const * source_vertex = load_file(source_vertex_file);
    char const * source_fragment = load_file(source_fragment_file);
    GLuint programId;
    if (source_vertex && source_fragment)
    {
        programId = uGl_LoadProgram(source_vertex, source_fragment);
    } 

    if (source_vertex)
        free((void*)source_vertex);

    if (source_fragment)
        free((void*)source_fragment);

    return programId;
}

#ifdef __cplusplus
}
#endif