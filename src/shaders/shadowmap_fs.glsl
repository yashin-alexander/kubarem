#version 430 core

out float FragDepth;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    sampler2DShadow shadowMap;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{
    FragDepth = gl_FragCoord.z;
}
