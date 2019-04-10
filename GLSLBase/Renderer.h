#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void Lecture3();
	void Lecture3_2();
	void Lecture3_3();
	void Lecture3_4();
	void Lecture4();
	void Lecture4_2();

	void CreateVBOQuads(int count, bool is_random, float x = 0, float y = 0, float z = 0);
	//void AddPoints(std::vector<float>& v);
	void DrawQuads();

	void CreateGridMesh();


private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;
	GLuint m_SinTrailShader = 0;

	GLuint m_VBOLecture2 = 0;
	
	GLuint m_VBOQuads = 0;
	int m_QuadsCnt = 0;

	GLuint m_VBOGridMesh = 0;
	int m_VBOGridMesh_Count = 0;

	int m_VBOQuads_vertexCount = 0;
};

