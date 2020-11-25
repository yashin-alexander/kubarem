#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMVP;

void main()
{
    gl_Position = lightMVP * vec4(aPos, 1.0);
    gl_Position = gl_Position/gl_Position.w;
}
