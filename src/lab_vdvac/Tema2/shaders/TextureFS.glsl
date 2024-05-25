#version 430

layout(location = 0) out vec4 out_color;

uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;

layout(location = 0) in vec3 out_pos;

void main() {

	float intensity;
	float alpha;
	vec4 tex;

	intensity = texture(VolumeTex, out_pos).x;
	tex = texture(TransferFunc, intensity);


	//tex.a = 1.0 - pow(1.0 - intensity.x, 0.001 * 200.0f);
	//tex.rgb *= tex.a;
	//out_color = tex;


	alpha = intensity.x * 0.1;
	out_color = vec4(intensity.x * alpha, intensity * alpha, intensity * alpha, alpha);
}