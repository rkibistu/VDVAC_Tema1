#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model, View, Projection;

layout(location = 0) out vec3 world_position;
layout(location = 1) out vec3 world_normal;

void main(){

	//calculeaza vectori in WORLD SPACE
	world_position = (Model * vec4(v_position,1)).xyz;
	world_normal = normalize( mat3(Model) * v_normal );

	
	gl_Position = Projection*View*Model*vec4(v_position,1); 
}
