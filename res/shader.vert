#version 450 core
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
	vec4 gl_Position;
};

layout(location = 0) out vec4 color;

vec2 positions[3] = vec2[] (
	vec2(-.5, .5),
	vec2(.5, .5),
	vec2(0, -.5)
);

vec3 colors[3] = vec3[] (
	vec3(2, 0, 0),
	vec3(0, 2, 0),
	vec3(0, 0, 2)
);

void main()
{
	gl_Position = vec4(positions[gl_VertexIndex], .5, 1);
	color = vec4(colors[gl_VertexIndex], 1);
}
