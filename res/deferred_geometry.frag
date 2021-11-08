#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in float w;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;

layout(binding = 1) uniform sampler2D tex;
layout(binding = 2) uniform sampler2D tex2;

layout(location = 0) out vec4 fragAlbedo;
layout(location = 1) out vec4 fragNormal;

void main()
{
	/* vec3 albedo = texture(tex, uv).rgb + texture(tex2, uv).rgb; */
	/* albedo *= color; */
	vec3 albedo = vec3(1, 1, 1);
	fragAlbedo = vec4(albedo, 1);
	fragNormal = vec4(normalize(normal), 0);
}
