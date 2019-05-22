#version 450

in vec3 a_Position;

uniform float gTime;

void main()
{
	vec3 temp=a_Position;

	float o=sin(gTime+(-0.5)*5.0)*0.3*-1.0;

	float size=smoothstep(-0.5,0.5,temp.x);
	size=1-size*0.7;

	temp.y+=sin(gTime+(temp.x)*5.0)*0.3+o;
	temp.y*= size;

	temp.x+=sin(gTime+(a_Position.y)*5.0)*smoothstep(-0.5,0.5,temp.x)*0.1;

	gl_Position = vec4(temp, 1);
}
