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
    UGL_LOAD(glCompileShader);
    UGL_LOAD(glLinkProgram);
    UGL_LOAD(glGetUniformLocation);
    UGL_LOAD(glGetAttribLocation);
    UGL_LOAD(glEnableVertexAttribArray);
    UGL_LOAD(glDisableVertexAttribArray);
    UGL_LOAD(glVertexAttribPointer);
    UGL_LOAD(glViewport);
    UGL_LOAD(glClear);
    UGL_LOAD(glUseProgram);
    UGL_LOAD(glUniformMatrix4fv);
    UGL_LOAD(glDrawArrays);
    

    return 0;
}