#version 450

in vec4 a_Position; // a_ : attribute ��� ���� ǥ��
in vec4 a_Color;

uniform float u_Time;

void main()
{
	//vec3 temp;
	//temp = a_Position.xyz * a_Position.w * u_Time;

	//float newX, newY;

	//newX = sin(u_Time*3.141592*2.0);
	//newY = cos(u_Time*3.141592*2.0);
	
	//newX += a_Position.x;
	//newY += a_Position.y;

	//gl_Position = vec4(newX, newY, 0, 1); // a_Position�� 0.5 ���ϸ� 0.5��� �׷���
	gl_Position = vec4(a_Position.xyz, 1);
}
