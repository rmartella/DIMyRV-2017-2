#version 330 core
out vec4 color;

uniform int code;

void main()
{
    color = vec4(code/255.0);
    //color = vec4(1, 1, 1, 1);
}