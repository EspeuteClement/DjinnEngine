#ifndef _uGL_H_
#define _uGL_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


#define uGLAPI extern




// Opengl Types declarations

typedef int32_t GLsizei;
typedef uint32_t GLuint;
typedef int32_t GLint;
typedef uint32_t GLenum;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;
typedef void GLvoid;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef float GLfloat;

// Opengl Constants

#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004


// Opengl Functions declarations
#define UGL_HANDLE(name) name ## _handle
#define WIN_API_CALL __stdcall *
typedef void    (WIN_API_CALL UGL_HANDLE(  glGenBuffers        )) ( GLsizei n, GLuint *buffers);
typedef void    (WIN_API_CALL UGL_HANDLE(  glBindBuffer        )) ( GLenum target, GLuint buffer);
typedef void    (WIN_API_CALL UGL_HANDLE(  glBufferData        )) ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
typedef GLuint  (WIN_API_CALL UGL_HANDLE(  glCreateShader      )) ( GLenum shaderType);
typedef void    (WIN_API_CALL UGL_HANDLE(  glShaderSource      )) ( GLuint shader,GLsizei count,const GLchar **string, const GLint *length);
typedef GLuint  (WIN_API_CALL UGL_HANDLE(  glCreateProgram     )) ( void);
typedef void    (WIN_API_CALL UGL_HANDLE(  glAttachShader      )) ( GLuint program, GLuint shader);
typedef void    (WIN_API_CALL UGL_HANDLE(  glCompileShader     )) ( GLuint shader);
typedef void    (WIN_API_CALL UGL_HANDLE(  glLinkProgram       )) ( GLuint program);
typedef GLint   (WIN_API_CALL UGL_HANDLE(  glGetUniformLocation)) ( GLuint program, const GLchar *name);
typedef GLint   (WIN_API_CALL UGL_HANDLE(  glGetAttribLocation )) ( GLuint program, const GLchar *name);
typedef void    (WIN_API_CALL UGL_HANDLE(  glEnableVertexAttribArray))( GLuint index);
typedef void    (WIN_API_CALL UGL_HANDLE(  glDisableVertexAttribArray))( GLuint index);
typedef void    (WIN_API_CALL UGL_HANDLE(  glVertexAttribPointer   )) ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
typedef void    (WIN_API_CALL UGL_HANDLE(  glViewport          )) ( GLint x, GLint y, GLsizei width, GLsizei height);
typedef void    (WIN_API_CALL UGL_HANDLE(  glClear             )) ( GLbitfield mask);
typedef void    (WIN_API_CALL UGL_HANDLE(  glUseProgram        )) ( GLuint program);
typedef void    (WIN_API_CALL UGL_HANDLE(  glUniformMatrix4fv  )) ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void    (WIN_API_CALL UGL_HANDLE(  glDrawArrays        )) ( GLenum mode, GLint first, GLsizei count);

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

} uglCtxt;

// Loader
typedef void* (* uGlLoadProc)(const char *name);
uGLAPI int uGlLoadGL(uGlLoadProc proc, uglCtxt* ctxt);


#ifdef __cplusplus
}
#endif

#endif // _uGL_H_