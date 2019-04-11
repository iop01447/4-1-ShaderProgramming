#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_OriginXY;
in float v_Radius;

void main()
{
	vec4 newColor;
	float dis = sqrt(v_OriginXY.x * v_OriginXY.x + v_OriginXY.y * v_OriginXY.y);
	if(dis < v_Radius)
	{
		newColor = v_Color;
		newColor.a = 1;//pow(1- dis / v_Radius, 2);
	}
	else
	{
	//	newColor = v_Color;
		newColor = vec4(0,0,0,0);
	}
	FragColor = newColor;//vec4(0.2,0.2,0.8,1);
}
