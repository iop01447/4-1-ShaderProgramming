#version 450

in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1); // a_Position에 0.5 곱하면 0.5배로 그려짐
}
