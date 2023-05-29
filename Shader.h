#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
				Shader(); // constructor

				void CreateFromString(const char* vertexCode, const char* fragmentCode);
				void CreateFromFiles(const char* vertex, const char* fragmentLocation);

				std::string ReadFile(const char* fileLocation);

				GLuint GetProjectionLocation();
				GLuint GetModelLocation();
				GLuint GetViewLocation();
				GLuint GetAmbientIntensityLocation();
				GLuint GetAmbientColourLocation();
				GLuint GetDiffuseIntensityLocation();
				GLuint GetDirectionLocation();

				void UseShader();
				void ClearShader();

				~Shader(); // destructor

private:
				GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection;

				void CompileShader(const char* vertexCode, const char* fragmentCode);
				void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

