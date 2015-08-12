#version 400 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;
uniform vec4 baseColor;

uniform int flipVertical;

void main()
{
	vec2 texCoords;

	if(flipVertical)
		texCoords = vec2(TexCoords.x, -TexCoords.y);
	else
		texCoords = TexCoords;

	vec4 result = texture(image, texCoords);

	color = result * baseColor;
}