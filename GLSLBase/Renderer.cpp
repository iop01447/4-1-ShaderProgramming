#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <array>
#include <list>
#include <string>
#include <cassert>
#define P_NUM 1000
#define P_SIZE 0.05f

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
	m_TEST0318Shader = CompileShaders("./Shaders/Line.vs", "./Shaders/Line.fs");
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect2.vs", "./Shaders/SolidRect2.fs");
	//m_TestRectWhite = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	//m_TEST0320ShaderMoveX = CompileShaders("./Shaders/SolidRectMoveX.vs", "./Shaders/SolidRectTest.fs");
	m_TestSprite = CompileShaders("./Shaders/spriteTest.vs", "./Shaders/spriteTest.fs");
	m_TestFill = CompileShaders("./Shaders/Fill.vs", "./Shaders/Fill.fs");
	//m_TESTShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRectTest.fs");
	//m_TEST0318Shader = CompileShaders("./Shaders/SolidRectTest.vs", "./Shaders/SolidRectTest.fs");
	//m_TEST0320ShaderBB = CompileShaders("./Shaders/SolidRectTest2.vs", "./Shaders/SolidRectTest.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();
	JHCreateTex();

	//Init Matrices
	InitMatrices();
}

void Renderer::InitMatrices()
{
	//Calc ortho projection matrix
	m_OrthoProjMat4 = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 2.f);

	//Calc view matrix
	m_CameraPosVec3 = glm::vec3(0.f, -1.f, 0.f); // 0 -1 0 
	m_CameraUpVec3 = glm::vec3(0.f, 0.f, 1.f); // 0 0 0
	m_CameraLookatVec3 = glm::vec3(0.f, 0.f, 0.f); // 0 0 1
	m_ViewMat4 = glm::lookAt(m_CameraPosVec3, m_CameraLookatVec3, m_CameraUpVec3);

	m_ViewProjMat4 = m_OrthoProjMat4 * m_ViewMat4;
}

