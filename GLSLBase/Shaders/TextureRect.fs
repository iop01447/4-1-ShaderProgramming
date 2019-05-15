#version 450

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;

in vec2 v_Tex;

uniform float u_Time;

uniform int u_Number[3];

void main()
{
	//default
	/*vec2 newTex = abs(v_Tex - vec2(0, 0.5)) + vec2(0, 0.5);
	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;*/

	//simple mean blurr
	/*vec2 newTex[5];
	vec4 newColor[5];
	float value = 1.0/64;
	
	newTex[0] = vec2(v_Tex.x, v_Tex.y);
	newTex[1] = vec2(v_Tex.x-value, v_Tex.y);
	newTex[2] = vec2(v_Tex.x+value, v_Tex.y);
	newTex[3] = vec2(v_Tex.x, v_Tex.y-value);
	newTex[4] = vec2(v_Tex.x, v_Tex.y+value);
    newColor[0] = texture(u_Texture, newTex[0]);
    newColor[1] = texture(u_Texture, newTex[1]);
    newColor[2] = texture(u_Texture, newTex[2]);
    newColor[3] = texture(u_Texture, newTex[3]);
    newColor[4] = texture(u_Texture, newTex[4]);
	vec4 finalColor = newColor[0] + newColor[1] + newColor[2] + newColor[3] + newColor[4];
	finalColor /= 5;
	FragColor = finalColor;*/

	/*vec2 newTex = v_Tex;
	newTex.y = abs(newTex.y-1);
	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;*/

	// b g r
	/*vec2 newTex;
	newTex.y = -v_Tex.y;
	newTex.x = fract(v_Tex.x * 3);
	newTex.y = newTex.y / 3.0;
	newTex.y += floor(v_Tex.x * 3) / 3.0;*/
	
	// 2
	/*vec2 newTex = v_Tex;
	newTex.y = abs(newTex.y-1);

	newTex.x = fract(v_Tex.x * 3.0);
	newTex.y = v_Tex.y / 3.0;
	newTex.y += (2-floor(v_Tex.x*3.0))/3.0;
	newTex.y = -newTex.y;

	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;*/

	// 위아래 반전 해결: 1.0-v_Tex.y

	// Multiple Textures
	/*vec4 newColor;
	vec2 newTex = vec2(v_Tex.x, 1.0-v_Tex.y);
	
	if(v_Tex.x < 0.5 && newTex.y < 0.5){
		FragColor = texture(u_Texture2, vec2(newTex.x * 2, newTex.y * 2));
	}
	else if(v_Tex.x > 0.5 && newTex.y < 0.5){
		FragColor = texture(u_Texture3, vec2(fract(newTex.x * 2), newTex.y*2));
	}
	else if(v_Tex.x < 0.5 && newTex.y > 0.5){
		FragColor = texture(u_Texture, vec2(newTex.x * 2, fract(newTex.y*2)));
	}
	else if(v_Tex.x > 0.5 && newTex.y > 0.5){
		FragColor = texture(u_Texture1, vec2(fract(newTex.x * 2), newTex.y*2));
	}*/

	/*vec4 newColor;
	vec2 newTex = vec2(v_Tex.x, 1.0-v_Tex.y);
	float newx = float(u_Number);
	newTex.x = newTex.x/10.0 + newx/10.0;
	newColor = texture(u_Texture, newTex);
	FragColor = newColor;*/

	vec2 newTex = v_Tex;

	//x_tex coord
	newTex.x = fract(newTex.x*3.0);

	//cal index
	int newIndex = int(floor(v_Tex.x*3.0));

	float newx = float(u_Number[newIndex]);
	newTex.x = newTex.x/10.0 + newx/10.0;
	
	vec4 newColor = texture(u_Texture, newTex);
	FragColor = newColor;
}
