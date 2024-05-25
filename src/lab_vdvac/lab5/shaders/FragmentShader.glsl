#version 430
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;
uniform vec3 color;

layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

void main(){
	vec3 L = normalize ( light_position - world_position);
	vec3 V = normalize ( eye_position - world_position);

	float ambient_light = 0.3;
	float diffuse_light = max(dot(L,world_normal),0);
	float specular_light =0;
	if(diffuse_light>0){
		specular_light = pow(max(dot(world_normal,normalize(L+V)),0), material_shininess);		//Blinn - Phong
	}

	float light = material_kd*(ambient_light + diffuse_light) + material_ks*specular_light;


	out_color = vec4(color * vec3(light), 1);
}