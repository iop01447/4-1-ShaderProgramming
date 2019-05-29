#version 450

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_UV;

uniform float gTime;

out vec2 v_Color;
out vec4 v_Pos;
out float m_Time;

void main()
{
	vec4 temp=vec4(a_Position,0);

	v_Color=vec4(a_UV.xy,0,1);
	v_Pos=gl_Position = vec4(temp.xyz, 1);
	m_Time=gTime;
}
