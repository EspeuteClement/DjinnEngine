#ifndef _uGL_H_
#define _uGL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


#define uGLAPI extern




// Opengl Types declarations

typedef int GLsizei;
typedef unsigned int GLuint;
typedef int GLint;
typedef unsigned int GLenum;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;
typedef int GLsizei;
typedef void GLvoid;

// Opengl Functions declarations
#define UGL_HANDLE(name) name ## _handle
typedef void    (* UGL_HANDLE(  glGenBuffers        )) ( GLsizei n, GLuint *buffers);
typedef void    (* UGL_HANDLE(  glBindBuffer        )) ( GLenum target, GLuint buffer);
typedef void    (* UGL_HANDLE(  glBufferData        )) ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
typedef GLuint  (* UGL_HANDLE(  glCreateShader      )) ( GLenum shaderType);
typedef void    (* UGL_HANDLE(  glShaderSource      )) ( GLuint shader,GLsizei count,const GLchar **string, const GLint *length);
typedef GLuint  (* UGL_HANDLE(  glCreateProgram     )) ( void);
typedef void    (* UGL_HANDLE(  glAttachShader      )) ( GLuint program, GLuint shader);
typedef void    (* UGL_HANDLE(  glLinkProgram       )) ( GLuint program);
typedef GLint   (* UGL_HANDLE(  glGetUniformLocation)) ( GLuint program, const GLchar *name);

// Context (store it somewhere)

#define UGL_STORE(name) UGL_HANDLE(name) name
typedef struct
{
    UGL_STORE(glGenBuffers);
    UGL_STORE(glBindBuffer);
    UGL_STORE(glBufferData);
    UGL_STORE(glCreateShader);
    UGL_STORE(glShaderSource);
    UGL_STORE(glCreateProgram);
    UGL_STORE(glAttachShader);
    UGL_STORE(glLinkProgram);
    UGL_STORE(glGetUniformLocation);
} uglCtxt;

// Loader
typedef void* (* uGlLoadProc)(const char *name);
uGLAPI int uGlLoadGL(uGlLoadProc proc, uglCtxt* ctxt);


#ifdef __cplusplus
}
#endif

#endif // _uGL_H_