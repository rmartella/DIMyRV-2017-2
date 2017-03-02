#version 330 core
// Input color from the vertex program.
in vec2 ex_texture;
out vec4 out_color;

uniform sampler2D sampler;

void main(){
    out_color = texture(sampler, ex_texture);
}