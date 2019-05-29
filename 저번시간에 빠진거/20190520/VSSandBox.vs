#version 450
// Attribute
in vec3 a_Position;   
 
uniform float u_Time;
const float PI = 3.141592;

void main()
{
	vec3 newPos = a_Position;

	// x: -0.5 ~ 0.5 => + 0.5 =? 0~1 => *PI => 0~2pi
	// y: -0.5 ~ 0.5

	float value = (newPos.x + 0.5) * 2 * PI;
	float sin_value = sin(value);
	
	newPos.y = newPos.y + sin_value;
	 
	gl_Position = vec4(newPos.xyz, 1);  
}

// Pos = Pos + V*t + (1/2)*a*t*t