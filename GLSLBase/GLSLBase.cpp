/*
Copyright 2018 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

//opengl regerence : https://www.slideshare.net/Khronos_Group/opengl-45-reference-card

/*
  ���̴������ϰ��� �ƴ��ѿ���Ű����ġʫ
  ������ �̻�����? �����̽��Ф��߸�������?
  �����׿�!!!!!
�ͤ��°������Ǥ��Է°� �����Ͷ������ҋ���..���������� ����!!!! �װŽἭ �����Ҽ����־����ϵ�
���������?�ƹ����̳�?

�����±�µ� ����10���̻�ƹٹٲ����ʤ����
�ߤ����ϰٳ׿�
������

�Է��� �����غ��ٽ��ϴ�
��������ְٳ׿�!!~!~!!~!!~!!~!!~~!~!~!~
���ؽ��� �Է������� ���ؽ���~~ �����Ӥ�������������������������������


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

