#version 450

in vec3 a_Position;

uniform float gTime;

void main()
{
	vec3 temp=a_Position;
	temp.y+=sin(gTime+(temp.x)*5.0)*0.3;
	gl_Position = vec4(temp, 1);
}
