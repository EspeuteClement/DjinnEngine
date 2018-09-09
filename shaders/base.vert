#version 100

#ifdef GL_ES
    precision mediump float;
#endif

in vec3 pos;

void main()
{
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;
}

