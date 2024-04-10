#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

uniform vec3 cameraPos;
uniform vec3 proxyPointPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {



	//just a test to see the proxyPointPos
	//gl_Position = Projection * View * Model * (vec4(proxyPointPos,1) + vec4(-1,0,0,0));
	//EmitVertex();
	//gl_Position = Projection * View * Model *  (vec4(proxyPointPos,1) + vec4(1, 0, 0, 0));
	//EmitVertex();
	//gl_Position = Projection * View * Model *  (vec4(proxyPointPos,1) + vec4(0, 1, 0, 0));
	//EmitVertex();

	vec3 n = normalize(proxyPointPos - cameraPos);
	vec3 p0 = (Model * vec4(proxyPointPos, 1)).xyz;

	vec3 c0 = (Model * vec4(0, 0, 0, 1)).xyz;
	vec3 c3 = (Model * vec4(0, 1, 0, 1)).xyz;
	vec3 l = normalize(c3 - c0);
	vec3 l0 = c0;

	//l
	//l0

	//conditie: 0 <= d <= 1 daca l e pozitiv
	//          -1 <= d <= 0 daca l e negativ

	float d;
	float divizor = dot(l, n);
	if (divizor != 0) {

		d = (dot((p0 - l0), n)) / divizor;
	}
	vec3 intersectionPoint = l0 + l * d;

	//if ((intersectionPoint.z < c0.z || intersectionPoint.z > c3.z) ||
	//	(intersectionPoint.x < c0.x || intersectionPoint.x > c3.x) ||
	//	(intersectionPoint.y < c0.y || intersectionPoint.y > c3.y)) {

	//	intersectionPoint.z = 0;
	//	intersectionPoint.x = 0;
	//	intersectionPoint.y = 0;
	//}

	if (d < 0 || d > 1) {
		intersectionPoint.z = 0;
		intersectionPoint.x = 0;
		intersectionPoint.y = 0;
	}

	gl_Position = Projection * View * (vec4(intersectionPoint, 1));
	EmitVertex();
	gl_Position = Projection * View * Model * vec4(0, 0, 0, 1);
	EmitVertex();
	gl_Position = Projection * View * Model * vec4(3, 0, 0, 1);
	EmitVertex();
	EndPrimitive();




	//second
	/*c0 = (Model * vec4(1, 0, 0, 1)).xyz;
	c3 = (Model * vec4(1, 0, 1, 1)).xyz;
	l = normalize(c0 - c3);
	l0 = c0;

	divizor = dot(l, n);
	if (divizor != 0) {

		d = (dot((p0 - l0), n)) / divizor;
	}

	intersectionPoint = l0 + l * d;
	if (intersectionPoint.z < c0.z || intersectionPoint.z > c3.z)
		intersectionPoint.z = 0;
	gl_Position = Projection * View * (vec4(intersectionPoint, 1));
	EmitVertex();
	gl_Position = Projection * View * Model * vec4(1, 0, 0, 1);
	EmitVertex();
	gl_Position = Projection * View * Model * vec4(0, 3, 0, 1);
	EmitVertex();
	EndPrimitive();*/
}
