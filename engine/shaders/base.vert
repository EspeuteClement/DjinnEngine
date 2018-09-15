#version 100

#ifdef GL_ES
    precision mediump float;
#endif

uniform mat4 MVP;
attribute vec3 vPos;
attribute vec4 vCol;
attribute vec2 vTexCoord;
varying vec2 texcoord;
varying vec4 color;

void main()
{
    texcoord = vTexCoord;
    gl_Position = MVP * vec4(vPos, 1.0);
    color = vCol;
}