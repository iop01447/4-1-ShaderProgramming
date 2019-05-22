#version 450

in vec3 a_Position;

uniform float gTime;

const float PI = 3.14;
out float v_Grey;
out vec2 v_Tex;

void main()
{
	vec3 newPos = a_Position;

	//0~1
	float additionalValueX = newPos.x + 0.5; //0~1
	float additionalValueY = newPos.y + 0.5; //0~1

	float periodX = 1.0 + (1.0 - additionalValueY) * 0.5;
	float periodY = 1.0 + additionalValueX * 0.5;

	//x :: -0.5~0.5 --> +0.5 -> 0~1 -> * 2 * PI -> 0~2PI
	float valueX = (additionalValueY * 2 * PI * periodX) - gTime*12.0;
	float valueY = (additionalValueX * 2 * PI * periodY) - gTime*5.0;
	
	float sinValueX = sin(valueX) * 0.08;
	float sinValueY = sin(valueY) * 0.2;

	//y scale
	newPos.y = newPos.y * ((1.0 - additionalValueX) * 0.5 + 0.5);

	//x
	newPos.x = newPos.x - sinValueX * additionalValueX;
	//y
	newPos.y = newPos.y + sinValueY * additionalValueX;

	gl_Position = vec4(newPos.xyz, 1);

	v_Grey = sinValueY + 0.5;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy; // 0~1, 0~1 tex coordinate
}
