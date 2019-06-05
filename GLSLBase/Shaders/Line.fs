#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_TextureSnow;
uniform sampler2D u_TextureGrass;

const vec3 c_Light1 = vec3(0.0, 10.0, 2.0); //카메라 위치 보다 조금 위
const vec3 c_CameraDir = vec3(0.0, -1.0, 0.3); 

in float v_Grey;
in vec2 v_Tex;
in vec3 v_Norm;
in vec3 v_Pos;

void main()
{
	//vec3 newColor = vec3(v_Grey);
	//FragColor = vec4(newColor,1);

	//FragColor = texture(u_Texture, vec2(v_Tex.x, 1.0-v_Tex.y))*v_Grey;

	vec4 snowColor = texture(u_TextureSnow, v_Tex * 10.0) * v_Grey;
	vec4 grassColor = texture(u_TextureGrass, v_Tex * 10.0) * (1.0 - v_Grey);
	vec4 finalColor = snowColor + grassColor;

	float a = 0.4;
	float d = 0.5;
	float s = 0.3;

	vec3 lightDir = c_Light1 - v_Pos;
	vec3 ambient = vec3(1, 1, 1);
	float diffuse = clamp(dot(lightDir, v_Norm), 0.0, 1.0);
	vec3 diffuseColor = vec3(1, 1, 1);
	vec3 reflectDir = reflect(lightDir, v_Norm);
	vec3 viewDir = v_Pos - c_CameraDir;
	vec3 specColor = vec3(1, 1, 1);
	float spec = clamp(dot(viewDir, reflectDir), 0.0, 1.0);
	spec = pow(spec, 6.0);

	vec3 newColor = ambient*a + diffuseColor*diffuse*d + specColor*spec*s;

//	FragColor = finalColor;
//	FragColor = vec4(abs(v_Norm), 1);
//	FragColor = vec4(v_Norm, 1);
	FragColor = vec4(newColor, 1);

}
