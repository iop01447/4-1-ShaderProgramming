#version 450

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;
uniform sampler2D u_Texture;

uniform float u_Number;
uniform float u_ResolX;
uniform float u_ResolY;

void main()
{
	vec2 newTex;

	int x_index = int(floor(fract(u_Number / u_ResolX) * u_ResolX)); 
	int y_index = int(floor(floor(u_Number / u_ResolX)));
	// int y_index = 0;

	// x_tex coord
	// cal index ...
	newTex.x = v_Tex.x / float(u_ResolX) + float(x_index) / float(u_ResolX);
	newTex.y = v_Tex.y / float(u_ResolY) + float(y_index) / float(u_ResolY); 

	//int newIndex = int(floor(v_Tex.x * 3.0));
	//float newY = 9.0 - float(u_Number[newIndex]);
	//newTex.y = (newTex.y / 10.0) + (newY / 10.0);
	FragColor = texture(u_Texture, newTex);

}
