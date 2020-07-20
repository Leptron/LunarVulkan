#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 proj;
} ubo;

layout(location = 0) out vec2 uvs;

out	gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = ubo.proj * ubo.model * vec4(inPos, 0.0, 1.0);
	uvs = inUV;
}