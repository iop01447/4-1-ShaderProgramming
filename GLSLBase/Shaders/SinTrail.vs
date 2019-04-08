#version 450

// Attributes
in vec3 a_Position; // in이 아니라 attribute라고 써도 잘 동작
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp; // Ratio Amplitude
in float a_Theta;
in vec4 a_Color;

// Varying --> fragment shader input
out vec4 v_Color;

uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0); // 90도 회전
const vec3 c_Gravity = vec3(0, -1, 0);
const float radius = 0.5;

void main()
{
	vec3 newPos = a_Position.xyz;
	vec3 newVel = a_Vel.xyz;

	float startTime = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;

	float newTime = u_Time - startTime;

	if(newTime > 0)
	{
		//amp = amp * newTime * newTime;
		newPos.x += cos(a_Theta);
		newPos.y += sin(a_Theta);

		newTime = mod(newTime, lifeTime);

		newVel += c_Gravity * newTime;

		newPos += a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime; // 초기속도가 들어가야 함

		vec3 vSin = a_Vel * c_RP;
		//newPos += vSin * sin(newTime * PI * 2 * ratio) * amp; // 주기) * 폭
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}

	gl_Position = vec4(newPos, 1);

	v_Color = vec4(1,0,0,1);
}
