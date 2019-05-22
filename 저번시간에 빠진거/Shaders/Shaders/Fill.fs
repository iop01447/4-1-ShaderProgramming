#version 450

uniform sampler2D uTexSampler;
uniform sampler2D uTexSampler2;
uniform sampler2D uTexSampler1;

layout(location=0) out vec4 FragColor;

uniform float gTime;

in vec2 o_uv;

void main()
{

float tmp=abs(sin(gTime));

	vec2 texOff=o_uv;
//	texOff.y-=0.5;
//	texOff.y=abs(texOff.y)+0.5;

	texOff.y*=-1;

	float tmpx=floor(5.0*(abs(sin(gTime))+1));
	float tmpy=floor(5.0*(abs(sin(gTime))+1));

	texOff.x*=tmpx;
	texOff.y*=tmpy;
	texOff.x+=floor(texOff.y-1)*0.5;

	vec4 newcolor = texture(uTexSampler, texOff);

	if(o_uv.x>0.33)
		 newcolor= texture(uTexSampler2, texOff);

	if(o_uv.x>0.66)
		 newcolor= texture(uTexSampler1, texOff);

	FragColor=newcolor;

}
