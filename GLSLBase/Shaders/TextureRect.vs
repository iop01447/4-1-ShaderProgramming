#version 450

uniform vec2 u_Pos;
uniform vec2 u_Size;

in vec3 a_Position;
in vec2 a_Tex;

out vec2 v_Tex;

void main()
{
	vec3 newPos = vec3(a_Position.xy * u_Size.xy + u_Pos.xy, 0);
	gl_Posiiton = vec4(newPos, 1);
	v_Tex = a_Tex;
}
