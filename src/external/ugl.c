#include "ugl.h"

#define UGL_LOAD(name) ctxt-> ## name = (UGL_HANDLE(name)) proc(#name)
int uGlLoadGL(uGlLoadProc proc, uglCtxt* ctxt)
{
    UGL_LOAD(glGenBuffers);
    UGL_LOAD(glBindBuffer);
    UGL_LOAD(glBufferData);
    UGL_LOAD(glCreateShader);
    UGL_LOAD(glShaderSource);
    UGL_LOAD(glCreateProgram);
    UGL_LOAD(glAttachShader);
    UGL_LOAD(glLinkProgram);
    UGL_LOAD(glGetUniformLocation);
    return 0;
}