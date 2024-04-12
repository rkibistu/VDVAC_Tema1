#version 430

layout(location = 0) out vec4 out_color;

uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;

layout(location = 0) in vec3 out_pos;

void main() {

	float intensity;
	vec4 color;
	intensity = texture(VolumeTex, out_pos).x;
	color = texture(TransferFunc, intensity);
	out_color = color;

}