#version 460
out vec4 FragColor;

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    Light color;
};  

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    Light color;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;

    Light color;     
};

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoords;

uniform vec3 cameraPosition;

#define MAX_DIRECTIONAL_LIGHT_COUNT 4
uniform int directionalLightCount;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];

#define MAX_POINT_LIGHT_COUNT 10
uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];

#define MAX_SPOT_LIGHT_COUNT 10
uniform int spotLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];

uniform Material material;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(cameraPosition - vertexPosition);

    vec3 result = vec3(0);
    for (int i = 0; i < directionalLightCount; i++)
        result += CalcDirectionalLight(directionalLights[i], norm, viewDir);
    for (int i = 0; i < pointLightCount; i++)
        result += CalcPointLight(pointLights[i], norm, vertexPosition, viewDir);
    for (int i = 0; i < spotLightCount; i++)
        result += CalcSpotLight(spotLights[i], norm, vertexPosition, viewDir);
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDirection = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);

    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, textureCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, textureCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, textureCoords));

    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist +
  			        light.quadratic * (dist * dist));
                 
    // Combine results
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, textureCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, textureCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, textureCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Combine results
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, textureCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, textureCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, textureCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
