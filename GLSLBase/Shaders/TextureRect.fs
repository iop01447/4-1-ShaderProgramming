#version 450

uniform sampler2D uTexSampler;
in vec2 vTexPos;

uniform float u_Time;

out vec4 FragColor;

void main()
{
	//vec2 newTex = fract(vTexPos * 3 + vec2(cos(u_Time), sin(u_Time)));
	vec2 newTex = abs(vTexPos - vec2(0, 0.5)) + vec2(0, 0.5);
	

    vec4 newColor = texture(uTexSampler, newTex);

	FragColor = newColor;
}
