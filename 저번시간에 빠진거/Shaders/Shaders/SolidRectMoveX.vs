#version 450

in vec2 a_Position;
in vec4 a_StartLifeRatioAmp;
in vec3 a_f2DirfSpeed;
//in vec2 a_elTime;

uniform float gTime;

const float PI = 3.141592;

const vec3 c_Gravity=vec3(0,-1,0);

const float TestSpeed =1;

//Vearying->>fragmentshader input
out vec4 v_Color;
out vec3 v_Center;
out vec3 v_Pos;
out float v_Size;

void main()
{
	v_Center=vec3(0,0,0);
	vec3 newPos= vec3(a_Position.xy,0);
	v_Size=abs(a_Position.x);
	vec2 Dir= a_f2DirfSpeed.xy;
	float Speed= a_f2DirfSpeed.z;

		float e=Dir.y*PI*2;
		mat2 MRot1=mat2(
				cos(e),-sin(e),
				sin(e), cos(e)
			);

	vec2 CirPos=vec2(0.8,0);
	CirPos=CirPos*MRot1;

	float start=a_StartLifeRatioAmp.x;
	float lifeTime=a_StartLifeRatioAmp.y*TestSpeed;
	float ratio=a_StartLifeRatioAmp.z;//аж╠Б
	float amp=a_StartLifeRatioAmp.w;

	float newTime =gTime-start;
	if(newTime>0)
	{
		newTime*=Speed*TestSpeed;
		newTime=mod(newTime,lifeTime);
		newPos.x+=newTime;
		newPos.y+=sin(newTime*2*PI*ratio)*amp*(newTime+0.1);
		v_Center.x+=newTime;
		v_Center.y+=sin(newTime*2*PI*ratio)*amp*(newTime+0.1);
		

		float d=Dir.x;

		mat2 MRot=mat2(
				cos(d),-sin(d),
				sin(d),cos(d)
			);
			
		newPos.xy=newPos.xy*MRot;
		newPos+=vec3(CirPos,0);

		v_Center.xy=v_Center.xy*MRot;
		v_Center+=vec3(CirPos,0);

		vec3 newVel=vec3(0,-1,0)*c_Gravity*newTime;

	//	newPos+=vec3(0,-1,0)*newTime+0.5*c_Gravity*newTime*newTime;
	//	v_Center+=vec3(0,-1,0)*newTime+0.5*c_Gravity*newTime*newTime;
	}
	else
	{
		newPos=vec3(10000,10000,10000);
		v_Center=vec3(10000,10000,10000);
	}

	float alpha=1-newTime/lifeTime;
	alpha*=alpha*alpha;
	gl_Position = vec4(newPos, 1);
	v_Pos=newPos;
	v_Color=vec4(abs(normalize(vec3(a_Position,0)+normalize(a_f2DirfSpeed))),alpha);
}
