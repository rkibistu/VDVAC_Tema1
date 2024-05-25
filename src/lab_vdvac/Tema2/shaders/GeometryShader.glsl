#version 430

// Input and output topologies
layout(lines) in;
layout(line_strip, max_vertices = 256) out;

// Uniform properties
// Model is always unit matrix (we don t apply modifiers)
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int u_linesCount; // number of generated points on the curbe (smoothness)

// Input
in int instance[2];

vec3 B2(vec3 p0, vec3 p1, float t) {
	return (1 - t) * p0 + t * p1;
}

vec3 B3(vec3 p0, vec3 p1, vec3 p2, float t) {

	return (1 - t) * B2(p0, p1, t) + t * B2(p1, p2, t);
}

vec3 B4(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {

	return (1 - t) * B3(p0, p1, p2, t) + t * B3(p1, p2, p3, t);
}

void DrawOnlyTheCurve(vec3 p0, vec3 p1, vec3 p2, vec3 p3) {

	float t = 0;
	for (int i = 0; i < u_linesCount; i++) {

		vec3 p = B4(control_p0, control_p1, control_p2, control_p3, t);
		gl_Position = Projection * View * vec4(p, 1);
		EmitVertex();
		t += 1.0 / u_linesCount;
	}
	vec3 p = B4(control_p0, control_p1, control_p2, control_p3, t);
	gl_Position = Projection * View * vec4(p, 1);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	const int SURFACE_TYPE_TRANSLATION = 0;
	const int SURFACE_TYPE_ROTATION = 1;

	EndPrimitive();

	DrawOnlyTheCurve(control_p0, control_p1, control_p2, control_p3);
}
