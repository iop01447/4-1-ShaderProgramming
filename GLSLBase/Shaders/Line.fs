#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_TextureSnow;
uniform sampler2D u_TextureGrass;

in float v_Grey;
in vec2 v_Tex;


void main()
{
	//vec3 newColor = vec3(v_Grey);
	//FragColor = vec4(newColor,1);

	//FragColor = texture(u_Texture, vec2(v_Tex.x, 1.0-v_Tex.y))*v_Grey;

	vec4 snowColor = texture(u_TextureSnow, v_Tex * 10.0) * v_Grey;
	vec4 grassColor = texture(u_TextureGrass, v_Tex * 10.0) * (1.0 - v_Grey);
	vec4 finalColor = snowColor + grassColor;

	FragColor = finalColor;

}
