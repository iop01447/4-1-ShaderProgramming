#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_OriXY;
in float v_Radius;

uniform float u_Time;

void main()
{
	float dis = sqrt(v_OriXY.x * v_OriXY.x + v_OriXY.y * v_OriXY.y);
	vec4 newColor;
	float rad = (1-u_Time)*1 + u_Time*v_Radius;
	if(dis < rad){
		newColor = vec4(0,1,0,1);
	}
	else{
		newColor = vec4(0,0,0,0);
	}
	FragColor = newColor;
}
