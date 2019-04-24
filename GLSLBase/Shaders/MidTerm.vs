#version 450

layout(location = 0) in vec3 a_Position;

out vec2 v_OriXY;
out float v_Radius;

void main()
{
	v_OriXY = a_Position.xy;
	v_Radius = abs(a_Position.x);

	gl_Position = vec4(a_Position.xyz, 1);
}
