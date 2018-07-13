#include "ugl.h"

#define UGL_STORE(name) UGL_HANDLE(name) name

UGL_STORE(glGenBuffers);
UGL_STORE(glBindBuffer);
UGL_STORE(glBufferData);
UGL_STORE(glCreateShader);
UGL_STORE(glShaderSource);
UGL_STORE(glCreateProgram);
UGL_STORE(glAttachShader);
UGL_STORE(glCompileShader);
UGL_STORE(glLinkProgram);
UGL_STORE(glGetUniformLocation);
UGL_STORE(glGetAttribLocation);
UGL_STORE(glEnableVertexAttribArray);
UGL_STORE(glDisableVertexAttribArray);
UGL_STORE(glVertexAttribPointer);
UGL_STORE(glViewport);
UGL_STORE(glClear);
UGL_STORE(glUseProgram);
UGL_STORE(glUniformMatrix4fv);
UGL_STORE(glDrawArrays);

#define UGL_LOAD(name) name = (UGL_HANDLE(name)) proc(#name)
int uGlLoadGL(uGlLoadProc proc)
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