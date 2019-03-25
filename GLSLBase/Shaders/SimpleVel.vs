#version 450

in vec3 a_Position;
in vec3 a_Vel;

uniform float u_Time;

void main()
{
	vec3 newPos = a_Position.xyz;

	float newTime = fract(u_Time); // fract: �Ҽ��� �κи� ����� ��, �׻� 0~1 ���̸� �ݺ�

	newPos += a_Vel * newTime;

	gl_Position = vec4(newPos, 1);
}
