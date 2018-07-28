#version 100

#ifdef GL_ES
    precision mediump float;
#endif

varying vec3 color;
varying vec2 texcoord;
uniform sampler2D tex;

void main()
{ 
    gl_FragColor = texture2D(tex, texcoord);
}