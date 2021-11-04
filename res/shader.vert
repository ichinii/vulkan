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

layout(location = 0) out vec3 pos;
layout(location = 1) out vec4 color;
layout(location = 2) out vec2 uv;

void main()
{
	vec4 pos4 = ubo.mvp * vec4(vertex_position, 1);
	pos4.y = -pos4.y;
	pos = pos4.xyz;
	gl_Position = pos4;

	color = vec4(vertex_color, 1);
	uv = vertex_uv;
}
