#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 30) out;

uniform vec3 cameraPos;
uniform vec3 proxyPointPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 fragColor;

struct edge {
	vec3 c0;
	vec3 c1;
};


//laturile cubului cu care verificam intersectia
edge edges[12];
//lista punctelor de intersectie
vec3 intersectionPoints[12];
//numarul punctelor de intersectie
int intersectionPointsCount;
//centrul punctelor de intersectie
vec3 center;
//unghiurile dupa care sortam
float angles[12];

void createEdges() {
	//0-3
	edges[0].c0 = vec3(0, 0, 0);
	edges[0].c1 = vec3(0, 0, 1);
	//1-2
	edges[1].c0 = vec3(1, 0, 0);
	edges[1].c1 = vec3(1, 0, 1);
	//4-7
	edges[2].c0 = vec3(0, 1, 0);
	edges[2].c1 = vec3(0, 1, 1);
	//5-6
	edges[3].c0 = vec3(1, 1, 0);
	edges[3].c1 = vec3(1, 1, 1);
	//0-1
	edges[4].c0 = vec3(0, 0, 0);
	edges[4].c1 = vec3(1, 0, 0);
	//4-5
	edges[5].c0 = vec3(0, 1, 0);
	edges[5].c1 = vec3(1, 1, 0);
	//7-6
	edges[6].c0 = vec3(0, 1, 1);
	edges[6].c1 = vec3(1, 1, 1);
	//3-2
	edges[7].c0 = vec3(0, 0, 1);
	edges[7].c1 = vec3(1, 0, 1);
	//0-4
	edges[8].c0 = vec3(0, 0, 0);
	edges[8].c1 = vec3(0, 1, 0);
	//1-5
	edges[9].c0 = vec3(1, 0, 0);
	edges[9].c1 = vec3(1, 1, 0);
	//2-6
	edges[10].c0 = vec3(1, 0, 1);
	edges[10].c1 = vec3(1, 1, 1);
	//3-7
	edges[11].c0 = vec3(0, 0, 1);
	edges[11].c1 = vec3(0, 1, 1);
}

void calculateIntersectionPoints(vec3 n, vec3 p0) {

	intersectionPointsCount = 0;
	for (int i = 0; i < 12; i++) {

		vec3 c0 = edges[i].c0;
		vec3 c3 = edges[i].c1;
		vec3 l = normalize(c0 - c3);
		vec3 l0 = c3; //mereu sa fie egala cu valoarea din dreapta calcului lui l

		//calculare distanta punctului de intersectie fata de l0 pe latura cubului
		float d;
		float divizor = dot(l, n);
		if (divizor != 0) {

			d = (dot((p0 - l0), n)) / divizor;
		}
		//calculare punct intersectie
		vec3 intersectionPoint = l0 + l * d;

		//punctul este in interiorul cubului
		if (d >= 0 && d <= 1) {
			intersectionPoints[intersectionPointsCount++] = intersectionPoint;
		}

	}
}

//calculeaza centrul punctelor de itnersectie
vec3 calculateCenter() {

	center = vec3(0, 0, 0);
	for (int i = 0; i < 12; i++) {
		if (i >= intersectionPointsCount)
			break;

		center.x += intersectionPoints[i].x;
		center.y += intersectionPoints[i].y;
		center.z += intersectionPoints[i].z;
	}
	center /= intersectionPointsCount;
	return center;
}
void movePoints(vec3 amount) {

	for (int i = 0; i < 12; i++) {
		if (i >= intersectionPointsCount)
			break;

		intersectionPoints[i] += amount;
	}
}

void calculateAngles(vec3 n) {

	//calculeaza unghiul dintre vectorul [primulPunctDeIntersectie,centru]
	// si restul vectorilor formati de punctele de intersectie si centru
	vec3 reference = normalize(center - intersectionPoints[0]);
	angles[0] = 1;
	for (int i = 1; i < 12; i++) {
		if (i >= intersectionPointsCount)
			break;

		vec3 temp = normalize(center - intersectionPoints[i]);

		float dotValue = dot(reference, temp);

		angles[i] = dotValue;

		
		vec3 crossVec = cross(reference, temp);
		if (dot(n, crossVec) < 0) {
			if (angles[i] > 0) {

				angles[i] *= -1;
			}
			angles[i] -= 100;
		}
	}
}

float getAngle(vec2 center, vec2 pt) {

	float x = pt.x - center.x;
	float y = pt.y - center.y;
	float angle = atan(y, x);
	if (angle < 0) {
		angle += 2 * 3.1415926535897932384626433832795;
	}

	return angle;
}

void calculateAngles2(vec3 n) {

	//calculeaza unghiul dintre vectorul [primulPunctDeIntersectie,centru]
	// si restul vectorilor formati de punctele de intersectie si centru
	for (int i = 0; i < 12; i++) {
		if (i >= intersectionPointsCount)
			break;

		angles[i] = getAngle(vec2(0, 0), vec2(intersectionPoints[i].x, intersectionPoints[i].y));
	}
}

//sorteaza punctele dupa unghi
void sortPoints() {
	for (int i = 0; i < 12 - 1; i++) {
		if (i >= intersectionPointsCount - 1)
			break;
		for (int j = i + 1; j < 12; j++) {
			if (j >= intersectionPointsCount)
				continue;

			//do sorting
			if (angles[i] > angles[j]) {
				vec3 tempVec = intersectionPoints[i];
				intersectionPoints[i] = intersectionPoints[j];
				intersectionPoints[j] = tempVec;

				float temp = angles[i];
				angles[i] = angles[j];
				angles[j] = temp;
			}
		}
	}
}

void renderPlane() {

	if (intersectionPointsCount < 3)
		return;

	int i = 0;
	for (i ; i < 12; i++) {

		if (i >= intersectionPointsCount - 1)
			break;

		gl_Position = Projection * View * (vec4(intersectionPoints[i], 1));
		EmitVertex();
		gl_Position = Projection * View * (vec4(intersectionPoints[i + 1], 1));
		EmitVertex();
		gl_Position = Projection * View * (vec4(center, 1));
		EmitVertex();
		fragColor = intersectionPoints[i];
		EndPrimitive();
	}

	fragColor = intersectionPoints[0];
	gl_Position = Projection * View * (vec4(intersectionPoints[i], 1));
	EmitVertex();
	gl_Position = Projection * View * (vec4(intersectionPoints[0], 1));
	EmitVertex();
	gl_Position = Projection * View * (vec4(center, 1));
	EmitVertex();
	EndPrimitive();
}

void main() {

	fragColor = vec3(1,0,0);

	createEdges();

	vec3 n = normalize(proxyPointPos - cameraPos);
	vec3 p0 = (Model * vec4(proxyPointPos, 1)).xyz;

	calculateIntersectionPoints(n,p0);

	calculateCenter();
	movePoints(-center);

	calculateAngles2(n);
	sortPoints();

	movePoints(center);

	renderPlane();
}
