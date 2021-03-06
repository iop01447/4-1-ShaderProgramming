#pragma once
#define NUM_TEX 6
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

	void GenGrid();

	void Test_CULINE(float time);
	//void Test_0313(float time);
	void TestMoveX(float time);
	void TestWhiteRect(float time);
	void TestFill();
	void TestUVRect(float time);
	void TestNumDraw(float time, int num);
	void TestNumDrawO(float time,int num);
	void Renderer::DrawTextureRect(GLuint tex, float x, float y, float sx, float sy);
	GLuint CreateFBO(int sx, int sy, GLuint* tex);
	void TestFBO();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);


	void JHCreateTex();

	void InitMatrices();

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_vb0size;
	GLuint m_VBORect[6]{};
	GLuint m_TestTexture;
	GLuint m_TestTexarr[NUM_TEX];
	GLuint VBO_DummyMesh;
	GLuint m_HeightMap;
	GLuint m_GrassTexture;
	GLuint m_SnowTexture;

	GLuint m_SolidRectShader = 0;
	GLuint m_TEST0318Shader = 0;
	//GLuint m_TESTShader = 0;
	//GLuint m_TEST0318Shader = 0;
	//GLuint m_TEST0320ShaderBB = 0;
	GLuint m_TEST0320ShaderMoveX = 0;
	GLuint m_SinTrailShader = 0;
	GLuint m_TestRectWhite = 0;
	GLuint m_TestFill = 0;
	GLuint m_TextureRectShader = 0;

	GLuint m_TestSprite = 0;


	//Matrices
	glm::mat4 m_ViewMat4;
	glm::mat4 m_OrthoProjMat4;
	glm::mat4 m_PersProjMat4;
	glm::mat4 m_ViewProjMat4;
	glm::vec3 m_CameraPosVec3;
	glm::vec3 m_CameraUpVec3;
	glm::vec3 m_CameraLookatVec3;

	GLuint m_FBOTexture0 = 0;
	GLuint m_FBOTexture1 = 0;
	GLuint m_FBOTexture2 = 0;
	GLuint m_FBOTexture3 = 0;
	GLuint m_DepthRenderBuffer = 0;

	//FrameBufferObjects
	GLuint m_FBO0 = 0;
};

