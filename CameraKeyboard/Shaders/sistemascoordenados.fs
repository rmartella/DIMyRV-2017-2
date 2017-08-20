#version 330 core
in vec2 ex_texCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	color = mix(texture(ourTexture1, ex_texCoord), texture(ourTexture2, ex_texCoord), 0.2);
}