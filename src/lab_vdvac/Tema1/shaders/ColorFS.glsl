#version 430
layout(location = 0) out vec4 out_color;


in vec3 out_pos;
layout(location = 1) in vec3 fragColor;

void main() {


	out_color = vec4(fragColor,1);

}