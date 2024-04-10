#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// uniforme
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) out vec3 EntryPoint;
layout(location = 1) out vec4 ExitPointCoord;

void main()
{


	gl_Position = Projection * View * Model * vec4(v_position, 1);

	//TODO3 
	// calculeaza EntryPoint, care este echivalenta cu culoarea per varf
	EntryPoint = v_color;
	// calculeaza ExitPointCoord, care este echivalenta cu pozitia per varf (in NDC)
	ExitPointCoord = gl_Position;

}
