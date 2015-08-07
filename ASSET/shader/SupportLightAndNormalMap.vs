#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 fragPosition;
out vec3 Normal;
out vec3 toCameraVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPosition = vec3(model * vec4(position, 1.0f));
    //Normal = mat3(transpose(inverse(model))) * normal;
    Normal = (model * vec4(normal, 0.0)).xyz;
    TexCoords = texCoords;

    toCameraVector = (inverse(view) * vec4(0, 0, 0, 1)).xyz - vec3(model * vec4(position, 1.0f)).xyz;
}