#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

layout(binding = 0) uniform UBO {
	mat4 mvp;
} ubo;

out gl_PerVertex {
	vec4 gl_Position;
};

layout(location = 0) out vec4 color;

void main()
{
	gl_Position = ubo.mvp * vec4(vertex_position, 1);
	color = vec4(vertex_color, 1);
}
