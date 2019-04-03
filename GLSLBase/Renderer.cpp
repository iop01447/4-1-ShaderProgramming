#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <time.h>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs"); // ���̴� ���α׷� ���̵�
	m_SimpleVelShader = CompileShaders("./Shaders/SimpleVel.vs", "./Shaders/SimpleVel.fs");
	m_SinTrailShader = CompileShaders("./Shaders/SinTrail.vs", "./Shaders/SinTrail.fs");

	//Create VBOs
	CreateVertexBufferObjects();
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.02f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5f, // x, y, z, something
		-size, size, 0.f, 0.5f,
		size, size, 0.f, 0.5f, //Triangle1
		-size, -size, 0.f, 0.5f,
		size, size, 0.f, 0.5f,
		size, -size, 0.f, 0.5f //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float color[]
		=
	{
		1, 0, 0, 1,// r, g, b, a
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1
	};

	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	// lecture2
	float triangleVertex[]
		=
	{
		-1.0, -0.0, 0.f, 0.0, 1.0, 0.f, 1.0, 0.0, 0.f // 9 floats
	};

	glGenBuffers(1, &m_VBOLecture2); // ������ �ϳ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW); // memory copy�� �Ͼ�� �������� ����. ���� GPU�� �ø��� Ÿ�̹��� �� �����ؾ� �Ѵ�.

	m_QuadsCnt = 100000; //1000
	CreateVBOQuads(m_QuadsCnt, false, 0, 0, 0);

	CreateGridMesh();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	static float time = 1.f;
	static float addValue = 0.01;
	time += addValue;
	if (time > 1)
		time = 0;
	
	glUniform1f(uTime, time); // float ����Ʈ �ϳ�
	

	GLuint aPos = glGetAttribLocation(m_SolidRectShader, "a_Position");
	GLuint aCol = glGetAttribLocation(m_SolidRectShader, "a_Color"); // color�� ���ǰ� ���� �ʾƼ� �ǹ̾��� ���� ���ϵ�

	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(aPos, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glEnableVertexAttribArray(aCol);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(aCol, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aCol);

}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader); // ���̴� ���� �Ⱦ��ϱ� �Ѿ
	glEnableVertexAttribArray(0); // 0�� ���� ���� �Ⱦ˾Ƶ� ��
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // ù��° ���ڰ� 0�̶�� ���� glEnableVertexAttribArray�� 0�־

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader); // ���̴� ���� �Ⱦ��ϱ� �Ѿ
	glEnableVertexAttribArray(0); // 0�� ���� ���� �Ⱦ˾Ƶ� ��
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // 0�̶�� ���� glEnableVertexAttribArray�� 0�־

	glDrawArrays(GL_TRIANGLES, 0, m_VBOGridMesh_Count); // GL_LINE_STRIP / GL_TRIANGLES

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3_2()
{
	glUseProgram(m_SimpleVelShader); 

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	static float time = 0;
	time += 0.01;
	//if (time > 2)
		//time = 0;
	glUniform1f(uTime, time);

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");

	glEnableVertexAttribArray(aPos); // Test: �� �Լ��� �� ����? 
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float)*3));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsCnt); // GL_LINE_STRIP
	
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
}

void Renderer::Lecture3_3()
{
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_SimpleVelShader, "u_Repeat");

	static float time = 0;
	time += 0.01;
	glUniform1f(uTime, time);
	//glUniform1f(uRepeat, true);

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_SimpleVelShader, "a_StartLife");

	glEnableVertexAttribArray(aPos); // Test: �� �Լ��� �� ����? 
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsCnt); // GL_LINE_STRIP

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}

void Renderer::Lecture3_4()
{
	GLuint shader = m_SinTrailShader;
	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	
	static float time = 0;
	time += 0.01;
	glUniform1f(uTime, time);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Vel");
	GLuint aStartLifeRatioAmp = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	GLuint aTheta = glGetAttribLocation(shader, "a_Theta");


	glEnableVertexAttribArray(aPos); // Test: �� �Լ��� �� ����? 
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLifeRatioAmp);
	glEnableVertexAttribArray(aTheta);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLifeRatioAmp, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aTheta, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 10));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsCnt); // GL_LINE_STRIP

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLifeRatioAmp);
	glDisableVertexAttribArray(aTheta);
}

