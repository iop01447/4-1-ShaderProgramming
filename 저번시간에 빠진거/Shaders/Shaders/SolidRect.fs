#version 450

layout(location=0) out vec4 FragColor;
in vec4 v_Color;
in vec4 v_Pos;
in float m_Time;

const int c_Size=5;
uniform vec2 c_Point[5];

void main()
{
	float distance =sqrt(v_Pos.x*v_Pos.x+v_Pos.y*v_Pos.y);


	float  time=m_Time*5;
	distance= max(0, distance*(tan((-time))))
			+   max(0, distance*(tan((-3.1415*0.5-time))));
	//distance : 0~oo, 0~oo, 0~oo

	//distance=max(0,distance*(-time-floor(-time)))*3.14*2;
	
	float colorOne=abs(tan(distance)-2);
	
	colorOne=	max(0,	tan(			-distance))
			+   max(0,	tan(-3.1415*0.5	-distance));
	vec4 color=1-vec4(colorOne);

	vec4 PointColor;
	PointColor=vec4(0);
	for(int i=0;i<c_Size;i++)
	{
		if(length(v_Pos.xy-c_Point[i])<0.03)
			PointColor=vec4(0.5);
	}

	//color.xyz=gl_PointCoord.xyz;


	FragColor = color +PointColor;


}
