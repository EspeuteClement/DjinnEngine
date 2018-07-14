#include "ugl.h"

#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif

#define UGL_STORE(name) UGL_HANDLE(name) name;
UGL_FUNCTIONS(UGL_STORE)

#define UGL_LOAD(name) name = (UGL_HANDLE(name)) proc(#name);
int uGlLoadGL(uGlLoadProc proc)
{
    UGL_FUNCTIONS(UGL_LOAD)
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

#ifdef __cplusplus
}
#endif