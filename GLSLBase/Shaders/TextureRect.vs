#version 450

in vec3 Position;
in vec2 TexPos;


out vec2 v_Tex;

void main()
{
	vec3 newPos = Position;
	vec2 newTex = TexPos;
//	newTex *= 2;
    gl_Position = vec4(newPos, 1.0);
    v_Tex = newTex;
}
