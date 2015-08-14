#version 400 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_shadowMap1;
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

in vec3 fragPosition;
in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPosLightSpace;

out vec4 color;

uniform vec3 viewPos;

uniform DirectionalLight directionalLight;

uniform int countPointLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int countSpotLight;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;

float ShadowCalculation(vec4 fragPosLightSpace, DirectionalLight light)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   
    projCoords = projCoords * 0.5f + 0.5f;

    float closestDepth = texture(material.texture_shadowMap1, projCoords.xy).r; 

    float currentDepth = projCoords.z;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.direction - fragPosition);
    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);

    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(material.texture_shadowMap1, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(material.texture_shadowMap1, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0f : 0.0f;        
        }    
    }
    shadow /= 9.0f;
  
    if(projCoords.z > 1.0f)
        shadow = 0.0f;

    return shadow;
}

vec3 CalcDirectionalLight(DirectionalLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    // Specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), mat.shininess);

    // Combine results
    vec3 ambient = light.ambient * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));

    float shadow = ShadowCalculation(FragPosLightSpace, light); 
    
    return ((ambient + (1.0f - shadow)) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    // Specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), mat.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // Combine results
    vec3 ambient = light.ambient * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, Material mat, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);

    // Specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), mat.shininess);

    //Spot Light
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    // Combine results
    vec3 ambient = light.ambient * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));

    diffuse *= intensity;
    specular *= intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

void main()
{    
    int i;
    vec3 result;
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 norm = normalize(Normal);

    result += CalcDirectionalLight(directionalLight, material, norm, fragPosition, viewDir);

    int pC = countPointLight;
    int sC = countSpotLight;

    if(pC > MAX_POINT_LIGHTS)
        pC = MAX_POINT_LIGHTS;

    if(sC > MAX_SPOT_LIGHTS)
        sC = MAX_SPOT_LIGHTS;

    for(i = 0; i < pC; i++)
        result += CalcPointLight(pointLights[i], material, norm, fragPosition, viewDir);

    for(i = 0; i < sC; i++)
        result += CalcSpotLight(spotLights[i], material, norm, fragPosition, viewDir);
        
    color = vec4(result, 1.0f);
}