#version 450

in vec4 a_Position; // a_ : attribute 라는 것을 표현
in vec4 a_Color;
in vec2 a_UV;

out vec4 v_Temp;

uniform float u_Time;

void main()
{
	gl_Position = vec4(a_Position.xyz, 1);

	v_Temp = vec4(a_Position.xy, 0, 1);
	//v_Temp = vec4(a_UV.xy, 0, 1);
	//v_Temp = a_Position;
}
