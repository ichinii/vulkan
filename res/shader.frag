#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 uv;

layout(binding = 1) uniform sampler2D tex;
layout(binding = 2) uniform sampler2D tex2;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = color *
		(vec4(texture(tex, uv).rgb, 1) * .2 + vec4(texture(tex2, uv).rgb, 1) * .8);
}
