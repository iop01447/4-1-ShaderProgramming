#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Temp;

uniform vec2 u_Points[5]; // 포인트 받아서 하는 것

const vec2 c_Points[2] = {vec2(0, 0), vec2(0.1, 0.1)}; // 이거 주변에서만 동그랗게 그리도록

void main()
{
	vec2 newUV = v_Temp.xy - vec2(0.5, 0.5);

	/*float distance = sqrt(v_Temp.x * v_Temp.x + v_Temp.y * v_Temp.y);
	float grey = sin(distance*3.141592*4*100);

	if(distance < 0.5 && distance > 0.48)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);
	}
	}*/
	//FragColor = v_Temp;
	//FragColor = vec4(newUV, 0, 1);
	//FragColor = vec4(grey);

	/*
	FragColor = vec4(0.2);
	for(int i=0; i<2; ++i){
		float tx = v_Temp.x - c_Points[i].x;
		float ty = v_Temp.y - c_Points[i].y;
		distance = sqrt(tx*tx + ty*ty);
		if(distance < 0.1)
		{
			FragColor = vec4(1);
		}
	}
	*/
	vec2 newUV = v_Temp.xy - vec2(0.5, 0.5); // -0.5~0.5, x, y

	float pointGrey = 0;

	for(int i=0; i<2; ++i)
	{
		vec2 newPoint = c_Points[i];
		vec2 newVec = newPoint - newUV;
		float distance = sqrt(newVec.x * newVec.x + newVec.y * newVec.y);
		if(distance < 0.1)
		{
			pointGrey += 0.5 * pow((1-(distance/0.1)), 5);
		}
	}
	FragColor = vec4(pointGrey, 0, 1);
}
