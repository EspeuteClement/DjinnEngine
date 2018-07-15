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

#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1

#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001

#define GL_NEAREST 0x2600

#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801

#define GL_RGBA                           0x1908
#define GL_UNSIGNED_BYTE 0x1401

#define GL_MULTISAMPLE 0x809D



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

typedef void    (GL_API UGL_HANDLE( glGenBuffers                ))  (    GLsizei n, GLuint *buffers);
typedef void    (GL_API UGL_HANDLE( glBindBuffer                ))  (    GLenum target, GLuint buffer);
typedef void    (GL_API UGL_HANDLE( glBufferData                ))  (    GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
typedef GLuint  (GL_API UGL_HANDLE( glCreateShader              ))  (    GLenum shaderType);
typedef void    (GL_API UGL_HANDLE( glShaderSource              ))  (    GLuint shader,GLsizei count,const GLchar **string, const GLint *length);
typedef GLuint  (GL_API UGL_HANDLE( glCreateProgram             ))  (    void);
typedef void    (GL_API UGL_HANDLE( glAttachShader              ))  (    GLuint program, GLuint shader);
typedef void    (GL_API UGL_HANDLE( glCompileShader             ))  (    GLuint shader);
typedef void    (GL_API UGL_HANDLE( glLinkProgram               ))  (    GLuint program);
typedef GLint   (GL_API UGL_HANDLE( glGetUniformLocation        ))  (    GLuint program, const GLchar *name);
typedef GLint   (GL_API UGL_HANDLE( glGetAttribLocation         ))  (    GLuint program, const GLchar *name);
typedef void    (GL_API UGL_HANDLE( glEnableVertexAttribArray   ))  (    GLuint index);
typedef void    (GL_API UGL_HANDLE( glDisableVertexAttribArray  ))  (    GLuint index);
typedef void    (GL_API UGL_HANDLE( glVertexAttribPointer       ))  (    GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
typedef void    (GL_API UGL_HANDLE( glViewport                  ))  (    GLint x, GLint y, GLsizei width, GLsizei height);
typedef void    (GL_API UGL_HANDLE( glClear                     ))  (    GLbitfield mask);
typedef void    (GL_API UGL_HANDLE( glUseProgram                ))  (    GLuint program);
typedef void    (GL_API UGL_HANDLE( glUniformMatrix4fv          ))  (    GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void    (GL_API UGL_HANDLE( glDrawArrays                ))  (    GLenum mode, GLint first, GLsizei count);
typedef void    (GL_API UGL_HANDLE( glGetShaderiv               ))  (    GLuint shader,  GLenum pname,  GLint *params);
typedef void    (GL_API UGL_HANDLE( glGetShaderInfoLog          ))  ( 	GLuint shader,GLsizei maxLength,GLsizei *length,GLchar *infoLog);
typedef void    (GL_API UGL_HANDLE( glDeleteShader              ))  (   GLuint shader);
typedef void    (GL_API UGL_HANDLE( glGenTextures               ))  (GLsizei n,  GLuint * textures);
typedef void    (GL_API UGL_HANDLE( glBindTexture               ))  (GLenum target,  GLuint texture);
typedef void    (GL_API UGL_HANDLE( glTexParameteri             ))  (GLenum target,  GLenum pname,  GLint param);
typedef void    (GL_API UGL_HANDLE( glTexImage2D                ))  (GLenum target,  GLint level,  GLint internalformat,  GLsizei width,  GLsizei height,  GLint border,  GLenum format,  GLenum type,  const GLvoid * data);
typedef void    (GL_API UGL_HANDLE( glEnable                    ))  (GLenum  cap);
typedef void    (GL_API UGL_HANDLE( glDisable                    )) (GLenum  cap);

// Used to get all the functions using various macros
#define UGL_FUNCTIONS(OP) \
        OP(glGenBuffers                 )\
        OP(glBindBuffer                 )\
        OP(glBufferData                 )\
        OP(glCreateShader               )\
        OP(glShaderSource               )\
        OP(glCreateProgram              )\
        OP(glAttachShader               )\
        OP(glCompileShader              )\
        OP(glLinkProgram                )\
        OP(glGetUniformLocation         )\
        OP(glGetAttribLocation          )\
        OP(glEnableVertexAttribArray    )\
        OP(glDisableVertexAttribArray   )\
        OP(glVertexAttribPointer        )\
        OP(glViewport                   )\
        OP(glClear                      )\
        OP(glUseProgram                 )\
        OP(glUniformMatrix4fv           )\
        OP(glDrawArrays                 )\
        OP(glGetShaderiv                )\
        OP(glGetShaderInfoLog           )\
        OP(glDeleteShader               )\
        OP(glGenTextures                )\
        OP(glBindTexture                )\
        OP(glTexParameteri              )\
        OP(glTexImage2D                 )\
        OP(glEnable                     )\
        OP(glDisable                    )\


// Context (store it somewhere)

#define UGL_DECLARE(name) extern UGL_HANDLE(name) name;
UGL_FUNCTIONS(UGL_DECLARE)



// Loader
typedef void* (* uGlLoadProc)(const char *name);
uGLAPI int uGlLoadGL(uGlLoadProc proc);


// Utility functions
uGLAPI GLuint uGl_LoadShader(GLenum type, const GLchar * source);

uGLAPI GLuint uGl_LoadProgram(const GLchar * source_vertex, const GLchar * source_fragment);

#ifdef __cplusplus
}
#endif

#endif // _uGL_H_