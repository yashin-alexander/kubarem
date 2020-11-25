#version 430 core

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    sampler2DShadow shadowMap;
    mat4 MVP;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec4 ShadowPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 texSample = texture(material.diffuse, TexCoords).rgb;

    // ambient
    vec3 ambient = light.ambient * texSample;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texSample;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // shadow
    vec3 normalizedShadowPos = (ShadowPos / ShadowPos.w).rgb/2.0 + 0.5;
    float bias = 0.0005*tan(acos(diff)); // Points looking perpendcular to the light direction needs larger bias
    bias = clamp(bias, 0.0, 0.001);
    normalizedShadowPos.z -= bias; // Add a little shift to avoid near zero errors
    float illumination = texture(light.shadowMap, normalizedShadowPos);

    vec3 result = ambient + (diffuse + specular) * illumination;
    FragColor = vec4(result, 1.0);
}
