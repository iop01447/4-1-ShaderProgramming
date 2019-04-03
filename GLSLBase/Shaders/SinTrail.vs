#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp; // Ratio Amplitude

uniform float u_Time;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0); // 90% È¸Àü

void main()
{
	vec3 newPos = a_Position.xyz;

	float startTime = a_StartLifeRatioAmp.x;
	float lifeTime = a_StartLifeRatioAmp.y;
	float ratio = a_StartLifeRatioAmp.z;
	float amp = a_StartLifeRatioAmp.w;

	float newTime = u_Time - startTime;

	if(newTime > 0)
	{
		amp = amp * newTime * newTime;

		newTime = mod(newTime, lifeTime);
		newPos += a_Vel * newTime;

		vec3 vSin = a_Vel * c_RP;
		newPos += vSin * sin(newTime * PI * 2 * ratio) * amp; // ÁÖ±â) * Æø
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}

	gl_Position = vec4(newPos, 1);
}
