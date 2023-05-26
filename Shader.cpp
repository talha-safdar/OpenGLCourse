#include "Shader.h"

Shader::Shader()
{
				shaderID = 0;
				uniformModel = 0;
				uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
				CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
				std::string vertexString = ReadFile(vertexLocation);
				std::string fragmentString = ReadFile(fragmentLocation);
				const char* vertexCode = vertexString.c_str(); // convert to string format
				const char* fragmentCode = fragmentString.c_str(); // convert to string format

				CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
				std::string content;
				std::ifstream fileStream(fileLocation, std::ios::in);

				if (!fileStream.is_open())
				{
								printf("Failed to read %s! File doesn't exist.", fileLocation);
								return "";
				}

				std::string line = "";
				while (!fileStream.eof())
				{
								std::getline(fileStream, line);
								content.append(line + "\n"); // not necessarily needed but good for debugging
				}

				fileStream.close();
				return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
				shaderID = glCreateProgram();
				if (!shaderID)
				{
								printf("Error creating shader program!\n");
								// it can be error handled
								return;
				}

				AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
				AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

				// link/validate the program
				GLint result = 0; // result of the two functions about to perform
				GLchar eLog[1024] = { 0 }; // place to log the error

				glLinkProgram(shaderID); // create executables on the GPU linkign all the programs together
				// still need to check if program linked properly and then validate it is working for the openGL settings
				glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
				if (!result)
				{
								glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
								printf("Error linking program: '%s'\n", eLog);
								return;
				}

				// validate program
				glValidateProgram(shaderID);
				glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
				if (!result)
				{
								glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
								printf("Error validating program: '%s'\n", eLog);
								return;
				}

				uniformProjection = glGetUniformLocation(shaderID, "projection");
				uniformModel = glGetUniformLocation(shaderID, "model");
				uniformView = glGetUniformLocation(shaderID, "view");
				uniformAmbientColour = glGetUniformLocation(shaderID, "directionalLight.colour");
				uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
}

GLuint Shader::GetProjectionLocation()
{
				return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
				return uniformModel;
}

GLuint Shader::GetViewLocation()
{
				return uniformView;
}

GLuint Shader::GetAmbientColourLocation()
{
				return uniformAmbientColour;
}
GLuint Shader::GetAmbientIntensityLocation()
{
				return uniformAmbientIntensity;
}

void Shader::UseShader()
{
				// should check if equals to zero first to catch errors
				glUseProgram(shaderID);
}

void Shader::ClearShader()
{
				if (shaderID != 0)
				{
								glDeleteProgram(shaderID); // delete program from GPU
								shaderID = 0;
				}

				uniformModel = 0; 
				uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
				GLuint theShader = glCreateShader(shaderType); // create empty shader of specific type

				const GLchar* theCode[1];
				theCode[0] = shaderCode;

				GLint codeLength[1];
				codeLength[0] = strlen(shaderCode); // length of the code

				// set up shader soruce code
				glShaderSource(theShader, 1, theCode, codeLength);
				glCompileShader(theShader);

				// error check
				// link/validate the program
				GLint result = 0; // result of the two functions about to perform
				GLchar eLog[1024] = { 0 }; // place to log the error

				// still need to check if program linked properly and then validate it is working for the openGL settings
				glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
				if (!result)
				{
								glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
								printf("Error compiling the '%d' shader: '%s'\n", shaderType, eLog);
								return;
				}

				// attach the shader
				glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
				ClearShader();
}
