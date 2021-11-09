#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput input_albedo;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput input_depth;
layout (input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput input_normal;

layout(binding = 3) uniform UBO {
	mat4 inverse_pv;
	vec2 windowSize;
	vec3 cameraPos;
} ubo;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

#define light_color vec3(1, 1, 1)
#define light_dir normalize(vec3(1, 1, 5))

void main()
{
	float covered = subpassLoad(input_albedo).a;
	if (covered < .5)
		discard;

	vec3 albedo = subpassLoad(input_albedo).rgb;
	float depth = subpassLoad(input_depth).r;
	vec3 normal = subpassLoad(input_normal).rgb;
	float w = subpassLoad(input_normal).w;

	vec2 screen_coord = gl_FragCoord.xy / ubo.windowSize * 2. - 1.;
	screen_coord.y = -screen_coord.y;
	vec4 pos = vec4(screen_coord, depth, w);
	pos.xyz *= pos.w;
	pos = ubo.inverse_pv * pos;

	float cosa = max(0., dot(light_dir, normal));
	vec3 diffuse = light_color * albedo.rgb * cosa;

	vec3 reflected = reflect(light_dir, normal);
	vec3 cameraDir = normalize(ubo.cameraPos - pos.xyz);
	float cosb = max(0., dot(-reflected, cameraDir));
	cosb = pow(cosb, 7.);
	vec3 spec = light_color * cosb;

	vec3 ambient = vec3(1);
	vec3 color = ambient * .1 + diffuse * .8 + spec * .3;
	fragColor = vec4(color, 1);
}
