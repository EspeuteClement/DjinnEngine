#version 100

#ifdef GL_ES
    precision mediump float;
#endif

out vec3 color;
void main(){
  color = vec3(1,0,0);
}