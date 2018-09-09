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
typedef float GLclampf;

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

#define GL_ACTIVE_TEXTURE 0x84E0

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF

#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_SAMPLER_BINDING 0x8919
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_POLYGON_MODE 0x0B40
#define GL_VIEWPORT 0x0BA2
#define GL_SCISSOR_BOX 0x0C10
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_BLEND 0x0BE2
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_SCISSOR_TEST 0x0C11
#define GL_FUNC_ADD 0x8006
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_FILL 0x1B02
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_INT 0x1405
#define GL_LINEAR 0x2601
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_ALPHA_TEST 0x0BC0


#define GL_GREATER 0x0204
#define GL_FRAMEBUFFER 0x8D40



#define GL_FRONT_AND_BACK 0x0408
#define GL_NEAREST 0x2600

#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801

#define GL_RGBA                           0x1908
#define GL_RGB 0x1907

#define GL_COLOR_ATTACHMENT0 0x8CE0


#define GL_UNSIGNED_BYTE 0x1401

#define GL_MULTISAMPLE 0x809D



#ifndef APIENTRY
    #define UGL_APIENTRY __stdcall
#else
    #define UGL_APIENTRY APIENTRY
#endif


#if __TARGET_WEB__
    #define GL_API *
#else
    #define GL_API UGL_APIENTRY *
#endif

// Opengl Functions declarations
#define UGL_HANDLE(name) _## name ## _handle

// Declare functions handles =============================
#define _TYPEDECL(x) typedef x
#define _FUNCNAME(x) (GL_API UGL_HANDLE(x))
#define _PARAMS(x) x
#include "ugl.incl"

// Declare functions storage =============================
#define _TYPEDECL(x)
#define _FUNCNAME(x) extern UGL_HANDLE(x) x
#define _PARAMS(x)
#include "ugl.incl"

// Loader
typedef void* (* uGlLoadProc)(const char *name);
uGLAPI int uGlLoadGL(uGlLoadProc proc);


// Utility functions
uGLAPI GLuint uGl_LoadShader(GLenum type, const GLchar * source);

uGLAPI GLuint uGl_LoadProgram(const GLchar * source_vertex, const GLchar * source_fragment);

uGLAPI GLuint uGl_LoadProgram_File(const char * source_vertex_file, const char * source_fragment_file);

#ifdef __cplusplus
}
#endif

#endif // _uGL_H_