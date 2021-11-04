#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput albedo;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput depth;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

void main()
{
	/* fragColor = vec4(subpassLoad(albedo).xy, subpassLoad(depth).r, 1); */
	fragColor = vec4(vec2(0), 1 - pow(subpassLoad(depth).r, 50), 1);
}
