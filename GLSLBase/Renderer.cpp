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
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs"); // 셰이더 프로그램 아이디
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
		-size, -size, 0.f, 0.5f, // x, y, z, value
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

	glGenBuffers(1, &m_VBOLecture2); // 개수는 하나
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW); // memory copy가 일어나서 생각보다 느림. 따라서 GPU에 올리는 타이밍을 잘 설정해야 한다.

	GenQuadsVBO(100, false, 0.1f, 0, 0, 0);
	CreateGridMesh();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
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

void Renderer::GenQuadsVBO(int count, bool is_random, float size, float x, float y, float z)
{
	m_QuadsCnt = count;
	float quad_size = size;// 0.05f;//1.f; //0.2f;
	int countQuad = count;
	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4; // x, y, z, vx, vy, vz, start, life, rat, amp, theta, r, g, b, a
	std::vector<float> quads;
	quads.reserve(countQuad * verticesPerQuad * floatsPerVertex); // 버텍스 개수 * 버텍스 구성 요소 * 사각형 개수 

	for (int i = 0; i < count; ++i)
	{
		float randX, randY;
		float randVX, randVY, randVZ;
		float randST, randLT; // emit(start) time, life time
		float STMax = 5.f, LTMax = 3.f, LTMin = 1.f;
		float ratio, amp;
		float ratioMin = 2.f;
		float ratioThres = 4.f;
		float ampMin = -0.1f;
		float ampThres = 0.2;
		float randVal = 0.f;
		float randThres = 1.f;
		float randR, randG, randB, alpha;

		randX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		randVX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVZ = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		randST = ((float)rand() / (float)RAND_MAX) * STMax;
		randLT = ((float)rand() / (float)RAND_MAX) * LTMax + LTMin;

		randX = 0.f;  // Lecture 6을 위해
		randY = 0.f;  // Lecture 6을 위해

		ratio = ratioMin + ((float)rand() / (float)RAND_MAX * ratioThres);
		amp = ampMin + ((float)rand() / (float)RAND_MAX * ampThres);

		randVal = randVal + ((float)rand() / (float)(RAND_MAX)* randThres);

		randR = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randG = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randB = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		alpha = 1.f;

		int arr[12]{ -1,-1,		-1,1,	1,1,	-1,-1,	1,-1,	1,1 };
		int index = 0;

		// triangle 1, 2
		for (int j = 0; j < 6; ++j) {
			quads.emplace_back(randX + quad_size * arr[index++]);
			quads.emplace_back(randY + quad_size * arr[index++]);
			quads.emplace_back(0);
			quads.emplace_back(randVX);
			quads.emplace_back(randVY);
			quads.emplace_back(randVZ);
			quads.emplace_back(randST);
			quads.emplace_back(randLT);
			quads.emplace_back(ratio);
			quads.emplace_back(amp);
			quads.emplace_back(randVal);
			quads.emplace_back(randR);
			quads.emplace_back(randG);
			quads.emplace_back(randB);
			quads.emplace_back(alpha);
			//quads.emplace_back(randX);
			//quads.emplace_back(randY);
		}
	}

	glGenBuffers(1, &m_VBOQuads);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glBufferData(GL_ARRAY_BUFFER, quads.size() * sizeof(float), quads.data(), GL_STATIC_DRAW);

	m_VBOQuads_vertexCount = count * verticesPerQuad;
}

void Renderer::DrawQuads()
{
	glUseProgram(m_SolidRectShader); // 쉐이더 아직 안쓰니까 넘어감
	glEnableVertexAttribArray(0); // 0이 뭔지 아직 안알아도 됨
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); // 0이라는 것은 glEnableVertexAttribArray에 0넣어서

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

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	static float time = 1.f;
	static float addValue = 0.01;
	time += addValue;
	if (time > 1)
		time = 0;

	glUniform1f(uTime, time); // float 포인트 하나


	GLuint aPos = glGetAttribLocation(m_SolidRectShader, "a_Position");
	GLuint aCol = glGetAttribLocation(m_SolidRectShader, "a_Color"); // color가 사용되고 있지 않아서 의미없는 값이 리턴됨

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
	glUseProgram(m_SolidRectShader); // 쉐이더 아직 안쓰니까 넘어감
	glEnableVertexAttribArray(0); // 0이 뭔지 아직 안알아도 됨
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // 첫번째 인자가 0이라는 것은 glEnableVertexAttribArray에 0넣어서

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader); // 쉐이더 아직 안쓰니까 넘어감
	glEnableVertexAttribArray(0); // 0이 뭔지 아직 안알아도 됨
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // 0이라는 것은 glEnableVertexAttribArray에 0넣어서

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

	glEnableVertexAttribArray(aPos); // Test: 이 함수에 들어갈 것은? 
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

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

	glEnableVertexAttribArray(aPos); // Test: 이 함수에 들어갈 것은? 
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


	glEnableVertexAttribArray(aPos); // Test: 이 함수에 들어갈 것은? 
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLifeRatioAmp);
	glEnableVertexAttribArray(aTheta);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLifeRatioAmp, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aTheta, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 10));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsCnt); // GL_LINE_STRIP GL_TRIANGLES

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLifeRatioAmp);
	glDisableVertexAttribArray(aTheta);
}

void Renderer::Lecture4()
{
	// 알파 블렌딩을 켜는 API
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shader = m_SinTrailShader;
	glUseProgram(shader);

	static float time = 0;
	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, time);

	time += 0.005f;

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Velocity");
	GLuint aTime = glGetAttribLocation(shader, "a_Time");
	GLuint aValue = glGetAttribLocation(shader, "a_Value");
	GLuint aColor = glGetAttribLocation(shader, "a_Color");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aTime);
	glEnableVertexAttribArray(aValue);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// (0x, 1y, 2z, 3vx, 4vy, 5vz, 6st, 7lt, 8x, 9y ... )
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aTime, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_vertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aTime);
	glDisableVertexAttribArray(aValue);
	glDisableVertexAttribArray(aColor);
}

void Renderer::Lecture4_2()
{
	GLuint shader = m_SolidRectShader;
	glUseProgram(shader);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");
	/*GLuint aStartLifeRatioAmp = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	GLuint aTheta = glGetAttribLocation(shader, "a_Theta");
	GLuint aColor = glGetAttribLocation(shader, "a_Color");*/

	glEnableVertexAttribArray(aPos); // Test: 이 함수에 들어갈 것은? 
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 17, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 17, (GLvoid*)(sizeof(float) * 15));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_QuadsCnt); // GL_LINE_STRIP

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUV);
}