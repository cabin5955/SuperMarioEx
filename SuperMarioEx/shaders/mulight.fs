#version 300 es
precision mediump float;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirectLight {
    vec3 position;
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

#define NR_POINT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

layout(location = 0) out vec4 FragColor;

uniform Material material;
uniform DirectLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 viewPos;
uniform vec4 meshColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 result = vec3(0.0f);
    vec3 ambient =  vec3(texture(material.diffuse, TexCoords)) * dirLight.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(dirLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, TexCoords));
    
    result = ambient + diffuse + specular;
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    
    FragColor = meshColor*vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
