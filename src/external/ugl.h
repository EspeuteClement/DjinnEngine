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


#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#if __TARGET_WEB__
    #define GL_API *
#else
    #define GL_API APIENTRY *
#endif

// Opengl Functions declarations
#define UGL_HANDLE(name) name ## _handle

typedef void    (GL_API UGL_HANDLE(  glGenBuffers        )) ( GLsizei n, GLuint *buffers);
typedef void    (GL_API UGL_HANDLE(  glBindBuffer        )) ( GLenum target, GLuint buffer);
typedef void    (GL_API UGL_HANDLE(  glBufferData        )) ( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
typedef GLuint  (GL_API UGL_HANDLE(  glCreateShader      )) ( GLenum shaderType);
typedef void    (GL_API UGL_HANDLE(  glShaderSource      )) ( GLuint shader,GLsizei count,const GLchar **string, const GLint *length);
typedef GLuint  (GL_API UGL_HANDLE(  glCreateProgram     )) ( void);
typedef void    (GL_API UGL_HANDLE(  glAttachShader      )) ( GLuint program, GLuint shader);
typedef void    (GL_API UGL_HANDLE(  glCompileShader     )) ( GLuint shader);
typedef void    (GL_API UGL_HANDLE(  glLinkProgram       )) ( GLuint program);
typedef GLint   (GL_API UGL_HANDLE(  glGetUniformLocation)) ( GLuint program, const GLchar *name);
typedef GLint   (GL_API UGL_HANDLE(  glGetAttribLocation )) ( GLuint program, const GLchar *name);
typedef void    (GL_API UGL_HANDLE(  glEnableVertexAttribArray))( GLuint index);
typedef void    (GL_API UGL_HANDLE(  glDisableVertexAttribArray))( GLuint index);
typedef void    (GL_API UGL_HANDLE(  glVertexAttribPointer   )) ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
typedef void    (GL_API UGL_HANDLE(  glViewport          )) ( GLint x, GLint y, GLsizei width, GLsizei height);
typedef void    (GL_API UGL_HANDLE(  glClear             )) ( GLbitfield mask);
typedef void    (GL_API UGL_HANDLE(  glUseProgram        )) ( GLuint program);
typedef void    (GL_API UGL_HANDLE(  glUniformMatrix4fv  )) ( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void    (GL_API UGL_HANDLE(  glDrawArrays        )) ( GLenum mode, GLint first, GLsizei count);

// Context (store it somewhere)

#define UGL_DECLARE(name) extern UGL_HANDLE(name) name

UGL_DECLARE(glGenBuffers);
UGL_DECLARE(glBindBuffer);
UGL_DECLARE(glBufferData);
UGL_DECLARE(glCreateShader);
UGL_DECLARE(glShaderSource);
UGL_DECLARE(glCreateProgram);
UGL_DECLARE(glAttachShader);
UGL_DECLARE(glCompileShader);
UGL_DECLARE(glLinkProgram);
UGL_DECLARE(glGetUniformLocation);
UGL_DECLARE(glGetAttribLocation);
UGL_DECLARE(glEnableVertexAttribArray);
UGL_DECLARE(glDisableVertexAttribArray);
UGL_DECLARE(glVertexAttribPointer);
UGL_DECLARE(glViewport);
UGL_DECLARE(glClear);
UGL_DECLARE(glUseProgram);
UGL_DECLARE(glUniformMatrix4fv);
UGL_DECLARE(glDrawArrays);



// Loader
typedef void* (* uGlLoadProc)(const char *name);
uGLAPI int uGlLoadGL(uGlLoadProc proc);


#ifdef __cplusplus
}
#endif

#endif // _uGL_H_