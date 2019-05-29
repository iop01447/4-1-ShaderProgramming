#version 450

uniform sampler2D uTexSampler;

layout(location=0) out vec4 FragColor;

in vec2 o_uv;

void main()
{

	vec2 texOff=o_uv;

	vec4 newcolor = texture(uTexSampler, texOff);

	FragColor=newcolor;

}
