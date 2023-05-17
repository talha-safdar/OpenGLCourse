#include <stdio.h>sizeDirection
#include <string.h> // for reproting errors nothing to do with opengl
#include <cmath> // for mathematical operators

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for raw values

// glm::mat4 model;

// window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float roRadians = 3.14159265f / 180.0f; // scale between ranges to convert degree to number

// buffers
// VAO=vertex array object holds mutiple VBOs to defie how to draw
// VBO=vertex buffer object
// IBO=Index buffer object
GLuint VAO, VBO, IBO, shader, uniformModel;

// not using GL datatype
bool direction = true; // for check direction
float triOffset = 0.0f; // offset starts from zero and move left or right
float triMaxOffset = 0.7f; // when hit 0.7 while moving to the right, it will toggle the direction boolean
float triIncrement = 0.01f; // increment by

float curAngle = 0.0f; // for rotating

float sizeDirection = true; // for size
float curSize = 0.4f; 
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
// clamp is used if the number is beyond zero put it between 0 and 1
static const char* vShader = "																																		\n\
#version 330																																																				\n\
																																																																\n\
layout (location = 0) in vec3 pos;																														\n\
																																																																\n\
out vec4 vCol;																																																																\n\
																																																																\n\
uniform mat4 model;																																												 \n\
																																																																\n\
void main()																																																					\n\
{																																																															\n\
				gl_Position = model * vec4(pos, 1.0);																							\n\
				vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);																		\n\
}";

// Fragment Shader
static const char* fShader = "																																		\n\
#version 330																																																				\n\
																																																																\n\
in vec4 vCol;																																																																\n\
																																																																\n\
out vec4 colour;																					 																										\n\
																																																																\n\
void main()																																																					\n\
{																																																															\n\
				colour = vCol;																																														\n\
}";


void CreateTriangle() 
{

				// for drawing vertices by using vertices[] by indexing them
				unsigned int indices[] = {
								0, 3, 1, // 
								1, 3, 2, // 
								2, 3, 0,
								0, 1, 2
				};

				GLfloat vertices[] = {
								-1.0f, -1.0f, 0.0f,
								0.0f, -1.0f, 1.0f,
								1.0f, -1.0f, 0.0f,
								0.0f, 1.0f, 0.0f
				};
								
				glGenVertexArrays(1, &VAO); // create VEO: (amount of array, ID of array)
				glBindVertexArray(VAO); // binding

				glGenBuffers(1, &IBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

								glGenBuffers(1, &VBO);
								glBindBuffer(GL_ARRAY_BUFFER, VBO);
												glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // static values won't change

												glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // last arguement from which element to start in the array
												glEnableVertexAttribArray(0); // location with 0

								glBindBuffer(GL_ARRAY_BUFFER, 0);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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
								printf("Error compiling the '%d' shader: '&s'\n", shaderType, eLog);
								return;
				}

				// attach the shader
				glAttachShader(theProgram, theShader);
}

void CompileShader()
{
				shader = glCreateProgram();
				if (!shader)
				{
								printf("Error creating shader program!\n");
								// it can be error handled
								return;
				}

				AddShader(shader, vShader, GL_VERTEX_SHADER);
				AddShader(shader, fShader, GL_FRAGMENT_SHADER);

				// link/validate the program
				GLint result = 0; // result of the two functions about to perform
				GLchar eLog[1024] = { 0 }; // place to log the error

				glLinkProgram(shader); // create executables on the GPU linkign all the programs together
				// still need to check if program linked properly and then validate it is working for the openGL settings
				glGetProgramiv(shader, GL_LINK_STATUS, &result);
				if (!result)
				{
								glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
								printf("Error linking program: '&s'\n", eLog);
								return;
				}

				// validate program
				glValidateProgram(shader);
				glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
				if (!result)
				{
								glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
								printf("Error validating program: '&s'\n", eLog);
								return;
				}

				uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
				// initialize GLFW
				if (glfwInit() == false) // if it fails
				{
								printf("GLFW initialization failed!"); // message error
								glfwTerminate(); // terminate it
								return 1; // standard 1=error, 2=kernel
				}

				// Setup GLFW window proeprties
				// OpenGL version
				// MAX and MIN verison
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

				// stable and NOT backward compatible profile (depecrated or removed features)
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				
				// make it forward compatible
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

				GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLCourse", NULL, NULL);
				if (!mainWindow)
				{
								printf("GLFW window creating failed!");
								glfwTerminate();
								return 1;
				}

				// Get buffer size information
				int bufferWidth, bufferHeight; // for the window to get its size
				glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // get the window's sizes and store into variable address

				// Set context for GLEW to use
				// tell GLEW: if multiple windows you can switch between them
				glfwMakeContextCurrent(mainWindow);

				// allow modern extension features
				glewExperimental = GL_TRUE; // accessing experimental stuff (enables them)

				if (glewInit() != GLEW_OK) // if GLEW features are false
				{
								// printf("GLW initialization failed!");
								printf("Error: % s", glewGetErrorString(glewInit()));
								glfwDestroyWindow(mainWindow); // GLFW remove window
								glfwTerminate(); // GLFW teriminate process
								return 1; // return error
				}

				// depth test is impoartant
				glEnable(GL_DEPTH_TEST);

				// Setup viewport size (like canvas in JavaFX, the drawable part inside the window)
				// use buffer size because it obtaines the exact inside window size
				glViewport(0, 0, bufferWidth, bufferHeight);

				CreateTriangle();
				CompileShader();

				// Loop until window closed
				while (!glfwWindowShouldClose(mainWindow)) // whe nclicking on X button i the window bar (which window should close)
				{
								// Get + handle user input events
								glfwPollEvents(); // check if any event happened (e.g. mosue, keyboard and window moving/resizing)

								if (direction)
								{
												triOffset += triIncrement;
								}
								else
								{
												triOffset -= triIncrement;
								}

								if (abs(triOffset) >= triMaxOffset)
								{
												direction = !direction;
								}

								curAngle += 0.7f;
								if (curAngle >= 300)
								{
												curAngle -= 300;
								}

								if (sizeDirection)
								{
												curSize += 0.01f;
								}
								else
								{
												curSize -= 0.01f;
								}

								if (curSize >= maxSize || curSize <= minSize)
								{
												sizeDirection = !sizeDirection;
								}

								// Clear colors in window and depth buffer bit
								glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // wipe screen of colours, fresh frame, set colour too
								glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // each pixel has more information than colour (light, shade)

								glUseProgram(shader);
								
								glm::mat4 model(1.0f);
								// if do translete first rotate then the model is rotate from the original pivot point hence it creates distortion
								// because it rotated after being moved away from the pivot point
								model = glm::rotate(model, curAngle * roRadians, glm::vec3(0.0f, 1.0f, 0.0f));
								// model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f)); // here can manipulate the matrix of the model
								model = glm::scale(model, glm::vec3(0.4, 0.4f, 1.0f)); // twice as big

								// glUniform1f(uniformModel, triOffset);
								glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
												glBindVertexArray(VAO);
																// glDrawArrays(GL_TRIANGLES, 0, 3);
																glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
																// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
																glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
												glBindVertexArray(0);
								glUseProgram(0);

								glfwSwapBuffers(mainWindow); // 2 buffers the front is shown the back is hidden, it is cycle
				}

				return 0;
}