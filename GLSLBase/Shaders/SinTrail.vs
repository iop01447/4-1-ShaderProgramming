#version 450

in vec3 a_Position;
in vec4 a_StartLifePerAmp; // Period Amplitude

uniform float u_Time;

const float PI = 3.141592;

void main()
{
	vec3 newPos = a_Position.xyz;

	float startTime = a_StartLifePerAmp.x;
	float lifeTime = a_StartLifePerAmp.y;
	float period = a_StartLifePerAmp.z;
	float amplitude = a_StartLifePerAmp.w;

	float newTime = u_Time - startTime;
	//float newTime = fract(u_Time);

	newPos.x += newTime;
	newPos.y += sin(newTime * PI * period) * amplitude;

	float life = newTime;
	float remainingLife = lifeTime - life;

	if(u_Repeat == true)
	{
		remainingLife = 1.f;
		newTime = mod(newTime, lifeTime);
	}

	if(newTime > 0 && remainingLife > 0)
	{
		newPos += (a_Vel * newTime) + (0.5 * c_Gravity * newTime * newTime);
	}
	else
	{
		newPos = vec3(100000, 100000, 100000);
	}

	gl_Position = vec4(newPos, 1);
}
