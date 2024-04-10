#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniforme
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int is_image;

layout(location = 0) out vec2 tex_coord;

void main(){
	vec3 v;
	v = v_position;
	if (is_image == 0)   //daca se reda reteaua de triunghiuri
	{
		//pozitiile trebuie mutate din intervalul [0,1] in [-1,1]
		v = v_position * vec3(2,2,1) - vec3(1,1,0);
	}
	
	//altfel, pentru quad-ul pe care se pune o imagine, nu se modifica 
	tex_coord = v_texture_coord;

	gl_Position = vec4(v, 1); 
}
