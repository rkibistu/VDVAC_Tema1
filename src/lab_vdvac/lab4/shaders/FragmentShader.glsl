#version 430
layout(location = 0) out vec4 out_color;

uniform sampler2D textura1;
uniform int is_image;
layout(location = 0) in vec2 tex_coord;

void main(){
	//
	vec4 color = vec4(0,1,0,1);
	if (is_image == 1)
		color = texture(textura1,tex_coord);
	out_color = color;
}