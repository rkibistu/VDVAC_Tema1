#version 430
layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 tex_coord;

uniform sampler2D textura1;

void main(){

	float tex = texture(textura1, tex_coord).x ;
	//canalul alfa - construit in functie de intensitatea din textura
	float alpha = tex.x * 0.1;
	out_color = vec4(tex* alpha, tex * alpha, tex * alpha, alpha);
}