void Renderer::CreateVertexBufferObjects()
{
	float rectsize{ 0.5 };
	float rect[]
		=
	{
		-rectsize, -rectsize, 0.f, 0,0,
		-rectsize, rectsize, 0.f, 0,1,
		rectsize, rectsize, 0.f,	1,1,	//Triangle1

		-rectsize, -rectsize, 0.f,0,0,
		rectsize, rectsize, 0.f, 1,1,
		rectsize, -rectsize, 0.f,1,0	//Triangle2
	};

	rectsize = 1;
	float rect3[]
		=
	{
		-rectsize, -rectsize, 0.f, 0,0,
		-rectsize, rectsize, 0.f, 0,1,
		rectsize, rectsize, 0.f,	1,1,	//Triangle1

		-rectsize, -rectsize, 0.f,0,0,
		rectsize, rectsize, 0.f, 1,1,
		rectsize, -rectsize, 0.f,1,0	//Triangle2
	};

	float rect1[]
		=
	{
		-0.5, -0.3, 0.f, -0.5, 0.5, 0.f, 0.3, 0.5, 0.f, //Triangle1
		-0.3, -0.5, 0.f,  0.5, 0.3, 0.f, 0.5, -0.5, 0.f, //Triangle2
	};

	float rect2[]
		=
	{
		-1, 0.5, 0.f, 0.5, -0.5, 1, 0.f , 0.5, -0.5, 0.5, 0.f, 0.5, //Triangle1
		1, -0.5, 0.f, 0.5,  0.5, -1, 0.f, 0.5, 0.5, -0.5, 0.f, 0.5, //Triangle2
	};


	std::array< std::vector<float> ,3>vQuadlist;
	//파티클 그리기
	{

		const float size{ P_SIZE };
		for (int i = 0; i < P_NUM; i++)
		{
			float pos_x{ (rand() % 2000 - 1000)* 0.001f };
			float pos_y{ (rand() % 2000 - 1000)* 0.001f };

			//pos_x = 0;
			//pos_y = 0;

			float dir_x{ (rand() % 2000 - 1000)* 0.001f };
			float dir_y{ (rand() % 2000 - 1000)* 0.001f };

			float emitTime{ 0.5f+(rand() % 2000 - 1000)* 0.0005f };
			float lifeTime{ 0.5f+(rand() % 2000 - 1000)* 0.0005f };

			float tmp=dir_x*dir_x + dir_y * dir_y;
			dir_x /= tmp;
			dir_y /= tmp;


			vQuadlist[0].insert(
				vQuadlist[0].end(),
				{
					pos_x - size, pos_y - size, dir_x,dir_y,emitTime,lifeTime,
					pos_x - size, pos_y + size, dir_x,dir_y,emitTime,lifeTime,
					pos_x + size, pos_y + size, dir_x,dir_y, emitTime,lifeTime,//Triangle1

					pos_x - size, pos_y - size, dir_x,dir_y,emitTime,lifeTime,
					pos_x + size,pos_y + size, dir_x, dir_y,emitTime,lifeTime,
					pos_x + size, pos_y - size, dir_x,dir_y, emitTime,lifeTime,//Triangle2
				}
			);

		}
	}

	//바닥스트립 그리기
	{

		const UINT quadsize{50 };
		const float quadsizef{ (1.f / quadsize) * 1.f };
		for (int i = 0; i < quadsize; i++)
		{
			for (int j = 0; j < quadsize; j++)
			{
				float pos_x{ -0.5f + i * quadsizef };
				float pos_y{ -0.5f + j * quadsizef };


				vQuadlist[1].insert(
					vQuadlist[1].end(),
					{
						pos_x , pos_y , 0.f, pos_x , pos_y + quadsizef, 0.f, pos_x + quadsizef, pos_y + quadsizef, 0.f, //Triangle1
						pos_x + quadsizef, pos_y , 0.f, pos_x , pos_y , 0.f, pos_x + quadsizef,pos_y + quadsizef, 0.f, //Triangle2
					}
				);
			}

		}
		m_vb0size = vQuadlist[1].size();
	}


	//중간에서 출발할해서 x축 움직일 그리기
	{

		const float size{ P_SIZE };
		for (int i = 0; i < P_NUM; i++)
		{
			float pos_x{ 0 };
			float pos_y{ 0 };

			float dir_x{ ((float)rand() / (float)RAND_MAX) *7};
			float dir_y{ ((float)rand() / (float)RAND_MAX) *7};

			float speed{ 2.f +1.f * ((float)rand() / (float)RAND_MAX) };


			float start	= ((float)rand() / (float)RAND_MAX);
			float Life	=1;
			float Ratio	=4	*((float)rand()/(float)RAND_MAX);
			float Amp	=0.5*((float)rand()/(float)RAND_MAX);

			//float emitTime{ 0.5f + (rand() % 2000 - 1000)* 0.0005f };
			//float lifeTime{ 0.5f + (rand() % 2000 - 1000)* 0.0005f };

			vQuadlist[2].insert(
				vQuadlist[2].end(),
				{
					pos_x - size, pos_y - size, start,Life,Ratio,Amp,	dir_x,dir_y,speed,
					pos_x - size, pos_y + size,  start,Life,Ratio,Amp,dir_x,dir_y,speed,
					pos_x + size, pos_y + size,  start,Life,Ratio,Amp,dir_x,dir_y,speed,		//Triangle1

					pos_x - size, pos_y - size, start,Life,Ratio,Amp,dir_x,dir_y,speed,
					pos_x + size,pos_y + size,	start,Life,Ratio,Amp,dir_x,dir_y,speed,
					pos_x + size, pos_y - size, start,Life,Ratio,Amp,dir_x,dir_y,speed,			//Triangle2
				}
			);
		}
	}

	glGenBuffers(7, m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[0]); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect1), rect1, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect2), rect2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[3]);
	glBufferData(GL_ARRAY_BUFFER, vQuadlist[0].size() * sizeof(float), &vQuadlist[0][0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[4]);
	glBufferData(GL_ARRAY_BUFFER, vQuadlist[1].size() * sizeof(float), &vQuadlist[1][0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[5]);
	glBufferData(GL_ARRAY_BUFFER, vQuadlist[2].size() * sizeof(float), &vQuadlist[2][0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect3), rect3, GL_STATIC_DRAW);
}

int gDummyVertexCount = 0;
void Renderer::GenGrid()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 8;
	int pointCountY = 8;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	gDummyVertexCount = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

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
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &VBO_DummyMesh);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DummyMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
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

