#version 400 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
    
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 128
#define MAX_SPOT_LIGHTS 128

in vec2 TexCoords;
in vec3 fragPosition;

in mat3 TBN;
vec3 TangentViewPos;
vec3 TangentFragPos;

out vec4 color;

uniform vec3 viewPos;

uniform DirectionalLight directionalLight;

uniform int countPointLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int countSpotLight;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;

uniform float gamma;

vec3 CalcDirectionalLight(DirectionalLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(TBN*-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);

    // Combine results
    vec3 ambient = light.ambient * texture(mat.texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(mat.texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(mat.texture_specular1, TexCoords).rgb;
    
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize((TBN*light.position) - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);

    // Attenuation
    float distance = length((TBN*light.position) - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // Combine results
    vec3 ambient = light.ambient * texture(mat.texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(mat.texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(mat.texture_specular1, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize((TBN*light.position) - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);

    //Spot Light
    float theta = dot(lightDir, normalize(TBN*-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    // Attenuation
    float distance = length((TBN*light.position) - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // Combine results
    vec3 ambient = light.ambient * texture(mat.texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(mat.texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(mat.texture_specular1, TexCoords).rgb;

    diffuse *= intensity;
    specular *= intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

void main()
{    
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * fragPosition;

    int i;
    vec3 result;
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 norm = texture(material.texture_normal1, TexCoords).rgb;
    norm = normalize(norm * 2.0f - 1.0f);
    //norm = normalize(TBN * norm);

    result += CalcDirectionalLight(directionalLight, material, norm, TangentFragPos, viewDir);

    int pC = countPointLight;
    int sC = countSpotLight;

    if(pC > MAX_POINT_LIGHTS)
        pC = MAX_POINT_LIGHTS;

    if(sC > MAX_SPOT_LIGHTS)
        sC = MAX_SPOT_LIGHTS;

    for(i = 0; i < pC; i++)
        result += CalcPointLight(pointLights[i], material, norm, TangentFragPos, viewDir);

    for(i = 0; i < sC; i++)
        result += CalcSpotLight(spotLights[i], material, norm, TangentFragPos, viewDir);

    result = pow(result, vec3(1.0f/gamma));
        
    color = vec4(result, 1.0f);
}