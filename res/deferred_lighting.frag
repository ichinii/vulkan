#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput input_albedo;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput input_depth;
layout (input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput input_normal;

layout(binding = 3) uniform UBO {
	mat4 p;
	vec2 windowSize;
} ubo;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

void main()
{
	vec4 albedo_ = subpassLoad(input_albedo);
	vec3 albedo = albedo_.rgb;
	float w = albedo_.w;
	float depth = subpassLoad(input_depth).r;
	vec3 normal = subpassLoad(input_normal).rgb;

	vec2 screen_coord = gl_FragCoord.xy / ubo.windowSize * 2. - 1.;
	screen_coord.y = -screen_coord.y;
	vec4 pos = vec4(screen_coord, depth, w);
	pos.xyz *= pos.w;
	pos = ubo.p * pos;
	fragColor = vec4(normal, 1);
}
