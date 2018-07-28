#version 100

#ifdef GL_ES
    precision mediump float;
#endif

uniform mat4 MVP;
attribute vec3 vCol;
attribute vec2 vPos;
attribute vec2 vTexCoord;
varying vec2 texcoord;
varying vec3 color;

void main()
{
    texcoord = vTexCoord;
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
}

