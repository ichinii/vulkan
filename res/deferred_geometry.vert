#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

layout(binding = 0) uniform UBO {
	mat4 mvp;
} ubo;

out gl_PerVertex {
	vec4 gl_Position;
};

layout(location = 0) out float w;
layout(location = 1) out vec3 color;
layout(location = 2) out vec2 uv;

void main()
{
	vec4 pos = ubo.mvp * vec4(vertex_position, 1);
	pos.y = -pos.y;
	gl_Position = pos;
	w = pos.w;

	color = vertex_color;
	uv = vertex_uv;
}
