#version 450

in vec3 a_Position;

uniform float gTime;
uniform vec2 u_Points[5];

const float PI = 3.14;
out float v_Grey;
out vec2 v_Tex;

void Flag()
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

void Wave()
{
	float grey = 0.0;
	vec3 newPos = a_Position.xyz;
	for (int i=0; i<5; i++)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i]; //vec2(0.0, 0.0);
		float dis = length(target - source) * 4 * PI * 2 * float(i); //0~0.5 --> 0~2PI
		grey += sin(dis - gTime);
	}

	newPos.z += grey * 0.5; // 클리핑 되어서 이렇게 보임...

	gl_Position = vec4(newPos.xyz, 1);

	v_Grey = (grey + 1.0)/2.0;
	v_Tex = vec2(0.5, 0.5) + a_Position.xy;
}

float lerp(float p1, float p2, float d1) { // mix라는 glsl 함수와 동일
  return (1-d1)*p1 + d1*p2;
}

void SphereMapping()
{
	float grey = 0.0;
	for (int i=0; i<2; i++)
	{
		vec2 target;
		vec2 source;
		target = a_Position.xy;
		source = u_Points[i]; //vec2(0.0, 0.0);
		float dis = length(target - source) * 4 * PI * 2 * float(2); //0~0.5 --> 0~2PI
		grey += sin(dis - gTime/10);
	}
	///
	float r = (grey)*0.1;// + 0.5; //0.5; // 구의 반지름
	float beta = (a_Position.x+0.5) * 2 * PI;  // 경도/ 0~2*PI
	float theta = (a_Position.y+0.5) * PI; // 위도/ 0~PI
	vec3 spherePos = a_Position.xyz;

	spherePos = vec3(
		r*sin(theta)*cos(beta),
		r*sin(theta)*sin(beta),
		r*cos(theta)
	); // 새로운 구면 좌표계
	
	vec3 originPos = a_Position.xyz;
	float interpol = fract(gTime/100);
	vec3 newPos = mix(originPos, spherePos, interpol);

	gl_Position = vec4(spherePos.xyz, 1);
	v_Grey = 1;
}

void main()
{
	//Flag();
	//Wave();
	SphereMapping();
}
