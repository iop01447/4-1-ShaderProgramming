#version 450

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_UV;

uniform float gTime;

out vec2 v_Color;

void main()
{
	vec4 temp=vec4(a_Position,0);
	vec2 tempUV=a_UV;
	v_Color=tempUV;
	gl_Position = vec4(temp.xyz, 1);
}
