#version 450

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_UV;

out vec2 o_uv;

void main()
{
	vec4 temp=vec4(a_Position,0);

	o_uv=a_UV;

	gl_Position = vec4(temp.xyz, 1);
}
