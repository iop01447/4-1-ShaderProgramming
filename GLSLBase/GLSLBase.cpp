/*
Copyright 2018 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

//opengl regerence : https://www.slideshare.net/Khronos_Group/opengl-45-reference-card

/*
  쎼이더설정하가ㅣ 아니한영ㅊ키가위치家
  ㅇ가가 이상하죠? 스페이스ㅠㅏ잘못눌리죠?
  귀찮네요!!!!!
익ㅂ력값고유의ㅏ입력갑 레지터라지으할떄의..교윳ㄱ밧이 있죠!!!! 그거써서 뭔가할수도있쓰ㅃ니따
상관없나요?아무색이나?

뭔갑좌기는데 구조10년이상아바바뀌지않ㄴ을까여
중ㅇ요하겟네요
오늘은

입력을 조절해보겟습니다
정말재미있겟네요!!~!~!!~!!~!!~!!~~!~!~!~
펍텍스의 입력정보는 버텍스다~~ ㄹㅇ임ㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎㅎ


*/

#include "stdafx.h"
#include <chrono>
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"

Renderer *g_Renderer = NULL;

int g_WindowSizeX = 500;
int g_WindowSizeY = 500;

bool test{ true };
bool test2{ true };

float testtime{0};
static std::chrono::time_point<std::chrono::high_resolution_clock> otime;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Renderer Test
	//g_Renderer->TestFill();
	//g_Renderer->TestWhiteRect(testtime);
	//g_Renderer->TestMoveX(testtime);
	//g_Renderer->TestUVRect(testtime);
	//g_Renderer->TestNumDrawO(testtime, floor(testtime * 1000));
	//g_Renderer->TestNumDraw(testtime,floor(testtime*100));

	g_Renderer->Test_CULINE(testtime);
	glutSwapBuffers();
}

void Idle(void)
{
	static auto& n=  std::chrono::high_resolution_clock::now();
	float del = std::chrono::duration<float>(n - otime).count();
	testtime += del;
	//std::cout << "test - " << del << std::endl;
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	if (state == GLUT_UP)
		switch (button )
		{
		case GLUT_LEFT_BUTTON:
			test = !test;
			break;
		case GLUT_RIGHT_BUTTON:
			test2 = !test2;
			break;

		default:
			break;
		}

	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("GLSL KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_6"))
	{
		std::cout << " GL Version is 4.6\n ";
	}
	else
	{
		std::cout << "GLEW 4.6 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(g_WindowSizeX, g_WindowSizeY);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	otime = std::chrono::high_resolution_clock::now();
	glutMainLoop();

	delete g_Renderer;

    return 0;
}

