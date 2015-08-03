#version 400 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

uniform float gamma;

void main()
{   
	vec3 diffuse = texture(texture_diffuse1, TexCoords).rgb;
	diffuse = pow(diffuse, vec3(1.0f/gamma));

    color = vec4(diffuse, 1.0f);
}