#version 400 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;
uniform vec4 baseColor;

void main()
{
	vec4 result = texture(image, TexCoords);

	color = result * baseColor;
}