void Renderer::CreateVBOQuads(int count, bool is_random, float x, float y, float z)
{
	float quad_size = 0.01f;
	int countQuad = count;
	int verticesPerQuad = 11;
	int floatsPerVertex = 3 + 3 + 2;
	std::vector<float> quads;
	quads.reserve(countQuad * verticesPerQuad * floatsPerVertex); // ���ؽ� ���� * ���ؽ� ���� ��� * �簢�� ���� 

	for (int i = 0; i < count; ++i)
	{
		float randx = 0;
		float randy = 0;
		if (is_random) {
			randx = 2.f* (((float)rand() / (float)RAND_MAX) - 0.5f);
			randy = 2.f* (((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else {
			randx = x;
			randy = y;
		}
		// 6���� ���ؽ��� ������ �ӵ� �ֱ�
		float randVelx = 2.f* (((float)rand() / (float)RAND_MAX) - 0.5f);
		float randVely = 2.f* (((float)rand() / (float)RAND_MAX) - 0.5f);
		float randVelz = 0.f;

		float startTimeMax = 10.f;
		float lifeTimeMax = 3.f;
		float startTime = startTimeMax * ((float)rand() / (float)RAND_MAX);
		float lifeTime = 3 + lifeTimeMax * ((float)rand() / (float)RAND_MAX);

		float randRatio = 4 * ((float)rand() / (float)RAND_MAX);
		float randAmplitude = 0.2 * ((float)rand() / (float)RAND_MAX);

		float randTheta = 2 * 3.14 * ((float)rand() / (float)RAND_MAX);

		// triangle 1
		quads.emplace_back(randx);
		quads.emplace_back(randy);
		quads.emplace_back(0);
		quads.emplace_back(randVelx);
		quads.emplace_back(randVely);
		quads.emplace_back(randVelz);
		quads.emplace_back(startTime);
		quads.emplace_back(lifeTime);
		quads.emplace_back(randRatio);
		quads.emplace_back(randAmplitude);
		quads.emplace_back(randTheta);

		quads.emplace_back(randx + quad_size);
		quads.emplace_back(randy);
		quads.emplace_back(0);
		quads.emplace_back(randVelx);
		quads.emplace_back(randVely);
		quads.emplace_back(randVelz);
		quads.emplace_back(startTime);
		quads.emplace_back(lifeTime);
		quads.emplace_back(randRatio);
		quads.emplace_back(randAmplitude);
		quads.emplace_back(randTheta);

		quads.emplace_back(randx);
		quads.emplace_back(randy + quad_size);
		quads.emplace_back(0);
		quads.emplace_back(randVelx);
		quads.emplace_back(randVely);
		quads.emplace_back(randVelz);
		quads.emplace_back(startTime);
		quads.emplace_back(lifeTime);
		quads.emplace_back(randRatio);
		quads.emplace_back(randAmplitude);
		quads.emplace_back(randTheta);

		// triangle 2
		quads.emplace_back(randx + quad_size);
		quads.emplace_back(randy);
		quads.emplace_back(0);
		quads.emplace_back(randVelx);
		quads.emplace_back(randVely);
		quads.emplace_back(randVelz);
		quads.emplace_back(startTime);
		quads.emplace_back(lifeTime);
		quads.emplace_back(randRatio);
		quads.emplace_back(randAmplitude);
		quads.emplace_back(randTheta);

		quads.emplace_back(randx);
		quads.emplace_back(randy + quad_size);
		quads.emplace_back(0);
		quads.emplace_back(randVelx);
		quads.emplace_back(randVely);
		quads.emplace_back(randVelz);
		quads.emplace_back(startTime);
		quads.emplace_back(lifeTime);
		quads.emplace_back(randRatio);
		quads.emplace_back(randAmplitude);
		quads.emplace_back(randTheta);

		quads.emplace_back(randx + quad_size);
		quads.emplace_back(randy + quad_size);
		quads.emplace_back(0);
		quads.emplace_back(randVelx);
		quads.emplace_back(randVely);
		quads.emplace_back(randVelz);
		quads.emplace_back(startTime);
		quads.emplace_back(lifeTime);
		quads.emplace_back(randRatio);
		quads.emplace_back(randAmplitude);
		quads.emplace_back(randTheta);
	}

	glGenBuffers(1, &m_VBOQuads);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glBufferData(GL_ARRAY_BUFFER, quads.size() * sizeof(float), quads.data(), GL_STATIC_DRAW);
}

void Renderer::DrawQuads()
{
	glUseProgram(m_SolidRectShader); // ���̴� ���� �Ⱦ��ϱ� �Ѿ
	glEnableVertexAttribArray(0); // 0�� ���� ���� �Ⱦ˾Ƶ� ��
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); // 0�̶�� ���� glEnableVertexAttribArray�� 0�־

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsCnt);

	glDisableVertexAttribArray(0);
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	std::vector<float> vertices;
	vertices.reserve((pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3);
	//float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_VBOGridMesh_Count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices.push_back(point[(y*pointCountX + x) * 2 + 0]);
			vertices.push_back(point[(y*pointCountX + x) * 2 + 1]);
			vertices.push_back(0.f);

			vertices.push_back(point[((y + 1)*pointCountX + (x + 1)) * 2 + 0]);
			vertices.push_back(point[((y + 1)*pointCountX + (x + 1)) * 2 + 1]);
			vertices.push_back(0.f);
			vertices.push_back(point[((y + 1)*pointCountX + x) * 2 + 0]);
			vertices.push_back(point[((y + 1)*pointCountX + x) * 2 + 1]);
			vertices.push_back(0.f);

			//Triangle part 2
			vertices.push_back(point[(y*pointCountX + x) * 2 + 0]);
			vertices.push_back(point[(y*pointCountX + x) * 2 + 1]);
			vertices.push_back(0.f);
			vertices.push_back(point[(y*pointCountX + (x + 1)) * 2 + 0]);
			vertices.push_back(point[(y*pointCountX + (x + 1)) * 2 + 1]);
			vertices.push_back(0.f);
			vertices.push_back(point[((y + 1)*pointCountX + (x + 1)) * 2 + 0]);
			vertices.push_back(point[((y + 1)*pointCountX + (x + 1)) * 2 + 1]);
			vertices.push_back(0.f);
		}
	}

	glGenBuffers(1, &m_VBOGridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices.data(), GL_STATIC_DRAW);
}

