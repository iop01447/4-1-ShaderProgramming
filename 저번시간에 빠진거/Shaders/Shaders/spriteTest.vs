#version 450

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_UV;

out vec2 o_uv;

uniform float gTime;
uniform int gNum;

uniform vec3 gPosition;
uniform vec2 gSize;

void main()
{
	vec4 temp=vec4(a_Position,0);
	temp.xy*=gSize;
	temp.xyz+=gPosition;

	vec2 tmpuv=a_UV;

	tmpuv.x*=0.1;
	tmpuv.x+=0.1*gNum;
	tmpuv.y*=-1;
	o_uv=tmpuv;

	gl_Position = vec4(temp.xyz, 1);
}
