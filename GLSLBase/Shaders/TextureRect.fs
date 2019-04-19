#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform float u_Time;

void main()
{
	//FragColor = vec4(v_Tex,0,1);

	vec4 newColor = vec4(0);

	float x = v_Tex.x * 2 * 3.14 + u_Time;
	float sinResult = (sin(x) + 1)/2; // / (v_Tex.x*10); //-1~1 -> 0~1
	float size = 0.01;

	if(sinResult + size > v_Tex.y  && sinResult - size < v_Tex.y )
	{
		newColor = vec4(1);
	}

	FragColor = newColor;
}