void Renderer::JHCreateTex()
{
	static const GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};

	glGenTextures(1, &m_TestTexture);

	glBindTexture(GL_TEXTURE_2D, m_TestTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, &checkerboard[0]);

	
	char names[NUM_TEX][20]{ "asdf.png", "asdf1.png" , "asdf2.png" ,"asdf31.png","asdf3.png", "./Textures/cat.png" };

	//glGenTextures(NUM_TEX, m_TestTexarr);
	for (int i = 0; i < NUM_TEX; ++i)
	{
		auto& tex = m_TestTexarr[i];
		//glBindTexture(GL_TEXTURE_2D, tex);
		tex = CreatePngTexture(names[i]);
	}
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}


void Renderer::Test_CULINE(float time)
{
//	glLineWidth(3);

	glUseProgram(m_TEST0318Shader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[4]);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	GLuint idTest = glGetUniformLocation(m_TEST0318Shader, "gTime");
	glUniform1f(idTest, time/30);
	GLuint uPoints = glGetUniformLocation(m_TEST0318Shader, "u_Points");
	float points[] = { 0.0f, 0.0f, 0.2f, 0.35f, -0.4f, -0.33f, 0.1f, -0.27f, -4.5f, 2.7f };
	glUniform2fv(uPoints, 5, points);
	GLuint uViewProjMat = glGetUniformLocation(m_TEST0318Shader, "u_ViewProjMat");
	glUniformMatrix4fv(uViewProjMat, 1, GL_FALSE, &m_ViewProjMat4[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TestTexarr[5]);

	glDrawArrays(GL_TRIANGLES, 0, m_vb0size); // GL_TRIANGLES/ GL_LINE_STRIP

	glDisableVertexAttribArray(attribPosition);
}

//void Renderer::Test_0313(float time)
//{
//	glUseProgram(m_TEST0320ShaderBB);
//
//	int attribPosition = glGetAttribLocation(m_TEST0320ShaderBB, "a_Position");
//	int attribPositionDir = glGetAttribLocation(m_TEST0320ShaderBB, "a_Dir");
//	int attribPositionELtime = glGetAttribLocation(m_TEST0320ShaderBB, "a_elTime");
//	glEnableVertexAttribArray(attribPosition);
//	glEnableVertexAttribArray(attribPositionDir);
//	glEnableVertexAttribArray(attribPositionELtime);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[3]);
//	glVertexAttribPointer(attribPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
//	glVertexAttribPointer(attribPositionDir, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 2));
//	glVertexAttribPointer(attribPositionELtime, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));
//
//
//	GLint idTest = glGetUniformLocation(m_TEST0320ShaderBB, "gTime");
//	glUniform1f(idTest, time);
//
//	glDrawArrays(GL_TRIANGLES, 0, P_NUM * 6);
//
//	glDisableVertexAttribArray(attribPosition);
//	glDisableVertexAttribArray(attribPositionELtime);
//	glDisableVertexAttribArray(attribPositionDir);
//}

void Renderer::TestMoveX(float time)
{
	auto shader = m_TEST0320ShaderMoveX;
	auto varr = m_VBORect[5];

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);

	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	int attribPositionDir = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	int attribPositionspped = glGetAttribLocation(shader, "a_f2DirfSpeed");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribPositionDir);
	glEnableVertexAttribArray(attribPositionspped);

	glBindBuffer(GL_ARRAY_BUFFER, varr);

	glVertexAttribPointer(attribPosition, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(attribPositionDir, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 2));
	glVertexAttribPointer(attribPositionspped, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 6));


	GLint idTest = glGetUniformLocation(shader, "gTime");
	glUniform1f(idTest, time);


	glDrawArrays(GL_TRIANGLES, 0, P_NUM * 6); //삼각형두개로사각형하나 그릴꺼라서

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribPositionDir);
	glDisableVertexAttribArray(attribPositionspped);
	glDisable(GL_BLEND);

}

