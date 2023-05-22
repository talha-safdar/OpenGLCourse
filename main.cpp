#include <stdio.h>
#include <string.h> // for reproting errors nothing to do with opengl
#include <cmath> // for mathematical operators
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // for raw values

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const float roRadians = 3.14159265f / 180.0f; // scale between ranges to convert degree to number

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// buffers
// VAO=vertex array object holds mutiple VBOs to defie how to draw
// VBO=vertex buffer object
// IBO=Index buffer object

// Vertex Shader
// clamp is used if the number is beyond zero put it between 0 and 1
// model = model itself in space
// view = camera relative to everyehting
// proejction = how the camera trasnelts into what it sees
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";


void CreateObjects() 
{

				// for drawing vertices by using vertices[] by indexing them
				unsigned int indices[] = {
								0, 3, 1,
								1, 3, 2, 
								2, 3, 0,
								0, 1, 2
				};

				GLfloat vertices[] = {
								-1.0f, -1.0f, 0.0f,
								0.0f, -1.0f, 1.0f,
								1.0f, -1.0f, 0.0f,
								0.0f, 1.0f, 0.0f
				};

				Mesh* obj1 = new Mesh();
				obj1->CreateMesh(vertices, indices, 12, 12);
				meshList.push_back(obj1);

				Mesh* obj2 = new Mesh();
				obj2->CreateMesh(vertices, indices, 12, 12);
				meshList.push_back(obj2);
}

void CreateShaders()
{
				Shader* shader1 = new Shader();
				shader1->CreateFromFiles(vShader, fShader);
				shaderList.push_back(*shader1);
}

int main()
{
				mainWindow = Window(800, 600);
				mainWindow.initialise();

				CreateObjects();
				CreateShaders();

				camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

				GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
				glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

				// Loop until window closed
				while (!mainWindow.getShouldClose()) // whe nclicking on X button i the window bar (which window should close)
				{
								GLfloat now = glfwGetTime();
								deltaTime = now - lastTime;
								lastTime = now;

								// Get + handle user input events
								glfwPollEvents(); // check if any event happened (e.g. mosue, keyboard and window moving/resizing)

								// check for key presses for camera
								camera.keyControl(mainWindow.getKeys(), deltaTime);
								camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

								// Clear colors in window and depth buffer bit
								glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // wipe screen of colours, fresh frame, set colour too
								glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // each pixel has more information than colour (light, shade)

								shaderList[0].UseShader();
								uniformModel = shaderList[0].GetModelLocation();
								uniformProjection = shaderList[0].GetProjectionLocation();
								uniformView = shaderList[0].GetViewLocation();

								glm::mat4 model(1.0f);
								// if do translete first rotate then the model is rotate from the original pivot point hence it creates distortion
								// because it rotated after being moved away from the pivot point
								model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); // here can manipulate the matrix of the model
								// model = glm::rotate(model, curAngle * roRadians, glm::vec3(0.0f, 1.0f, 0.0f));
								model = glm::scale(model, glm::vec3(0.4, 0.4f, 1.0f)); // twice as big

								glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
								glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
								glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
								meshList[0]->RenderMesh();

								// for second object
								model = glm::mat4(1.0f); // 1.0 is very important otherwise the model is not visible
								model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
								model = glm::scale(model, glm::vec3(0.4, 0.4f, 1.0f)); // twice as big
								glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
								meshList[1]->RenderMesh();

								glUseProgram(0);

								mainWindow.swapBuffers(); // 2 buffers the front is shown the back is hidden, it is cycle
				}
				return 0;
}