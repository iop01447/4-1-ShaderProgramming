#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_Color;

void main()
{
	FragColor = vec4(v_Color.xy,0,1);
}
