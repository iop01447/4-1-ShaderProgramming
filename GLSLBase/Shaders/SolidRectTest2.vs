#version 450

in vec2 a_Position;
in vec2 a_Dir;
in vec2 a_elTime;

uniform float gTime;

const vec3 c_Gravity = vec3(0,-10,0);

void main()
{
	vec2 temp=a_Position.xy;
	vec2 dir=a_Dir.xy;
	temp.y+=sin(((gl_VertexID/6)*0.001)*gTime*30+(temp.x)*5.0)*0.01;
	temp.x+=cos(((gl_VertexID/6)*0.001)*gTime*30+(temp.y)*5.0)*0.01;

	vec3 t3=vec3(temp,0);



	float time=(gTime-(a_elTime.x+a_elTime.y)*int(gTime/(a_elTime.x+a_elTime.y)));

	if(a_elTime.x<time)
	{
		time=time-a_elTime.x;

	time*=2;

	t3 = vec3(temp,0)+ vec3(dir,0)*time+0.5*c_Gravity*time*time;
	}
	else
	t3=vec3(1000,1000,1000);

	gl_Position = vec4(t3, 1);
}
