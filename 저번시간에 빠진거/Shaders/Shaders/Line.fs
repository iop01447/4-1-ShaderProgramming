#version 450

layout(location=0) out vec4 FragColor;

void main()
{
	vec4 color=vec4(0.2,0.65,0.7,1);
	//color.xyz=gl_PointCoord.xyz;
	FragColor = color;
}
