#version 330 core

in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D Texture;

void main()
{
	frag_color = texture(Texture,TexCoord);
}