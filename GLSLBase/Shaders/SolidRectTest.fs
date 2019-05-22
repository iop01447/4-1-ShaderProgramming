#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec3 v_Center;
in vec3 v_Pos;
in float v_Size;

void main()
{
	vec4 color=v_Color;

	float tmp=length(v_Pos-v_Center)/v_Size;
	//float tmp=v_Size;
	tmp*=tmp*tmp;
	tmp*=tmp*tmp;

	color.w*=1-tmp;
	FragColor = color;
}
