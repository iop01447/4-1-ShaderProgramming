#version 450

in vec3 a_Position;
in vec3 a_Vel;
in vec4 a_StartLifeRatioAmp; // Ratio Amplitude

uniform float u_Time;

const float PI = 3.141592;

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
		newTime = mod(newTime, lifeTime);

		amp = amp * newTime * newTime;
		newPos.x += newTime;
		newPos.y += sin(newTime * 2 * PI * ratio) * amp; // sin 救率捞 林扁, 官冰率捞 气
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}

	gl_Position = vec4(newPos, 1);
}
