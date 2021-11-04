#version 450 core
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
	vec4 gl_Position;
};

layout(location = 0) out vec2 uv;

void main()
{
	int i = gl_VertexIndex;
	uv = vec2(i % 2, i / 2);
	uv = uv * 2. - 1.;
	uv.y = -uv.y;

	gl_Position = vec4(uv, 0, 1);
}