void Renderer::TestWhiteRect(float ftime)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
	auto shader = m_TestRectWhite;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	int aUV = glGetAttribLocation(shader, "a_UV");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[0]);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	GLint idTest = glGetUniformLocation(shader, "gTime");
	glUniform1f(idTest, ftime);

	float pointarr[]= {
		0,0,
		-0.61,-0.31,
		-0.3,0.51,
		0.6,-0.1,
		0.1,-0.7,
		0.5,0.2
	};

	GLint point = glGetUniformLocation(shader, "c_Point");
	glUniform2fv(point, 5, pointarr);


	glDrawArrays(GL_TRIANGLES, 0,6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(aUV);
	glDisable(GL_BLEND);
}

void Renderer::TestFill()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
	auto shader = m_TestFill;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	int aUV = glGetAttribLocation(shader, "a_UV");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[0]);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(aUV);

	glDisable(GL_BLEND);
}


void Renderer::TestUVRect(float time)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
	auto shader = m_TestFill;
	glUseProgram(shader);

	auto attribPosition	= glGetAttribLocation(shader, "a_Position");
	auto aUV				= glGetAttribLocation(shader, "a_UV");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(aUV);

	GLint idTest = glGetUniformLocation(shader, "gTime");
	glUniform1f(idTest, time);

	int uniformTex = glGetUniformLocation(shader, "uTexSampler");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TestTexarr[0]);

	int uniformTex1 = glGetUniformLocation(shader, "uTexSampler2");
	glUniform1i(uniformTex1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TestTexarr[1]);

	int uniformTex2 = glGetUniformLocation(shader, "uTexSampler1");
	glUniform1i(uniformTex2, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_TestTexarr[2]);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[0]);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(aUV);

	glDisable(GL_BLEND);
}

void Renderer::TestNumDraw(float time,int num)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
	auto shader = m_TestSprite;
	glUseProgram(shader);

	auto attribPosition = glGetAttribLocation(shader, "a_Position");
	auto aUV = glGetAttribLocation(shader, "a_UV");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(aUV);

	GLint idTest = glGetUniformLocation(shader, "gTime");
	glUniform1f(idTest, time);
	GLint idgNum = glGetUniformLocation(shader, "gNum");
	glUniform1i(idgNum, num);
	GLint idgPosition = glGetUniformLocation(shader, "gPosition");
	glUniform3f(idgPosition, cos(time)*0.5, 0.5*sin(time), 0);
	GLint idgSize = glGetUniformLocation(shader, "gSize");
	glUniform2f(idgSize, 0.5,0.5);
	
	int uniformTex = glGetUniformLocation(shader, "uTexSampler");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TestTexarr[4]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[6]);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(aUV);

	glDisable(GL_BLEND);
}


void Renderer::TestNumDrawO(float time, int num)
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
	auto shader = m_TestSprite;
	glUseProgram(shader);

	auto attribPosition = glGetAttribLocation(shader, "a_Position");
	auto aUV = glGetAttribLocation(shader, "a_UV");
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(aUV);

	GLint idTest = glGetUniformLocation(shader, "gTime");
	glUniform1f(idTest, time);

	int uniformTex = glGetUniformLocation(shader, "uTexSampler");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TestTexarr[3]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect[6]);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	GLint idgNum = glGetUniformLocation(shader, "gNum");
	GLint idgPosition = glGetUniformLocation(shader, "gPosition");

	int numt = num;
	std::list<int> numv;
	while (true)
	{
		numv.push_front(numt % 10);
		numt=numt / 10;
		if (numt == 0)break;
	}


	float sizeN = numv.size();//? 몇개를 그리냐?
	float numdrawdel =1.f/ sizeN; //그럼 얼만큼띄워서 그리냐?
	float numx = -(sizeN-1)* numdrawdel*0.5; //그럼 시작위치는 어떠냐? 00
	float numy = 0;

	//numv.size
	GLint idgSize = glGetUniformLocation(shader, "gSize");
	glUniform2f(idgSize, numdrawdel, numdrawdel);

	int i = 0;
	for (auto numvi:numv)
	{
		glUniform1i(idgNum, numvi);
		glUniform3f(idgPosition, numx, numy, 0);
		numx += numdrawdel;
		glDrawArrays(GL_TRIANGLES, 0, 6);
		i++;
	}


	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(aUV);

	//glDisable(GL_BLEND);
}