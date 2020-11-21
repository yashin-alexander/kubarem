#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in mat4 modelViewMatrix;

uniform mat4 projectionMatrix;

void main(void){
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
}