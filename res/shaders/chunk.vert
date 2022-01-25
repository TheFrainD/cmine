#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in float a_light;

out vec3 v_pos;
out vec2 v_uv;
out float v_light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * model * vec4(a_pos, 1.0);
	v_uv = a_uv;
	v_pos = a_pos;
	v_light = a_light;
}
