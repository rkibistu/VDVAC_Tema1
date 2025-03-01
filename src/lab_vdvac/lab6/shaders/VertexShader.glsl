#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_position_d;
layout(location = 4) in vec3 v_normal_d;
layout(location = 5) in vec2 v_texture_coord_d;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

layout(location = 0) out vec2 tex_coord;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;

void main(){

	// TODO calculeaza textura prin interpolare 
	vec2 tex = mix(v_texture_coord, v_texture_coord_d, time/250.0);
	tex_coord = tex;

	//TODO calculeaza pozitia prin interpolare
	vec3 pos = mix(v_position, v_position_d, time/250.0f);
	world_position = ( Model * vec4(pos,1) ).xyz;
	

	//TODO calculeaza normala prin interpolare
	vec3 normal = mix(v_normal, v_normal_d, time / 250.0);
	world_normal = normalize( mat3(Model) * normal );

	gl_Position = Projection * View * Model * vec4(pos, 1); 
}
