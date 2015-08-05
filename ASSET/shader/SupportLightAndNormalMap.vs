#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoords;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat3 TBN;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPosition = vec3(model * vec4(position, 1.0f));
    TexCoords = texCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);

    /*vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));

    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(T, N);*/
    
    TBN = transpose(mat3(T, B, N));  
}