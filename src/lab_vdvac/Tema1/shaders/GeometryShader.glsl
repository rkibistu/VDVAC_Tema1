#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

uniform vec3 cameraPos;
uniform vec3 proxyPointPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

struct edge{
	vec3 c0;
	vec3 c1;
};

//laturile cubului cu care verificam intersectia
edge edges[12];
//lista punctelor de intersectie
vec3 intersectionPoints[12];
//numarul punctelor de intersectie
int intersectionPointsCount;

void createEdges(){
	//0-3
	edges[0].c0 =  vec3(0, 0, 0);
	edges[0].c1 =  vec3(0, 0, 1);
	//1-2
	edges[1].c0 =  vec3(1, 0, 0);
	edges[1].c1 =  vec3(1, 0, 1);
	//4-7
	edges[2].c0 =  vec3(0, 1, 0);
	edges[2].c1 =  vec3(0, 1, 1);
	//5-6
	edges[3].c0 =  vec3(1, 1, 0);
	edges[3].c1 =  vec3(1, 1, 1);
	//0-1
	edges[4].c0 =  vec3(0, 0, 0);
	edges[4].c1 =  vec3(1, 0, 0);
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
void main() {

	createEdges();
	
	vec3 n = normalize(proxyPointPos - cameraPos);
	vec3 p0 = (Model * vec4(proxyPointPos, 1)).xyz;

	intersectionPointsCount = 0;
	for(int i=0;i<12;i++){
	
		vec3 c0 = edges[i].c0;
		vec3 c3 = edges[i].c1;
		vec3 l = normalize(c0 - c3);
		vec3 l0 = c3; //mereu sa fie egala cu valoarea din dreapta cand calculezi l

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

	if(intersectionPointsCount > 2){
		gl_Position = Projection * View * (vec4(intersectionPoints[intersectionPointsCount-1], 1));
		EmitVertex();
		gl_Position = Projection * View  * (vec4(intersectionPoints[intersectionPointsCount-2], 1));
		EmitVertex();
		gl_Position = Projection * View  * (vec4(intersectionPoints[intersectionPointsCount-3], 1));
		EmitVertex();
		EndPrimitive();
	}
	
}
