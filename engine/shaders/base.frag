#version 100

#ifdef GL_ES
    precision mediump float;
#endif

varying vec4 color;
varying vec2 texcoord;
uniform sampler2D tex;

void main()
{ 
    gl_FragColor = color;// * texture2D(tex, texcoord);
}